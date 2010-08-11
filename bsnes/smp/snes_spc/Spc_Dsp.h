// Highly accurate SNES SPC-700 DSP emulator

// snes_spc 0.9.5
#ifndef BLARGG_SPC_DSP_H
#define BLARGG_SPC_DSP_H

#include "blargg_common.h"

BLARGG_NAMESPACE_BEGIN

extern "C" { typedef void (*dsp_copy_func_t)( unsigned char** io, void* state, size_t ); }

struct Spc_Dsp {
public:
	typedef BOOST::uint8_t byte;
	
// Setup

	// Initializes DSP and has it use the 64K RAM provided
	void init( void* ram_64k );
	
	// Sets destination for output samples. If begin is NULL, doesn't generate any.
	typedef short sample_t;
	void set_output( sample_t* begin, sample_t* end );
	
	// Current position in output buffer, or NULL if no buffer set
	sample_t* output_ptr() const;
	
	// Sets function that is called when output buffer is filled, or NULL for none
	blargg_callback<void (*)( void* user_data )> set_output_callback;
	//void set_output_callback( void (*func)( void* user_data ), void* user_data );
	
// Emulation

	// Resets DSP to power-on state
	void reset();

	// Emulates pressing reset switch on SNES
	void soft_reset();
	
	// Reads/writes DSP registers. For accuracy, you must first call run()
	// to catch the DSP up to present.
	int  read ( int addr ) const;
	void write( int addr, int data );

	// Runs DSP for specified number of clocks (~1024000 per second). Every 32 clocks
	// a pair of samples is be generated.
	void run( int clock_count );
	
// Sound control

	// Using these reduces emulation accuracy.
	
	// Mutes voices corresponding to non-zero bits in mask (issues repeated KOFF events).
	enum { voice_count = 8 };
	void mute_voices( int mask )            { mute_mask = mask; }
	
	// If true, prevents channels and global volumes from being phase-negated
	void disable_surround( bool disable = true );
	
// State
	
	// Resets DSP and uses supplied values to initialize registers
	enum { register_count = 128 };
	void load( byte const regs [register_count] );

	// Saves/loads exact emulator state
	enum { state_size = 640 }; // maximum space needed when saving
	typedef dsp_copy_func_t copy_func_t;
	void copy_state( unsigned char** io, copy_func_t );

	// Returns non-zero if new key-on events occurred since last call
	bool check_kon();
	
// DSP register addresses

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

public:
	BLARGG_DISABLE_NOTHROW
	
	typedef BOOST::int8_t   int8_t;
	typedef BOOST::int16_t int16_t;
	
	enum { echo_hist_size = 8 };
	
	enum env_mode_t { env_release, env_attack, env_decay, env_sustain };
	enum { brr_buf_size = 12 };
	struct voice_t
	{
		int buf [brr_buf_size*2];// decoded samples (twice the size to simplify wrap handling)
		int buf_pos;            // place in buffer where next samples will be decoded
		int interp_pos;         // relative fractional position in sample (0x1000 = 1.0)
		int brr_addr;           // address of current BRR block
		int brr_offset;         // current decoding offset in BRR block
		byte* regs;             // pointer to voice's DSP registers
		int vbit;               // bitmask for voice: 0x01 for voice 0, 0x02 for voice 1, etc.
		int kon_delay;          // KON delay/current setup phase
		env_mode_t env_mode;
		int env;                // current envelope level
		int hidden_env;         // used by GAIN mode 7, very obscure quirk
		byte t_envx_out;
	};
private:
	enum { brr_block_size = 9 };
	
	// non-emulation state
	byte* ram; // 64K shared RAM between DSP and SMP
	int mute_mask;
	int surround_threshold;
	sample_t* output_begin;
	sample_t* output_ptr_;
	sample_t* output_end;
	sample_t* user_output_end;
	sample_t dummy_buf [2];
	bool kon_check;         // set when a new KON occurs
	
	struct state_t
	{
		int every_other_sample; // toggles every sample
		int kon;                // KON value when last checked
		int noise;
		int counter;
		int echo_offset;        // offset from ESA in echo buffer
		int echo_length;        // number of bytes that echo_offset will stop at
		int phase;              // next clock cycle to run (0-31)
		
		// Hidden registers also written to when main register is written to
		int  new_kon;
		byte endx_buf;
		byte envx_buf;
		byte outx_buf;
		
