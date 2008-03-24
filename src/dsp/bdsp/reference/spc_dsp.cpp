// http://www.slack.net/~ant/

#include "spc_dsp.h"

#include <limits.h>
#include <string.h>

/* Copyright (C) 2007 Shay Green. This module is free software; you
can redistribute it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version. This
module is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details. You should have received a copy of the GNU Lesser General Public
License along with this module; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA */

// Volume registers and efb are signed! Easy to forget int8_t cast.
// Prefixes are to avoid accidental use of locals with same names.

// Global registers
enum {
    r_mvoll = 0x0C, r_mvolr = 0x1C,
    r_evoll = 0x2C, r_evolr = 0x3C,
    r_kon   = 0x4C, r_koff  = 0x5C,
    r_flg   = 0x6C, r_endx  = 0x7C,
    r_efb   = 0x0D, r_pmon  = 0x2D,
    r_non   = 0x3D, r_eon   = 0x4D,
    r_dir   = 0x5D, r_esa   = 0x6D,
    r_edl   = 0x7D,
    r_fir   = 0x0F // 8 coefficients at 0x0F, 0x1F ... 0x7F
};

// Voice registers
enum {
	v_voll   = 0x00, v_volr   = 0x01,
	v_pitchl = 0x02, v_pitchh = 0x03,
	v_srcn   = 0x04, v_adsr0  = 0x05,
	v_adsr1  = 0x06, v_gain   = 0x07,
	v_envx   = 0x08, v_outx   = 0x09
};

// Internal envelope modes
enum env_mode_t { env_release, env_attack, env_decay, env_sustain };

// Internal voice state
enum { brr_buf_size   = 12 };
enum { brr_block_size =  9 };
typedef struct voice_t
{
	int buf [brr_buf_size*2];// decoded samples (twice the size to simplify wrap handling)
	int buf_pos;            // place in buffer where next samples will be decoded
	int interp_pos;         // relative fractional position in sample (0x1000 = 1.0)
	int brr_addr;           // address of current BRR block
	int brr_offset;         // current decoding offset in BRR block
	uint8_t* regs;          // pointer to voice's DSP registers
	int vbit;               // bitmask for voice: 0x01 for voice 0, 0x02 for voice 1, etc.
	int kon_delay;          // KON delay/current setup phase
	enum env_mode_t env_mode;
	int env;                // current envelope level
	int t_envx_out;
	int hidden_env;         // used by GAIN mode 7, very obscure quirk
} voice_t;

static voice_t m_voice_state [spc_dsp_voice_count];
static uint8_t* m_ram; // 64K shared RAM between DSP and SMP
spc_dsp_t m_spc_dsp;
spc_dsp_sample_t* m_spc_dsp_out_begin;
spc_dsp_sample_t* m_spc_dsp_out;
spc_dsp_sample_t* m_spc_dsp_out_end;

// "Member" access
#define m           m_spc_dsp

// Access global DSP register
#define REG(n)      m.regs [r_##n]

// Access voice DSP register
#define VREG(r,n)   r [v_##n]

// if ( io < -32768 ) io = -32768;
// if ( io >  32767 ) io =  32767;
#define CLAMP16( io )\
{\
	if ( (int16_t) io != io )\
		io = 0x7FFF ^ (io >> 31);\
}

// Gaussian interpolation

