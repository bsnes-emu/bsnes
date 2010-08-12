// Fast SNES SPC-700 DSP emulator (about 3x speed of accurate one)

// snes_spc 0.9.5
#ifndef BLARGG_SPC_DSP_H
#define BLARGG_SPC_DSP_H

#include "blargg_common.h"

BLARGG_NAMESPACE_BEGIN

struct Spc_Dsp {
public:
	typedef BOOST::uint8_t byte;
	
// Setup
	
	// Initializes DSP and has it use the 64K RAM provided
	void init( void* ram_64k );

	// Sets function that is called when output buffer is filled, or NULL for none
	blargg_callback<void (*)( void* user_data )> set_output_callback;
	//void set_output_callback( void (*func)( void* user_data ), void* user_data );
	
	// Sets destination for output samples. If begin is NULL, doesn't generate any.
	typedef short sample_t;
	void set_output( sample_t* begin, sample_t* end );
	
	// Current position in output buffer, or NULL if no buffer set
	sample_t* output_ptr() const;
	
	// Number of samples written to output buffer since last set, or 0 if no buffer set.
	int sample_count() const;

// Emulation
	
	// Resets DSP to power-on state
	void reset();

	// Emulates pressing reset switch on SNES
	void soft_reset();
	
	// Reads/writes DSP registers. For accuracy, you must first call spc_run_dsp()
	// to catch the DSP up to present.
	int  read ( int addr ) const;
	void write( int addr, int data );

	// Runs DSP for specified number of clocks (~1024000 per second). Every 32 clocks
	// a pair of samples is generated.
	void run( int clock_count );

// Sound control

	// Mutes voices corresponding to non-zero bits in mask (overrides VxVOL with 0).
	// Reduces emulation accuracy.
	enum { voice_count = 8 };
	void mute_voices( int mask )        { mute_mask = mask; }
	
	// If true, prevents channels and global volumes from being phase-negated
	void disable_surround( bool disable = true );

// State
	
	// Resets DSP and uses supplied values to initialize registers
	enum { register_count = 128 };
	void load( byte const regs [register_count] );

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
		int* buf_pos;           // place in buffer where next samples will be decoded
		int interp_pos;         // relative fractional position in sample (0x1000 = 1.0)
		int brr_addr;           // address of current BRR block
		int brr_offset;         // current decoding offset in BRR block
		int kon_delay;          // KON delay/current setup phase
		env_mode_t env_mode;
		int env;                // current envelope level
		int hidden_env;         // used by GAIN mode 7, very obscure quirk
		int volume [2];         // copy of volume from DSP registers, with surround disabled
	};
private:
	// non-emulation state
	byte* ram; // 64K shared RAM between DSP and SMP
	int mute_mask;
	int surround_threshold;
	sample_t* output_begin;
	sample_t* output_ptr_;
	sample_t* output_end;
	sample_t* user_output_end;
	sample_t dummy_buf [2];
	
	struct state_t
	{
		int every_other_sample; // toggles every sample
		int kon;                // KON value when last checked
		int noise;
		int echo_offset;        // offset from ESA in echo buffer
		int echo_length;        // number of bytes that echo_offset will stop at
		int phase;              // next clock cycle to run (0-31)
		unsigned counters [4];
		
		int new_kon;
		int t_koff;
		
		// Echo history keeps most recent 8 samples (twice the size to simplify wrap handling)
		int (*echo_hist_pos) [2]; // &echo_hist [0 to 7]
		int echo_hist [echo_hist_size * 2] [2];
		
		unsigned* counter_select [32];
		voice_t voices [voice_count];
	};
	state_t m;
	
	byte regs [register_count];
	
	void init_counter();
	void run_counter( int );
	void update_voice_vol( int addr );
	void set_null_output();
	void write_sample( int l, int r );
	void apply_output_enables();
};

inline int Spc_Dsp::read( int addr ) const
{
	assert( (unsigned) addr < register_count );
	return regs [addr];
}

inline void Spc_Dsp::update_voice_vol( int addr )
{
	int l = (int8_t) regs [addr + v_voll];
	int r = (int8_t) regs [addr + v_volr];
	
	if ( l * r < surround_threshold )
	{
		// signs differ, so negate those that are negative
		l ^= l >> 7;
		r ^= r >> 7;
	}
	
	int index = addr >> 4;
	voice_t& v = m.voices [index];
	int enabled = ~mute_mask >> index & 1;
	v.volume [0] = l * enabled;
	v.volume [1] = r * enabled;
}

inline void Spc_Dsp::write( int addr, int data )
{
	assert( (unsigned) addr < register_count );
	
	regs [addr] = (byte) data;
	int low = addr & 0x0F;
	if ( low < 0x2 ) // voice volumes
	{
		update_voice_vol( low ^ addr /* addr & 0xF0 */ );
	}
	else if ( low == 0xC )
	{
		if ( addr == r_kon )
			m.new_kon = (byte) data;
		
		if ( addr == r_endx ) // always cleared, regardless of data written
			regs [r_endx] = 0;
	}
}

inline void Spc_Dsp::disable_surround( bool disable )
{
	surround_threshold = disable ? 0 : -0x4000;
}

inline Spc_Dsp::sample_t* Spc_Dsp::output_ptr() const
{
	// Don't return pointer into dummy_buf
	return (output_ptr_ != dummy_buf ? output_ptr_ : user_output_end);
}

inline int Spc_Dsp::sample_count() const
{
	sample_t* p = output_ptr();
	return (p ? p - output_begin : 0);
}

#define SPC_NO_COPY_STATE_FUNCS 1

#define SPC_LESS_ACCURATE 1

BLARGG_NAMESPACE_END

#endif
