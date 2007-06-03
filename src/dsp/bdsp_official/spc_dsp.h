// SNES SPC-700 DSP emulator

#ifndef SPC_DSP_H
#define SPC_DSP_H

#include <assert.h>
#include <stdint.h>

//// Setup

// Initializes DSP and has it use the 64K RAM provided
void spc_dsp_init( void* ram_64k );

// Restores DSP registers using supplied values
enum { spc_dsp_register_count = 128 };
void spc_dsp_load( uint8_t const regs [spc_dsp_register_count] );

// Mutes voice n if bit 1<<b is set
enum { spc_dsp_voice_count = 8 };
static void spc_dsp_mute_voices( int mask );

// Sets destination for output samples. If out is NULL or out_size is 0,
// doesn't generate any.
typedef short spc_dsp_sample_t;
static void spc_dsp_set_output( spc_dsp_sample_t* out, int out_size );

// Number of samples written to output since it was last set, always
// a multiple of 2
static int spc_dsp_sample_count( void );


//// Emulation

// Resets DSP to power-on state. Does not affect anything set by above functions.
void spc_dsp_reset( void );

// Emulates pressing reset switch on SNES
void spc_dsp_soft_reset( void );

// Reads/writes DSP registers. For accuracy, you must first call spc_run_dsp()
// to catch the DSP up to present.
static int spc_dsp_read( int addr );
static void spc_dsp_write( int addr, int data );

// Runs DSP for specified number of clocks (~1024000 per second). Every 32 clocks
// a pair of samples will be generated.
void spc_dsp_run( int clock_count );


//// private

enum { spc_dsp_echo_hist_size = 8 };

extern spc_dsp_sample_t* m_spc_dsp_out_begin;
extern spc_dsp_sample_t* m_spc_dsp_out;
extern spc_dsp_sample_t* m_spc_dsp_out_end;

typedef struct spc_dsp_t
{
	uint8_t regs [spc_dsp_register_count];
	
	// Echo history keeps most recent 8 samples (twice the size to simplify wrap handling)
	int echo_hist [spc_dsp_echo_hist_size * 2] [2];
	int (*echo_hist_pos) [2]; // &echo_hist [0 to 7]
	int mute_mask;
	
	int every_other_sample; // toggles every sample
	int kon;                // KON value when last checked
	int noise;
	int counter;
	int echo_offset;        // offset from ESA in echo buffer
	int echo_length;        // number of bytes that echo_offset will stop at
	int phase;              // next clock cycle to run (0-31)
	
	// Hidden registers also written to when main register is written to
	int new_kon;
	int endx_buf;
	int envx_buf;
	int outx_buf;
	
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
	
} spc_dsp_t;
extern spc_dsp_t m_spc_dsp;

static inline int spc_dsp_read( int addr )
{
	assert( (unsigned) addr < spc_dsp_register_count );
	return m_spc_dsp.regs [addr];
}

static inline void spc_dsp_write( int addr, int data )
{
	assert( (unsigned) addr < spc_dsp_register_count );
	
	m_spc_dsp.regs [addr] = data;
	switch ( addr & 0x0F )
	{
	case 0x08:
		m_spc_dsp.envx_buf = data;
		break;
		
	case 0x09:
		m_spc_dsp.outx_buf = data;
		break;
	
	case 0x0C:
		if ( addr == 0x4C ) // KON
			m_spc_dsp.new_kon = data;
		
		if ( addr == 0x7C ) // ENDX, write always clears it regardless of data
		{
			m_spc_dsp.endx_buf = 0;
			m_spc_dsp.regs [0x7C] = 0;
		}
		break;
	}
}

static inline void spc_dsp_mute_voices( int mask ) { m_spc_dsp.mute_mask = mask; }

static inline void spc_dsp_set_output( spc_dsp_sample_t* out, int out_size )
{
	assert( out_size % 2 == 0 ); // must be even
	m_spc_dsp_out_begin = out;
	m_spc_dsp_out       = out;
	if ( out )
		out += out_size;
	m_spc_dsp_out_end   = out;
}

static inline int spc_dsp_sample_count( void ) { return m_spc_dsp_out - m_spc_dsp_out_begin; }

#endif