static short const gauss [512] =
{
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,
   2,   2,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   5,   5,   5,   5,
   6,   6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,  10,  10,  10,
  11,  11,  11,  12,  12,  13,  13,  14,  14,  15,  15,  15,  16,  16,  17,  17,
  18,  19,  19,  20,  20,  21,  21,  22,  23,  23,  24,  24,  25,  26,  27,  27,
  28,  29,  29,  30,  31,  32,  32,  33,  34,  35,  36,  36,  37,  38,  39,  40,
  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,
  58,  59,  60,  61,  62,  64,  65,  66,  67,  69,  70,  71,  73,  74,  76,  77,
  78,  80,  81,  83,  84,  86,  87,  89,  90,  92,  94,  95,  97,  99, 100, 102,
 104, 106, 107, 109, 111, 113, 115, 117, 118, 120, 122, 124, 126, 128, 130, 132,
 134, 137, 139, 141, 143, 145, 147, 150, 152, 154, 156, 159, 161, 163, 166, 168,
 171, 173, 175, 178, 180, 183, 186, 188, 191, 193, 196, 199, 201, 204, 207, 210,
 212, 215, 218, 221, 224, 227, 230, 233, 236, 239, 242, 245, 248, 251, 254, 257,
 260, 263, 267, 270, 273, 276, 280, 283, 286, 290, 293, 297, 300, 304, 307, 311,
 314, 318, 321, 325, 328, 332, 336, 339, 343, 347, 351, 354, 358, 362, 366, 370,
 374, 378, 381, 385, 389, 393, 397, 401, 405, 410, 414, 418, 422, 426, 430, 434,
 439, 443, 447, 451, 456, 460, 464, 469, 473, 477, 482, 486, 491, 495, 499, 504,
 508, 513, 517, 522, 527, 531, 536, 540, 545, 550, 554, 559, 563, 568, 573, 577,
 582, 587, 592, 596, 601, 606, 611, 615, 620, 625, 630, 635, 640, 644, 649, 654,
 659, 664, 669, 674, 678, 683, 688, 693, 698, 703, 708, 713, 718, 723, 728, 732,
 737, 742, 747, 752, 757, 762, 767, 772, 777, 782, 787, 792, 797, 802, 806, 811,
 816, 821, 826, 831, 836, 841, 846, 851, 855, 860, 865, 870, 875, 880, 884, 889,
 894, 899, 904, 908, 913, 918, 923, 927, 932, 937, 941, 946, 951, 955, 960, 965,
 969, 974, 978, 983, 988, 992, 997,1001,1005,1010,1014,1019,1023,1027,1032,1036,
1040,1045,1049,1053,1057,1061,1066,1070,1074,1078,1082,1086,1090,1094,1098,1102,
1106,1109,1113,1117,1121,1125,1128,1132,1136,1139,1143,1146,1150,1153,1157,1160,
1164,1167,1170,1174,1177,1180,1183,1186,1190,1193,1196,1199,1202,1205,1207,1210,
1213,1216,1219,1221,1224,1227,1229,1232,1234,1237,1239,1241,1244,1246,1248,1251,
1253,1255,1257,1259,1261,1263,1265,1267,1269,1270,1272,1274,1275,1277,1279,1280,
1282,1283,1284,1286,1287,1288,1290,1291,1292,1293,1294,1295,1296,1297,1297,1298,
1299,1300,1300,1301,1302,1302,1303,1303,1303,1304,1304,1304,1304,1304,1305,1305,
};

static inline int interpolate( voice_t const* const v )
{
	// Make pointers into gaussian based on fractional position between samples
	int offset = v->interp_pos >> 4 & 0xFF;
	short const* fwd = gauss + 255 - offset;
	short const* rev = gauss       + offset; // mirror left half of gaussian
	
	int const* in = &v->buf [(v->interp_pos >> 12) + v->buf_pos];
	int out;
	out  = (fwd [  0] * in [0]) >> 11;
	out += (fwd [256] * in [1]) >> 11;
	out += (rev [256] * in [2]) >> 11;
	out = (int16_t) out;
	out += (rev [  0] * in [3]) >> 11;
	
	CLAMP16( out );
	out &= ~1;
	return out;
}


//// Counters

enum { simple_counter_range = 2048 * 5 * 3 }; // 30720

static unsigned short const counter_rates [32] =
{
   simple_counter_range + 1, // never fires
          2048, 1536,
	1280, 1024,  768,
	 640,  512,  384,
	 320,  256,  192,
	 160,  128,   96,
	  80,   64,   48,
	  40,   32,   24,
	  20,   16,   12,
	  10,    8,    6,
	   5,    4,    3,
	         2,
	         1
};

