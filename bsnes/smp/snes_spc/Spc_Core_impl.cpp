// snes_spc 0.9.5. http://www.slack.net/~ant/

#include "Spc_Core.h"

//#include "spc_cpu_log.h"

/* Copyright (C) 2004-2010 Shay Green. This module is free software; you
can redistribute it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version. This
module is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details. You should have received a copy of the GNU Lesser General Public
License along with this module; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA */

#include "blargg_source.h"

// Note: SPC_MORE_ACCURACY exists mainly so I can run my validation tests, which
// do crazy echo buffer accesses.
#ifndef SPC_MORE_ACCURACY
	#define SPC_MORE_ACCURACY 0
#endif


//// Timers

inline void Spc_Core::adjust_timers( int offset )
{
	timers [0].time += offset;
	timers [1].time += offset;
	timers [2].time += offset;
}

inline void Spc_Core::run_timer( rel_time_t time, int i )
{
	if ( time >= timers [i].time )
		run_timer_( time, i );
}

inline void Spc_Core::enable_timers( rel_time_t time, int new_control )
{
	for ( int i = 0; i < timer_count; i++ )
	{
		int const bit = 1 << i;
		
		int changed = new_control ^ regs [r_control];
		if ( changed & bit )
		{
			run_timer( time, i );
			
			if ( new_control & bit )
			{
				// Timer just enabled
				regs_in [r_t0out + i] = 0;
				timers [i].divider    = 0;
			}
		}
	}
}

inline int Spc_Core::read_timer( rel_time_t time, int i )
{
	run_timer( time, i );
	
	int result = regs_in [r_t0out + i];
	regs_in [r_t0out + i] = 0;
	return result;
}



//// DSP

#if SPC_LESS_ACCURATE
	int const max_reg_time = 29;
	
	/* Fast DSP only runs every 32nd clock. By adjusting the end time based
	on which register is being accessed, in most cases the register access
	is emulated at the precise time. */
	static signed char const reg_times [256] =
	{
		 -1,  0,-11,-10,-15,-11, -2, -2,  4,  3, 14, 14, 26, 26, 14, 22,
		  2,  3,  0,  1,-12,  0,  1,  1,  7,  6, 14, 14, 27, 14, 14, 23,
		  5,  6,  3,  4, -1,  3,  4,  4, 10,  9, 14, 14, 26, -5, 14, 23,
		  8,  9,  6,  7,  2,  6,  7,  7, 13, 12, 14, 14, 27, -4, 14, 24,
		 11, 12,  9, 10,  5,  9, 10, 10, 16, 15, 14, 14, -2, -4, 14, 24,
		 14, 15, 12, 13,  8, 12, 13, 13, 19, 18, 14, 14, -2,-36, 14, 24,
		 17, 18, 15, 16, 11, 15, 16, 16, 22, 21, 14, 14, 28, -3, 14, 25,
		 20, 21, 18, 19, 14, 18, 19, 19, 25, 24, 14, 14, 14, 29, 14, 25,
		 
		 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
		 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
		 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
		 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
		 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
		 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
		 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
		 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29,
	};
	
	// not surrounded in block so else can be added
	#define RUN_DSP( time, offset ) \
		int count = (time) - (offset) - dsp_time;\
		if ( count >= 0 )\
		{\
			int clock_count = (count & ~(clocks_per_sample - 1)) + clocks_per_sample;\
			dsp_time += clock_count;\
			dsp_.run( clock_count );\
		}
#else
	#define RUN_DSP( time, ignored ) \
		{\
			int count = (time) - dsp_time;\
			if ( !SPC_MORE_ACCURACY || count )\
			{\
				assert( count > 0 );\
				dsp_time = (time);\
				dsp_.run( count );\
			}\
		}
#endif

int Spc_Core::dsp_read( rel_time_t time )
{
	RUN_DSP( time, reg_times [regs [r_dspaddr] & 0x7F] );
	
	return dsp_.read( regs [r_dspaddr] & 0x7F );
}

inline void Spc_Core::dsp_write( rel_time_t time, int data )
{
	RUN_DSP( time, reg_times [regs [r_dspaddr]] )
	#if SPC_LESS_ACCURATE
		else if ( dsp_time == skipping_time )
		{
			int r = regs [r_dspaddr];
			if ( r == Spc_Dsp::r_kon )
				skipped_kon |= data & ~dsp_.read( Spc_Dsp::r_koff );
			
			if ( r == Spc_Dsp::r_koff )
			{
				skipped_koff |=  data;
				skipped_kon  &= ~data;
			}
		}
	#endif
	
	if ( regs [r_dspaddr] <= 0x7F )
		dsp_.write( regs [r_dspaddr], data );
	else if ( !SPC_MORE_ACCURACY )
		dprintf( "SPC wrote to DSP register > $7F\n" );
}


