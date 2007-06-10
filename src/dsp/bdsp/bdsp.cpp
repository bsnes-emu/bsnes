/* This code is heavily customized for bsnes and requires cothreads.
Original portable snes_spc library available at http://www.slack.net/~ant/
Copyright (C) 2007 Shay Green. See license.txt. */

#include "../../base.h"

int const brr_block_size = 9;

// Accesses global DSP register
#define REG(n)      m.regs [r_##n]

// Accesses voice DSP register
#define VREG(r,n)   r [v_##n]

// Volume registers and efb are signed! Easy to forget int8 cast.
// Prefixes are to avoid accidental use of locals with same names.

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


//// Counters

int const simple_counter_range = 2048 * 5 * 3; // 30720

static unsigned const counter_rates [32] =
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

static unsigned const counter_offsets [32] =
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

inline unsigned bDSP::read_counter( int rate )
{
	return ((unsigned) m.counter + counter_offsets [rate]) % counter_rates [rate];
}


//// Envelope

inline void bDSP::run_envelope( voice_t* const v )
{
	int env = v->env;
	if ( v->env_mode == env_release )
	{
		if ( (env -= 0x8) < 0 )
			env = 0;
		v->env = env;
	}
	else
	{
		int rate;
		int env_data = VREG(v->regs,adsr1);
		if ( m.t_adsr0 & 0x80 ) // ADSR
		{
			if ( v->env_mode >= env_decay )
			{
				env--;
				env -= asr<8>( env );
				rate = env_data & 0x1F;
				if ( v->env_mode == env_decay )
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
					env -= asr<8>( env );
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

inline void bDSP::decode_brr( voice_t* v )
{
	// Arrange the four input nybbles in 0xABCD order for easy decoding
	int nybbles = m.t_brr_byte * 0x100 + ram [(v->brr_addr + v->brr_offset + 1) & 0xFFFF];
	
	// Write to next four samples in circular buffer
	int* pos = &v->buf [v->buf_pos];
	if ( (v->buf_pos += 4) >= brr_buf_size )
		v->buf_pos = 0;
	
	// Decode four samples
	for ( int* end = pos + 4; pos < end; pos++ )
	{
		// Extract nybble and sign-extend
		int s = asr<12>( sclip<16>( nybbles ) );
		nybbles <<= 4;
		
		// Shift sample based on header
		int const shift = m.t_brr_header >> 4;
		s = asr<1>( s << shift );
		if ( shift >= 0xD ) // handle invalid range
			s = (s < 0 ? -0x800 : 0);
		
		// Apply IIR filter (8 is the most commonly used)
		int const p1 = pos [brr_buf_size - 1];
		int const p2 = asr<1>( pos [brr_buf_size - 2] );
		switch ( m.t_brr_header >> 2 & 3 )
		{
		case 1: s += asr<1>( p1 ) + asr<5>( -p1 ); break;                    // s += p1 * 0.4687500
		case 2: s += p1 + asr<6>( p1 *  -3 ) - p2 + asr<4>( p2     ); break; // s += p1 * 0.9531250 - p2 * 0.46875
		case 3: s += p1 + asr<7>( p1 * -13 ) - p2 + asr<4>( p2 * 3 ); break; // s += p1 * 0.8984375 - p2 * 0.40625
		}
		
		// Adjust and write sample
		s = sclip<16>( sclamp<16>( s ) * 2 );
		pos [brr_buf_size] = pos [0] = s; // second copy simplifies wrap-around
	}
}


//// Voices

#define VOICE_CLOCK( n ) void bDSP::voice_##n( voice_t* const v )

inline VOICE_CLOCK( V1 )
{
	m.t_dir_addr = m.t_dir * 0x100 + m.t_srcn * 4;
	m.t_srcn = VREG(v->regs,srcn);
}
inline VOICE_CLOCK( V2 )
{
	// Read sample pointer (ignored if not needed)
	uint8 const* entry = &ram [m.t_dir_addr];
	if ( !v->kon_delay )
		entry += 2;
	m.t_brr_next_addr = entry [0] | entry [1] << 8;
	
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
	m.t_brr_byte   = ram [(v->brr_addr + v->brr_offset) & 0xFFFF];
	m.t_brr_header = ram [v->brr_addr]; // brr_addr doesn't need masking
}
VOICE_CLOCK( V3c )
{
	// Pitch modulation using previous voice's output
	if ( m.t_pmon & v->vbit )
		m.t_pitch += asr<10>( asr<5>( m.t_output ) * m.t_pitch );
	
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
	{
		// Make pointers into gaussian based on fractional position between samples
		int offset = v->interp_pos >> 4 & 0xFF;
		short const* fwd = gauss + 255 - offset;
		short const* rev = gauss       + offset; // mirror left half of gaussian
		
		int const* in = &v->buf [(v->interp_pos >> 12) + v->buf_pos];
		int out;
		out  = asr<11>( fwd [  0] * in [0] );
		out += asr<11>( fwd [256] * in [1] );
		out += asr<11>( rev [256] * in [2] );
		out = sclip<16>( out );
		out += asr<11>( rev [  0] * in [3] );
		
		out = sclamp<16>( out ) & ~1;
		
		// Noise
		if ( m.t_non & v->vbit )
			out = sclip<16>( m.noise * 2 );
		
		// Apply envelope
		m.t_output = asr<11>( out * v->env ) & ~1;
		v->t_envx_out = (uint8) (v->env >> 4);
	}
	
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
inline void bDSP::voice_output( voice_t const* v, int ch )
{
	// Apply left/right volume
	int amp = asr<7>( m.t_output * (int8) VREG(v->regs,voll + ch) );
	
	// Add to output total
	m.t_main_out [ch] = sclamp<16>( m.t_main_out [ch] + amp );
	
	// Optionally add to echo total
	if ( m.t_eon & v->vbit )
		m.t_echo_out [ch] = sclamp<16>( m.t_echo_out [ch] + amp );
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
	int endx_buf = REG(endx) | m.t_looped;
	
	// Clear bit in ENDX if KON just began
	if ( v->kon_delay == 5 )
		endx_buf &= ~v->vbit;
	m.endx_buf = (uint8) endx_buf;
}
inline VOICE_CLOCK( V6 )
{
	(void) v; // avoid compiler warning about unused v
	m.outx_buf = (uint8) (m.t_output >> 8);
}
inline VOICE_CLOCK( V7 )
{
	// Update ENDX
	REG(endx) = m.endx_buf;
	
	m.envx_buf = v->t_envx_out;
}
inline VOICE_CLOCK( V8 )
{
	// Update OUTX
	VREG(v->regs,outx) = m.outx_buf;
}
inline VOICE_CLOCK( V9 )
{
	// Update ENVX
	VREG(v->regs,envx) = m.envx_buf;
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
#define ECHO_PTR( ch )      (&ram [echo_ptr + ch * 2])

// Sample in echo history buffer, where 0 is the oldest
#define ECHO_FIR( i )       (m.echo_hist [(m.echo_hist_pos + (i)) & (echo_hist_size - 1)])

// Calculate FIR point for left/right channel
#define CALC_FIR( i, ch )   asr<6>( ECHO_FIR( i + 1 ) [ch] * (int8) REG(fir + i * 0x10) )

inline int get_echo_sample( void const* p )
{
	return ((uint8 const*) p) [0] |
	       (( int8 const*) p) [1] << 8;
}

inline void set_echo_sample( void* p, unsigned n )
{
	((uint8*) p) [0] = (uint8) n;
	((uint8*) p) [1] = (uint8) (n >> 8);
}

inline int bDSP::calc_echo_output( int ch, int echo_in )
{
	return sclamp<16>(
			sclip<16>( asr<7>( m.t_main_out [ch] * (int8) REG(mvoll + ch * 0x10) ) ) +
			sclip<16>( asr<7>( echo_in           * (int8) REG(evoll + ch * 0x10) ) ) );
}


//// Timing

void bDSP::enter()
{
	int t_esa = REG(esa);
	
	while ( 1 )
	{
		// n is currently ignored
		#define NEXT_CLOCK( n ) \
			scheduler.addclocks_dsp( 3 );
		
		// Execute clock for a particular voice
		#define V( clock, voice )   voice_##clock( &m.voices [voice] );

		/* The most common sequence of clocks uses composite operations
		for efficiency. For example, the following are equivalent to the
		individual steps on the right:

		V(V7_V4_V1,2) -> V(V7,2) V(V4,3) V(V1,5)
		V(V8_V5_V2,2) -> V(V8,2) V(V5,3) V(V2,4)
		V(V9_V6_V3,2) -> V(V9,2) V(V6,3) V(V3,4) */
		
		NEXT_CLOCK( 0)  V(V5,0)V(V2,1)
		NEXT_CLOCK( 1)  V(V6,0)V(V3,1)
		NEXT_CLOCK( 2)  V(V7_V4_V1,0)
		NEXT_CLOCK( 3)  V(V8_V5_V2,0)
		NEXT_CLOCK( 4)  V(V9_V6_V3,0)
		NEXT_CLOCK( 5)         V(V7_V4_V1,1)
		NEXT_CLOCK( 6)         V(V8_V5_V2,1)
		NEXT_CLOCK( 7)         V(V9_V6_V3,1)
		NEXT_CLOCK( 8)                V(V7_V4_V1,2)
		NEXT_CLOCK( 9)                V(V8_V5_V2,2)
		NEXT_CLOCK(10)                V(V9_V6_V3,2)
		NEXT_CLOCK(11)                       V(V7_V4_V1,3)
		NEXT_CLOCK(12)                       V(V8_V5_V2,3)
		NEXT_CLOCK(13)                       V(V9_V6_V3,3)
		NEXT_CLOCK(14)                              V(V7_V4_V1,4)
		NEXT_CLOCK(15)                              V(V8_V5_V2,4)
		NEXT_CLOCK(16)                              V(V9_V6_V3,4)
		NEXT_CLOCK(17)  V(V1,0)                            V(V7,5)V(V4,6)
		NEXT_CLOCK(18)                                     V(V8_V5_V2,5)
		NEXT_CLOCK(19)                                     V(V9_V6_V3,5)
		NEXT_CLOCK(20)         V(V1,1)                            V(V7,6)V(V4,7)
		NEXT_CLOCK(21)                                            V(V8,6)V(V5,7)  V(V2,0)  /* t_brr_next_addr order dependency */
		NEXT_CLOCK(22)  V(V3a,0)                                  V(V9,6)V(V6,7)
			
			// History
			if ( ++m.echo_hist_pos >= echo_hist_size )
				m.echo_hist_pos = 0;
			
			int const echo_ptr = (t_esa * 0x100 + m.echo_offset) & 0xFFFF;
			
			// FIR
			int echo_in_l = CALC_FIR( 0, 0 );
			int echo_in_r = CALC_FIR( 0, 1 );
		
			ECHO_FIR( 0 ) [0] = asr<1>( get_echo_sample( ECHO_PTR( 0 ) ) );
		
		NEXT_CLOCK(23) V(V7,7)
			
			echo_in_l += CALC_FIR( 1, 0 ) + CALC_FIR( 2, 0 );
			echo_in_r += CALC_FIR( 1, 1 ) + CALC_FIR( 2, 1 );
			
			ECHO_FIR( 0 ) [1] = asr<1>( get_echo_sample( ECHO_PTR( 1 ) ) );
			
		NEXT_CLOCK(24) V(V8,7)
		
			echo_in_l += CALC_FIR( 3, 0 ) + CALC_FIR( 4, 0 ) + CALC_FIR( 5, 0 );
			echo_in_r += CALC_FIR( 3, 1 ) + CALC_FIR( 4, 1 ) + CALC_FIR( 5, 1 );
		
		NEXT_CLOCK(25) V(V3b,0) V(V9,7)
		
			echo_in_l = sclip<16>( echo_in_l + CALC_FIR( 6, 0 ) ) + sclip<16>( CALC_FIR( 7, 0 ) );
			echo_in_r = sclip<16>( echo_in_r + CALC_FIR( 6, 1 ) ) + sclip<16>( CALC_FIR( 7, 1 ) );
			
			echo_in_l = sclamp<16>( echo_in_l ) & ~1;
			echo_in_r = sclamp<16>( echo_in_r ) & ~1;
			
		NEXT_CLOCK(26)

			// Echo feedback
			int echo_out_l = m.t_echo_out [0] + sclip<16>( asr<7>( echo_in_l * (int8) REG(efb) ) );
			int echo_out_r = m.t_echo_out [1] + sclip<16>( asr<7>( echo_in_r * (int8) REG(efb) ) );
			
			echo_out_l = sclamp<16>( echo_out_l ) & ~1;
			echo_out_r = sclamp<16>( echo_out_r ) & ~1;
		
			// Output
			int main_out_l = calc_echo_output( 0, echo_in_l );
		
		NEXT_CLOCK(27)
		
			int main_out_r = calc_echo_output( 1, echo_in_r );
			
			// TODO: global muting isn't this simple (turns DAC on and off
			// or something, causing small ~37-sample pulse when first muted)
			if ( REG(flg) & 0x40 )
			{
				main_out_l = 0;
				main_out_r = 0;
			}
			
			// Output sample to DAC
			snes.audio_update( main_out_l, main_out_r );
			
			m.t_main_out [0] = 0;
			m.t_main_out [1] = 0;
			
			m.t_pmon = REG(pmon) & 0xFE; // voice 0 doesn't support PMON
		
		NEXT_CLOCK(28)
		
			m.t_non = REG(non);
			m.t_eon = REG(eon);
			m.t_dir = REG(dir);
			
			int echo_disabled = REG(flg);
			
		NEXT_CLOCK(29)
		
			// Write left echo
			if ( !(echo_disabled & 0x20) )
				set_echo_sample( ECHO_PTR( 0 ), echo_out_l );
			m.t_echo_out [0] = 0;
			
			t_esa = REG(esa);
			
			if ( !m.echo_offset )
				m.echo_length = (REG(edl) & 0x0F) * 0x800;
			
			m.echo_offset += 4;
			if ( m.echo_offset >= m.echo_length )
				m.echo_offset = 0;
			
			if ( (m.every_other_sample ^= 1) != 0 )
				m.new_kon &= ~m.kon; // clears KON 63 clocks after it was last read
		
			echo_disabled = REG(flg);
			
		NEXT_CLOCK(30)
			
			// Write right echo
			if ( !(echo_disabled & 0x20) )
				set_echo_sample( ECHO_PTR( 1 ), echo_out_r );
			m.t_echo_out [1] = 0;
			
			if ( m.every_other_sample )
			{
				m.kon    = m.new_kon;
				m.t_koff = REG(koff); 
			}
			
			if ( --m.counter < 0 )
				m.counter = simple_counter_range - 1;
			
			// Noise
			if ( !read_counter( REG(flg) & 0x1F ) )
			{
				int feedback = (m.noise << 13) ^ (m.noise << 14);
				m.noise = (feedback & 0x4000) ^ (m.noise >> 1);
			}
			
			V(V3c,0)
		
		NEXT_CLOCK(31) V(V4,0) V(V1,2)
	}
}


//// Setup

bDSP::bDSP() { }

bDSP::~bDSP() { }

void bDSP::reset()
{
	REG(flg) = 0xE0;
	
	m.noise              = 0x4000;
	m.echo_hist_pos      = 0;
	m.every_other_sample = 1;
	m.echo_offset        = 0;
	m.counter            = 0;
}

static uint8 const initial_regs [bDSP::register_count] =
{
	0x45,0x8B,0x5A,0x9A,0xE4,0x82,0x1B,0x78,0x00,0x00,0xAA,0x96,0x89,0x0E,0xE0,0x80,
	0x2A,0x49,0x3D,0xBA,0x14,0xA0,0xAC,0xC5,0x00,0x00,0x51,0xBB,0x9C,0x4E,0x7B,0xFF,
	0xF4,0xFD,0x57,0x32,0x37,0xD9,0x42,0x22,0x00,0x00,0x5B,0x3C,0x9F,0x1B,0x87,0x9A,
	0x6F,0x27,0xAF,0x7B,0xE5,0x68,0x0A,0xD9,0x00,0x00,0x9A,0xC5,0x9C,0x4E,0x7B,0xFF,
	0xEA,0x21,0x78,0x4F,0xDD,0xED,0x24,0x14,0x00,0x00,0x77,0xB1,0xD1,0x36,0xC1,0x67,
	0x52,0x57,0x46,0x3D,0x59,0xF4,0x87,0xA4,0x00,0x00,0x7E,0x44,0x9C,0x4E,0x7B,0xFF,
	0x75,0xF5,0x06,0x97,0x10,0xC3,0x24,0xBB,0x00,0x00,0x7B,0x7A,0xE0,0x60,0x12,0x0F,
	0xF7,0x74,0x1C,0xE5,0x39,0x3D,0x73,0xC1,0x00,0x00,0x7A,0xB3,0xFF,0x4E,0x7B,0xFF
};

void bDSP::power()
{
	ram = (uint8*) r_smp->get_spcram_handle();
	memset( &m, 0, sizeof m );
	//memcpy( m.regs, initial_regs, sizeof m.regs );
	memset(m.regs, 0, sizeof m.regs);
	REG(flg) = 0xe0;
	
	// Internal state
	for ( int i = voice_count; --i >= 0; )
	{
		voice_t* v = &m.voices [i];
		v->brr_offset = 1;
		v->vbit       = 1 << i;
		v->regs       = &m.regs [i * 0x10];
	}
	m.new_kon = REG(kon);
	m.t_dir   = REG(dir);
	
	reset();
}