static unsigned short const counter_offsets [32] =
{
	  1, 0, 1040,
	536, 0, 1040,
	536, 0, 1040,
	536, 0, 1040,
	536, 0, 1040,
	536, 0, 1040,
	536, 0, 1040,
	536, 0, 1040,
	536, 0, 1040,
	536, 0, 1040,
	     0,
	     0
};

static inline void init_counters( void ) { }

static inline void run_counters( void )
{
	if ( --m.counter < 0 )
		m.counter = simple_counter_range - 1;
}

static inline unsigned read_counter( int rate )
{
	return ((unsigned) m.counter + counter_offsets [rate]) % counter_rates [rate];
}


//// Envelope

static inline void run_envelope( voice_t* const v )
{
	int env = v->env;
	if ( v->env_mode == env_release ) // 60%
	{
		if ( (env -= 0x8) < 0 )
			env = 0;
		v->env = env;
	}
	else
	{
		int rate;
		int env_data = VREG(v->regs,adsr1);
		if ( m.t_adsr0 & 0x80 ) // 99% ADSR
		{
			if ( v->env_mode >= env_decay ) // 99%
			{
				env--;
				env -= env >> 8;
				rate = env_data & 0x1F;
				if ( v->env_mode == env_decay ) // 1%
					rate = (m.t_adsr0 >> 3 & 0x0E) + 0x10;
			}
			else // env_attack
			{
				rate = (m.t_adsr0 & 0x0F) * 2 + 1;
				env += rate < 31 ? 0x20 : 0x400;
			}
		}
		else // GAIN
		{
			env_data = VREG(v->regs,gain);
			int mode = env_data >> 5;
			if ( mode < 4 ) // direct
			{
				env = env_data * 0x10;
				rate = 31;
			}
			else
			{
				rate = env_data & 0x1F;
				if ( mode == 4 ) // 4: linear decrease
				{
					env -= 0x20;
				}
				else if ( mode < 6 ) // 5: exponential decrease
				{
					env--;
					env -= env >> 8;
				}
				else // 6,7: linear increase
				{
					env += 0x20;
					if ( mode > 6 && (unsigned) v->hidden_env >= 0x600 )
						env += 0x8 - 0x20; // 7: two-slope linear increase
				}
			}
		}
		
		// Sustain level
		if ( (env >> 8) == (env_data >> 5) && v->env_mode == env_decay )
			v->env_mode = env_sustain;
		
		v->hidden_env = env;
		
		// unsigned cast because linear decrease going negative also triggers this
		if ( (unsigned) env > 0x7FF )
		{
			env = (env < 0 ? 0 : 0x7FF);
			if ( v->env_mode == env_attack )
				v->env_mode = env_decay;
		}
		
		if ( !read_counter( rate ) )
			v->env = env; // nothing else is controlled by the counter
	}
}


//// BRR Decoding

static inline void decode_brr( voice_t* v )
{
	// Arrange the four input nybbles in 0xABCD order for easy decoding
	int nybbles = m.t_brr_byte * 0x100 + m_ram [(v->brr_addr + v->brr_offset + 1) & 0xFFFF];
	
	int const header = m.t_brr_header;
	
	// 0: >>1  1: <<0  2: <<1 ... 12: <<11  13-15: >>4 <<11
	static unsigned char const shifts [16 * 2] = {
		13,12,12,12,12,12,12,12,12,12,12, 12, 12, 16, 16, 16,
		 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 11, 11, 11
	};
	int const scale = header >> 4;
	int const right_shift = shifts [scale];
	int const left_shift  = shifts [scale + 16];
	
	// Write to next four samples in circular buffer
	int* pos = &v->buf [v->buf_pos];
	if ( (v->buf_pos += 4) >= brr_buf_size )
		v->buf_pos = 0;
	
	// Decode four samples
	for ( int* end = pos + 4; pos < end; pos++ )
	{
		// Extract upper nybble and scale appropriately
		int s = ((int16_t) nybbles >> right_shift) << left_shift;
		nybbles <<= 4;
		
		// Apply IIR filter (8 is the most commonly used)
		int const filter = header & 0x0C;
		int const p1 = pos [brr_buf_size - 1];
		int const p2 = pos [brr_buf_size - 2] >> 1;
		if ( filter >= 8 )
		{
			s += p1;
			s -= p2;
			if ( filter == 8 ) // s += p1 * 0.953125 - p2 * 0.46875
			{
				s += p2 >> 4;
				s += (p1 * -3) >> 6;
			}
			else // s += p1 * 0.8984375 - p2 * 0.40625
			{
				s += (p1 * -13) >> 7;
				s += (p2 * 3) >> 4;
			}
		}
		else if ( filter ) // s += p1 * 0.46875
		{
			s += p1 >> 1;
			s += (-p1) >> 5;
		}
		
		// Adjust and write sample
		CLAMP16( s );
		s = (int16_t) (s * 2);
		pos [brr_buf_size] = pos [0] = s; // second copy simplifies wrap-around
	}
}