		// Temporary state between clocks
		
		// read once per sample
		int t_pmon;
		int t_non;
		int t_eon;
		int t_dir;
		int t_koff;
		
		// read a few clocks ahead then used
		int t_brr_next_addr;
		int t_adsr0;
		int t_brr_header;
		int t_brr_byte;
		int t_srcn;
		int t_esa;
		int t_echo_enabled;
		
		// internal state that is recalculated every sample
		int t_dir_addr;
		int t_pitch;
		int t_output;
		int t_looped;
		int t_echo_ptr;
		
		// left/right sums
		int t_main_out [2];
		int t_echo_out [2];
		int t_echo_in  [2];
		
		voice_t voices [voice_count];
		
		// Echo history keeps most recent 8 samples (twice the size to simplify wrap handling)
		int (*echo_hist_pos) [2]; // &echo_hist [0 to 7]
		int echo_hist [echo_hist_size * 2] [2];
	};
	state_t m;
	
	byte regs [register_count];
	
	void init_counter();
	void run_counters();
	unsigned read_counter( int rate );
	
	int  interpolate( voice_t const* v );
	void run_envelope( voice_t* const v );
	void decode_brr( voice_t* v );

	void misc_27();
	void misc_28();
	void misc_29();
	void misc_30();

	void voice_output( voice_t const* v, int ch );
	void voice_V1( voice_t* const );
	void voice_V2( voice_t* const );
	void voice_V3( voice_t* const );
	void voice_V3a( voice_t* const );
	void voice_V3b( voice_t* const );
	void voice_V3c( voice_t* const );
	void voice_V4( voice_t* const );
	void voice_V5( voice_t* const );
	void voice_V6( voice_t* const );
	void voice_V7( voice_t* const );
	void voice_V8( voice_t* const );
	void voice_V9( voice_t* const );
	void voice_V7_V4_V1( voice_t* const );
	void voice_V8_V5_V2( voice_t* const );
	void voice_V9_V6_V3( voice_t* const );

	void echo_read( int ch );
	int  echo_output( int ch );
	void echo_write( int ch );
	void echo_22();
	void echo_23();
	void echo_24();
	void echo_25();
	void echo_26();
	void echo_27();
	void echo_28();
	void echo_29();
	void echo_30();
	
	void set_null_output();
	void write_sample( int l, int r );
};

#include <assert.h>

inline int Spc_Dsp::read( int addr ) const
{
	assert( (unsigned) addr < register_count );
	
	return regs [addr];
}

inline void Spc_Dsp::write( int addr, int data )
{
	assert( (unsigned) addr < register_count );
	
	regs [addr] = (byte) data;
	switch ( addr & 0x0F )
	{
	case v_envx:
		m.envx_buf = (byte) data;
		break;
		
	case v_outx:
		m.outx_buf = (byte) data;
		break;
	
	case 0x0C:
		if ( addr == r_kon )
			m.new_kon = (byte) data;
		
		if ( addr == r_endx ) // always cleared, regardless of data written
		{
			m.endx_buf = 0;
			regs [r_endx] = 0;
		}
		break;
	}
}

inline void Spc_Dsp::disable_surround( bool disable )
{
	surround_threshold = disable ? 0 : -0x4000;
}

inline bool Spc_Dsp::check_kon()
{
	bool old = kon_check;
	kon_check = 0;
	return old;
}

inline Spc_Dsp::sample_t* Spc_Dsp::output_ptr() const
{
	// Don't return pointer into dummy_buf
	return (output_ptr_ != dummy_buf ? output_ptr_ : user_output_end);
}

class SPC_State_Copier {
	Spc_Dsp::copy_func_t func;
	unsigned char** buf;
public:
	SPC_State_Copier( unsigned char** p, Spc_Dsp::copy_func_t f ) { func = f; buf = p; }
	void copy( void* state, size_t size );
	int copy_int( int state, int size );
	void skip( int count );
	
	// Reads uint8_t and then skips that many bytes. If writing, writes
	// uint8_t of 0. This allows future expansion at this point, by writing
	// non-zero and additional data.
	void extra();
};

#define SPC_COPY( type, state )\
{\
	state = (BOOST::type) copier.copy_int( state, sizeof (BOOST::type) );\
	check( (BOOST::type) state == state );\
}

BLARGG_NAMESPACE_END

#endif