//// Memory access

#if SPC_MORE_ACCURACY
	#define MEM_ACCESS( time, addr ) \
		if ( time >= dsp_time )\
			{ RUN_DSP( time, max_reg_time ); }\

#elif !defined (NDEBUG)
	// Debug-only check for read/write within echo buffer, since this might result in
	// inaccurate emulation due to the DSP not being caught up to the present.
	#define MEM_ACCESS( time, addr ) check( !check_echo_access( (BOOST::uint16_t) addr ) );

#else
	#define MEM_ACCESS( time, addr )

#endif


// Read/write handlers are divided into multiple functions to keep rarely-used
// functionality separate so often-used functionality can be optimized better
// by compiler.

// If write isn't preceded by read, data has this added to it
int const no_read_before_write = 0x2000;

void Spc_Core::cpu_write_smp_reg_( rel_time_t time, int data, int reg )
{
	// Undo write to RAM, since most writes shouldn't affect value read back
	ram() [regs_addr + reg] = regs_in [reg];
	
	switch ( reg )
	{
	case r_test:
		if ( (byte) data != 0x0A )
			dprintf( "SPC wrote to TEST register\n" );
		break;
	
	case r_control:
		if ( data & 0x10 )
		{
			write_apuio( 0, 0 );
			write_apuio( 1, 0 );
		}
		
		if ( data & 0x20 )
		{
			write_apuio( 2, 0 );
			write_apuio( 3, 0 );
		}
		
		enable_timers( time, data );
		
		enable_rom( data & 0x80 );
		break;
	
	// Registers that act like RAM
	case r_dspaddr:
	case 0x8:
	case 0x9:
		ram()   [regs_addr + reg] = data;
		regs_in [            reg] = data;
		break;
	
	case r_t0target:
	case r_t1target:
	case r_t2target:
		run_timer( time, reg - r_t0target );
		break;
	
	case r_t0out:
	case r_t1out:
	case r_t2out:
		if ( !SPC_MORE_ACCURACY )
			dprintf( "SPC wrote to counter %d\n", (int) reg - r_t0out );
		
		if ( data < no_read_before_write / 2 )
		{
			// This was a write which also reads one clock before. That read
			// causes the counter to be cleared.
			read_timer( time - 1, reg - r_t0out );
		}
		break;
	}
	
	regs [reg] = data;
}

void Spc_Core::cpu_write_high( rel_time_t time, int data, int i )
{
	// i = addr - rom_addr
	
	// $FFC0-$FFFF
	if ( i < rom_size )
	{
		hi_ram [i] = (byte) data;
		if ( rom_enabled )
			ram() [i + rom_addr] = rom_ [i]; // restore overwritten ROM
	}
	// Wrapped around to beginning of RAM
	else
	{
		assert( ram() [i + rom_addr] == (byte) data );
		ram() [i + rom_addr] = padding_fill; // restore overwritten padding
		cpu_write( time, data, i + rom_addr - 0x10000 );
	}
}

inline void Spc_Core::cpu_write_smp_reg( rel_time_t time, int data, int reg )
{
	if ( reg == r_dspaddr ) // 64%
		regs [r_dspaddr] = data;
	
	else if ( reg == r_dspdata ) // 35%
	{
		ram() [regs_addr + r_dspdata] = 0xFF; // needed for copy_state test to pass
		dsp_write( time, data );
	}
	
	else // 1%
		cpu_write_smp_reg_( time, data, reg );
}

void Spc_Core::cpu_write( rel_time_t time, int data, int addr )
{
	// CPU can generate addresses beyond $FFFF, which need to wrap around
	
	MEM_ACCESS( time, addr )
	
	// RAM
	ram() [addr] = (byte) data;
	int reg = addr - regs_addr;
	if ( reg >= 0 ) // 64%
	{
		// $F0+
		
		if ( reg < reg_count ) // 87%
		{
			// $F0-$FF
			cpu_write_smp_reg( time, data, reg );
			return;
		}
		
		reg -= rom_addr - regs_addr;
		if ( reg >= 0 ) // 1%
		{
			// $FFC0+
			cpu_write_high( time, data, reg );
		}
	}
}