//// Misc

#define MISC_CLOCK( n ) inline static void misc_##n( void )

MISC_CLOCK( 27 )
{
	m.t_pmon = REG(pmon) & 0xFE; // voice 0 doesn't support PMON
}
MISC_CLOCK( 28 )
{
	m.t_non = REG(non);
	m.t_eon = REG(eon);
	m.t_dir = REG(dir);
}
MISC_CLOCK( 29 )
{
	if ( (m.every_other_sample ^= 1) != 0 )
		m.new_kon &= ~m.kon; // clears KON 63 clocks after it was last read
}
MISC_CLOCK( 30 )
{
	if ( m.every_other_sample )
	{
		m.kon    = m.new_kon;
		m.t_koff = REG(koff) | m.mute_mask; 
	}
	
	run_counters();
	
	// Noise
	if ( !read_counter( REG(flg) & 0x1F ) )
	{
		int feedback = (m.noise << 13) ^ (m.noise << 14);
		m.noise = (feedback & 0x4000) ^ (m.noise >> 1);
	}
}


//// Voices

#define VOICE_CLOCK( n ) static void voice_##n( voice_t* const v )

inline VOICE_CLOCK( V1 )
{
	m.t_dir_addr = m.t_dir * 0x100 + m.t_srcn * 4;
	m.t_srcn = VREG(v->regs,srcn);
}
inline VOICE_CLOCK( V2 )
{
	// Read sample pointer (ignored if not needed)
	uint8_t const* entry = &m_ram [m.t_dir_addr];
	if ( !v->kon_delay )
		entry += 2;
	m.t_brr_next_addr = entry [1] * 0x100 + entry [0];
	
	m.t_adsr0 = VREG(v->regs,adsr0);
	
	// Read pitch, spread over two clocks
	m.t_pitch = VREG(v->regs,pitchl);
}
inline VOICE_CLOCK( V3a )
{
	m.t_pitch += (VREG(v->regs,pitchh) & 0x3F) << 8;
}
inline VOICE_CLOCK( V3b )
{
	// Read BRR header and byte
	m.t_brr_byte   = m_ram [(v->brr_addr + v->brr_offset) & 0xFFFF];
	m.t_brr_header = m_ram [v->brr_addr]; // brr_addr doesn't need masking
}
VOICE_CLOCK( V3c )
{
	// Pitch modulation using previous voice's output
	if ( m.t_pmon & v->vbit )
		m.t_pitch += ((m.t_output >> 5) * m.t_pitch) >> 10;
	
	if ( v->kon_delay )
	{
		// Get ready to start BRR decoding on next sample
		if ( v->kon_delay == 5 )
		{
			v->brr_addr    = m.t_brr_next_addr;
			v->brr_offset  = 1;
			v->buf_pos     = 0;
			m.t_brr_header = 0; // header is ignored on this sample
		}
		
		// Envelope is never run during KON
		v->env        = 0;
		v->hidden_env = 0;
		
		// Disable BRR decoding until last three samples
		v->interp_pos = 0;
		if ( --v->kon_delay & 3 )
			v->interp_pos = 0x4000;
		
		// Pitch is never added during KON
		m.t_pitch = 0;
	}
	
	// Gaussian interpolation
	int output = interpolate( v );
	
	// Noise
	if ( m.t_non & v->vbit )
		output = (int16_t) (m.noise * 2);
	
	// Apply envelope
	m.t_output = (output * v->env) >> 11 & ~1;
	v->t_envx_out = v->env >> 4;
	
	// Immediate silence due to end of sample or soft reset
	if ( REG(flg) & 0x80 || (m.t_brr_header & 3) == 1 )
	{
		v->env_mode = env_release;
		v->env      = 0;
	}
	
	if ( m.every_other_sample )
	{
		// KOFF
		if ( m.t_koff & v->vbit )
			v->env_mode = env_release;
		
		// KON
		if ( m.kon & v->vbit )
		{
			v->kon_delay = 5;
			v->env_mode  = env_attack;
		}
	}
	
	// Run envelope for next sample
	if ( !v->kon_delay )
		run_envelope( v );
}
static inline void voice_output( voice_t const* v, int ch )
{
	// Apply left/right volume
	int amp = (m.t_output * (int8_t) VREG(v->regs,voll + ch)) >> 7;
	
	// Add to output total
	m.t_main_out [ch] += amp;
	CLAMP16( m.t_main_out [ch] );
	
	// Optionally add to echo total
	if ( m.t_eon & v->vbit )
	{
		m.t_echo_out [ch] += amp;
		CLAMP16( m.t_echo_out [ch] );
	}
}
VOICE_CLOCK( V4 )
{
	// Decode BRR
	m.t_looped = 0;
	if ( v->interp_pos >= 0x4000 )
	{
		decode_brr( v );
		
		if ( (v->brr_offset += 2) >= brr_block_size )
		{
			// Start decoding next BRR block
			assert( v->brr_offset == brr_block_size );
			v->brr_addr = (v->brr_addr + brr_block_size) & 0xFFFF;
			if ( m.t_brr_header & 1 )
			{
				v->brr_addr = m.t_brr_next_addr;
				m.t_looped = v->vbit;
			}
			v->brr_offset = 1;
		}
	}
	
	// Apply pitch
	v->interp_pos = (v->interp_pos & 0x3FFF) + m.t_pitch;
	
	// Keep from getting too far ahead (when using pitch modulation)
	if ( v->interp_pos > 0x7FFF )
		v->interp_pos = 0x7FFF;
	
	// Output left
	voice_output( v, 0 );
}
inline VOICE_CLOCK( V5 )
{
	// Output right
	voice_output( v, 1 );
	
	// ENDX, OUTX, and ENVX won't update if you wrote to them 1-2 clocks earlier
	
	m.endx_buf = REG(endx) | m.t_looped;
	
	// Clear bit in ENDX if KON just began
	if ( v->kon_delay == 5 )
		m.endx_buf &= ~v->vbit;
}
inline VOICE_CLOCK( V6 )
{
	m.outx_buf = m.t_output >> 8;
}
inline VOICE_CLOCK( V7 )
{
	// Update ENDX
	REG(endx) = (uint8_t) m.endx_buf;
	
	m.envx_buf = v->t_envx_out;
}
inline VOICE_CLOCK( V8 )
{
	// Update OUTX
	VREG(v->regs,outx) = (uint8_t) m.outx_buf;
}
inline VOICE_CLOCK( V9 )
{
	// Update ENVX
	VREG(v->regs,envx) = (uint8_t) m.envx_buf;
}

// Most voices do all these in one clock, so make a handy composite
inline VOICE_CLOCK( V3 )
{
	voice_V3a( v );
	voice_V3b( v );
	voice_V3c( v );
}

// Common combinations of voice steps on different voices. This greatly reduces
// code size and allows everything to be inlined in these functions.
VOICE_CLOCK(V7_V4_V1) { voice_V7(v); voice_V1(v+3); voice_V4(v+1); }
VOICE_CLOCK(V8_V5_V2) { voice_V8(v); voice_V5(v+1); voice_V2(v+2); }
VOICE_CLOCK(V9_V6_V3) { voice_V9(v); voice_V6(v+1); voice_V3(v+2); }


//// Echo

// Current echo buffer pointer for left/right channel
#define ECHO_PTR( ch )      (&m_ram [m.t_echo_ptr + ch * 2])

// Sample in echo history buffer, where 0 is the oldest
#define ECHO_FIR( i )       (m.echo_hist_pos [i])

// Calculate FIR point for left/right channel
#define CALC_FIR( i, ch )   ((ECHO_FIR( i + 1 ) [ch] * (int8_t) REG(fir + i * 0x10)) >> 6)

#define ECHO_CLOCK( n ) inline static void echo_##n( void )

static inline void echo_read( int ch )
{
	uint8_t const* in = ECHO_PTR( ch );
	int s = (int8_t) in [1] * 0x100 + in [0];
	// second copy simplifies wrap-around handling
	ECHO_FIR( 0 ) [ch] = ECHO_FIR( 8 ) [ch] = s >> 1;
}