//// CPU read

inline int Spc_Core::cpu_read_smp_reg( rel_time_t time, int reg )
{
	int result = regs_in [reg];
	
	reg -= r_dspaddr;
	if ( (unsigned) reg <= 1 ) // 4%
	{
		// dspaddr or dspdata
		result = regs [r_dspaddr];
		
		if ( (unsigned) reg == 1 ) // dspdata
			result = dsp_read( time );
	}
	return result;
}

int Spc_Core::cpu_read( rel_time_t time, int addr )
{
	// CPU can generate addresses beyond $FFFF, which need to wrap around
	
	MEM_ACCESS( time, addr )
	
	// RAM
	int result = ram() [addr];
	int reg = addr - regs_addr;
	if ( reg >= 0 ) // 40%
	{
		// $F0+
		
		reg -= 0x10;
		if ( (unsigned) reg >= 0xFF00 ) // 21%
		{
			// $F0-$FF or $10000+
			
			reg += 0x10 - r_t0out;
			
			// Timers
			if ( (unsigned) reg < timer_count ) // 90%
			{
				// $FD-$FF
				result = read_timer( time, reg );
			}
			// Other registers
			else if ( reg < 0 ) // 10%
			{
				// $F0-$FC
				result = cpu_read_smp_reg( time, reg + r_t0out );
			}
			else // 1%
			{
				// $10000+
				assert( reg + (r_t0out + regs_addr - 0x10000) < 0x100 );
				result = cpu_read( time, reg + (r_t0out + regs_addr - 0x10000) );
			}
		}
	}
	
	return result;
}

#define READ(  delay, addr )        cpu_read ( TIME( delay ), addr )
#define WRITE( delay, addr, data )  cpu_write( TIME( delay ), data, addr )

#if SPC_MORE_ACCURACY
	#define READ_TIMER( delay, addr, out )\
		{ out = READ( delay, addr ); }

	#define WRITE_DP_FAST( offset, data ) \
		WRITE_DP( 0, offset, data )
#else
	// timers are by far the most common thing read from dp
	#define READ_TIMER( delay, addr_, out )\
	{\
		rel_time_t time = TIME( delay );\
		int dp_addr = addr_;\
		int ti = dp_addr - (r_t0out + regs_addr);\
		if ( (unsigned) ti < timer_count )\
		{\
			out = read_timer( time, ti );\
		}\
		else\
		{\
			out = ram [dp_addr];\
			int i = dp_addr - regs_addr;\
			if ( (unsigned) i < 0x10 )\
				out = cpu_read_smp_reg( time, i );\
		}\
	}
	
	#define WRITE_DP_FAST( offset, data ) \
	{\
		int i = dp + offset;\
		ram [i] = (byte) data;\
		i -= regs_addr;\
		if ( (unsigned) i < 0x10 )\
			cpu_write_smp_reg( TIME( 0 ), data, i );\
	}
#endif


//// Run

void Spc_Core::end_frame( time_t end_time )
{
	// Catch CPU up to as close to end as possible. If final instruction
	// would exceed end, does NOT execute it and leaves spc_time < end.
	if ( end_time > spc_time )
		run_until_( end_time );
	
	spc_time -= end_time;
	
	// Should have run at least to end_time, and at most 11 clocks over
	// (for DIV YA,X)
	assert( 0 <= spc_time && spc_time < 12 );
	
	// Catch timers and DSP up to end_time
	
	for ( int i = 0; i < timer_count; i++ )
		run_timer( 0, i );
	
	// This may still leave dsp_time < 0, in the case of the fast DSP, but
	// that's fine
	if ( dsp_time < 0 )
		{ RUN_DSP( 0, max_reg_time ); }
}

// Macro to put prefix into same file as body of function, so that debugger
// can properly step through
#define SPC_CPU_RUN_FUNC \
void Spc_Core::run_until_( time_t end_time )\
{\
	rel_time_t rel_time = spc_time - end_time;\
	check( rel_time <= 0 );\
	\
	spc_time -= rel_time;\
	dsp_time += rel_time;\
	adjust_timers( rel_time );

	#include "Spc_Cpu_run.h"
	
	spc_time += rel_time;
	dsp_time -= rel_time;
	adjust_timers( -rel_time );
	check( spc_time >= end_time );
}