ECHO_CLOCK( 22 )
{
	// History
	if ( ++m.echo_hist_pos >= &m.echo_hist [spc_dsp_echo_hist_size] )
		m.echo_hist_pos = m.echo_hist;
	
	m.t_echo_ptr = (m.t_esa * 0x100 + m.echo_offset) & 0xFFFF;
	echo_read( 0 );
	
	// FIR (using l and r temporaries below helps compiler optimize)
	int l = CALC_FIR( 0, 0 );
	int r = CALC_FIR( 0, 1 );
	
	m.t_echo_in [0] = l;
	m.t_echo_in [1] = r;
}
ECHO_CLOCK( 23 )
{
	int l = CALC_FIR( 1, 0 ) + CALC_FIR( 2, 0 );
	int r = CALC_FIR( 1, 1 ) + CALC_FIR( 2, 1 );
	
	m.t_echo_in [0] += l;
	m.t_echo_in [1] += r;
	
	echo_read( 1 );
}
ECHO_CLOCK( 24 )
{
	int l = CALC_FIR( 3, 0 ) + CALC_FIR( 4, 0 ) + CALC_FIR( 5, 0 );
	int r = CALC_FIR( 3, 1 ) + CALC_FIR( 4, 1 ) + CALC_FIR( 5, 1 );
	
	m.t_echo_in [0] += l;
	m.t_echo_in [1] += r;
}
ECHO_CLOCK( 25 )
{
	int l = m.t_echo_in [0] + CALC_FIR( 6, 0 );
	int r = m.t_echo_in [1] + CALC_FIR( 6, 1 );
	
	l = (int16_t) l;
	r = (int16_t) r;
	
	l += (int16_t) CALC_FIR( 7, 0 );
	r += (int16_t) CALC_FIR( 7, 1 );
	
	CLAMP16( l );
	CLAMP16( r );
	
	m.t_echo_in [0] = l & ~1;
	m.t_echo_in [1] = r & ~1;
}
static inline int echo_output( int ch )
{
	int out = (int16_t) ((m.t_main_out [ch] * (int8_t) REG(mvoll + ch * 0x10)) >> 7) +
			(int16_t) ((m.t_echo_in [ch] * (int8_t) REG(evoll + ch * 0x10)) >> 7);
	CLAMP16( out );
	return out;
}
ECHO_CLOCK( 26 )
{
	// Left output volumes
	// (save sample for next clock so we can output both together)
	m.t_main_out [0] = echo_output( 0 );
	
	// Echo feedback
	int l = m.t_echo_out [0] + (int16_t) ((m.t_echo_in [0] * (int8_t) REG(efb)) >> 7);
	int r = m.t_echo_out [1] + (int16_t) ((m.t_echo_in [1] * (int8_t) REG(efb)) >> 7);
	
	CLAMP16( l );
	CLAMP16( r );
	
	m.t_echo_out [0] = l & ~1;
	m.t_echo_out [1] = r & ~1;
}
ECHO_CLOCK( 27 )
{
	// Output
	int outl = m.t_main_out [0];
	int outr = echo_output( 1 );
	m.t_main_out [0] = 0;
	m.t_main_out [1] = 0;
	
	// TODO: global muting isn't this simple (turns DAC on and off
	// or something, causing small ~37-sample pulse when first muted)
	if ( REG(flg) & 0x40 )
	{
		outl = 0;
		outr = 0;
	}
	
	// Output sample to DAC
	#ifdef SPC_DSP_OUT_HOOK
		SPC_DSP_OUT_HOOK( outl, outr );
	#else
		spc_dsp_sample_t* out = m_spc_dsp_out;
		assert( !out || out < m_spc_dsp_out_end ); // fails if output buffer is too small
		if ( out != m_spc_dsp_out_end )
		{
			out [0] = outl;
			out [1] = outr;
			m_spc_dsp_out = out + 2;
		}
	#endif
}
ECHO_CLOCK( 28 )
{
	m.t_echo_enabled = REG(flg);
}
static inline void echo_write( int ch )
{
	if ( !(m.t_echo_enabled & 0x20) )
	{
		uint8_t* out = ECHO_PTR( ch );
		int s = m.t_echo_out [ch];
		out [0] = (uint8_t) s;
		out [1] = (uint8_t) (s >> 8);
	}
	m.t_echo_out [ch] = 0;
}
ECHO_CLOCK( 29 )
{
	m.t_esa = REG(esa);
	
	if ( !m.echo_offset )
		m.echo_length = (REG(edl) & 0x0F) * 0x800;
	
	m.echo_offset += 4;
	if ( m.echo_offset >= m.echo_length )
		m.echo_offset = 0;
	
	// Write left echo
	echo_write( 0 );
	
	m.t_echo_enabled = REG(flg);
}
ECHO_CLOCK( 30 )
{
	// Write right echo
	echo_write( 1 );
}


//// Timing

#if !SPC_DSP_CUSTOM_RUN

void spc_dsp_run( int clocks_remain )
{
	assert( clocks_remain > 0 );
	
	int const phase = m.phase;
	m.phase = (phase + clocks_remain) & 31;
	switch ( phase )
	{
	loop:
	
		#define PHASE( n ) if ( n && !--clocks_remain ) break; case n:
		#include "spc_dsp_timing.h"
		#undef PHASE
	
		if ( --clocks_remain )
			goto loop;
	}
}

#endif


//// Setup

void spc_dsp_reset( void )
{
	// Clear everything to zero, then set things which must be non-zero
	memset( m_voice_state, 0, sizeof m_voice_state );
	memset( &m_spc_dsp,    0, sizeof m_spc_dsp );
	
	m.noise              = 1;
	m.echo_hist_pos      = m.echo_hist;
	m.every_other_sample = 1;
	init_counters();
	
	int i;
	for ( i = spc_dsp_voice_count; --i >= 0; )
	{
		voice_t* v = &m_voice_state [i];
		v->regs       = &m.regs [i * 0x10];
		v->vbit       = 1 << i;
		v->brr_offset = 1;
	}
	
	REG(flg) = 0xE0;
}

void spc_dsp_soft_reset( void )
{
	// TODO: doesn't reset everything
	spc_dsp_reset();
}

void spc_dsp_init( void* ram_64k )
{
	m_ram = (uint8_t*) ram_64k;
	spc_dsp_reset();
	
	#if INT_MAX < 0x7FFFFFFF
		#error "Requires that int have at least 32 bits"
	#endif
	
	#ifndef NDEBUG
		// be sure this sign-extends
		assert( (int16_t) 0x8000 == -0x8000 );
		
		// be sure right shift preserves sign
		assert( (-1 >> 1) == -1 );
		
		// check clamp macro
		int i;
		i = +0x8000; CLAMP16( i ); assert( i == +0x7FFF );
		i = -0x8001; CLAMP16( i ); assert( i == -0x8000 );
	#endif
}

void spc_dsp_load( uint8_t const regs [spc_dsp_register_count] )
{
	int i;
	for ( i = 0; i < 0x80; i++ )
		spc_dsp_write( i, regs [i] );
	m.t_esa = regs [r_esa];
	m.t_dir = regs [r_dir];
}
