// snes_spc 0.9.5. http://www.slack.net/~ant/

#include "Spc_Core.h"

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

void Spc_Core::set_tempo( int t )
{
	#if !SPC_DISABLE_TEMPO
		tempo = t;
		
		if ( !t )
			t = 1;
		
		int const timer2_rate  = 1 << timer2_shift;
		int rate = (timer2_rate * tempo_unit + (t >> 1)) / t;
		if ( rate < timer2_rate / 4 )
			rate = timer2_rate / 4; // max 4x tempo
		
		prescaler_base = rate;
	#endif
}

blargg_err_t Spc_Core::init()
{
	BLARGG_CLEAR( &regs );
	BLARGG_CLEAR( &regs_in );
	
	// Most SPC music doesn't need ROM, and almost all the rest only rely
	// on these two bytes
	BLARGG_CLEAR( &rom_ );
	rom_ [0x3E] = 0xFF;
	rom_ [0x3F] = 0xC0;
	
	set_tempo( tempo_unit );
	dsp_.init( ram() );
	reset();
	
	return blargg_ok;
}

void Spc_Core::init_rom( byte const in [rom_size] )
{
	memcpy( rom_, in, sizeof rom_ );
}

void Spc_Core::enable_rom( bool enable )
{
	if ( rom_enabled != enable )
	{
		rom_enabled = enable;
		if ( enable )
			memcpy( hi_ram, &ram() [rom_addr], sizeof hi_ram );
		
		memcpy( &ram() [rom_addr], (enable ? rom_ : hi_ram), rom_size );
		// TODO: ROM can still get overwritten when DSP writes to echo buffer
	}
}

void Spc_Core::ram_loaded()
{
	// ROM isn't swapped in anymore, since its area was overwritten with
	// contents of RAM there
	rom_enabled = false;
	
	// Put STOP instruction around memory to catch PC underflow/overflow
	memset( padded_ram,                          padding_fill, ram_padding );
	memset( padded_ram + ram_padding + ram_size, padding_fill, ram_padding );
}

void Spc_Core::load_regs( byte const new_regs [reg_count], byte const ports [port_count] )
{
	memcpy( regs,    new_regs, reg_count );
	memcpy( regs_in, regs,     reg_count );
	
	// These always read back as 0 or are handled specially
	// Commented registers are already copied from regs.
	regs_in [r_test    ] = 0;
	regs_in [r_control ] = 0;
	//       r_dspaddr
	regs_in [r_dspdata ] = 0xFF; // in case attempt is made to execute value
	//       r_cpuio0
	//       r_cpuio1
	//       r_cpuio2
	//       r_cpuio3
	//       8
	//       9
	regs_in [r_t0target] = 0;
	regs_in [r_t1target] = 0;
	regs_in [r_t2target] = 0;
	regs_in [r_t0out   ] &= 0x0F;
	regs_in [r_t1out   ] &= 0x0F;
	regs_in [r_t2out   ] &= 0x0F;
	
	// RAM should always hold copy of regs_in
	memcpy( ram() + regs_addr, regs_in, reg_count );
	
	enable_rom( regs [r_control] & 0x80 );
	
	for ( int i = 0; i < port_count; i++ )
		regs [r_cpuio0 + i] = ports [i];
}

void Spc_Core::reset_common( int timer_counter_init )
{
	memset( &cpu_regs, 0, sizeof cpu_regs );
	cpu_regs.pc = rom_addr;
	
	cpu_error     = NULL;
	echo_accessed = 0;
	spc_time      = 0;
	dsp_time      = 0;
	skipped_kon   = 0;
	skipped_koff  = 0;
	
	#if SPC_LESS_ACCURATE
		dsp_time = clocks_per_sample + 1;
	#endif
	
	int i;
	for ( i = 0; i < timer_count; i++ )
	{
		timers [i].time    = 1;
		timers [i].divider = 0;
	}
	
	byte new_regs [reg_count] = { };
	
	new_regs [r_test   ] = 0x0A;
	new_regs [r_control] = 0x80; // ROM enabled
	
	for ( i = 0; i < timer_count; i++ )
		new_regs [r_t0out + i] = timer_counter_init;
	
	static byte const new_ports [port_count] = { };
	
	load_regs( new_regs, new_ports );
}

void Spc_Core::soft_reset()
{
	reset_common( 0 );
	
	dsp_.soft_reset();
}

void Spc_Core::reset()
{
	memset( ram(), 0xFF, 0x10000 );
	ram_loaded();
	
	reset_common( 0x0F );
	
	dsp_.reset();
}

const char* Spc_Core::error()
{
	const char* e = cpu_error;
	cpu_error = NULL;
	return e;
}

bool Spc_Core::check_echo_access( int addr )
{
	if ( !(dsp_.read( dsp_.r_flg ) & 0x20) )
	{
		int start = 0x100 * dsp_.read( dsp_.r_esa );
		int size  = 0x800 * (dsp_.read( dsp_.r_edl ) & 0x0F);
		int end   = start + (size ? size : 4);
		if ( start <= addr && addr < end )
		{
			if ( !echo_accessed )
			{
				echo_accessed = true;
				return true;
			}
		}
	}
	return false;
}

// (n ? n : 256) & 0xFF
inline int if_0_then_256( int n )
{
	return byte (n - 1) + 1;
}

void Spc_Core::run_timer_( rel_time_t time, int index )
{
	Timer& t = timers [index];
	check( time >= t.time );
	
	int elapsed; // number of ticks, guaranteed at least 1
	int const timer01_shift = 3;
	
	#if SPC_DISABLE_TEMPO
	{
		int shift = timer2_shift;
		if ( index < 2 )
			shift += timer01_shift;
		
		elapsed = ((time - t.time) >> shift) + 1;
		t.time += elapsed << shift;
	}
	#else
	{
		int prescaler = prescaler_base;
		if ( index < 2 )
			prescaler <<= timer01_shift;
		
		elapsed = (time - t.time) / prescaler + 1;
		t.time += elapsed * prescaler;
	}
	#endif
	
	if ( regs [r_control] >> index & 1 )
	{
		int period = if_0_then_256( regs [r_t0target + index] );
		
		// Ticks until divider will output tick. If divider currently matches
		// period, it will be 256 ticks until output tick, not 0.
		int remain = if_0_then_256( period - t.divider );
		
		// value divider will take, assuming no tick
		int divider = t.divider + elapsed;
		
		// time relative to when tick will occur
		int over = elapsed - remain;
		if ( over >= 0 )
		{
			// ticks elapsed in second stage
			int elapsed2 = over / period;
			
			regs_in [r_t0out + index] =
					(regs_in [r_t0out + index] + 1 + elapsed2) & 0x0F;
			
			// remaining count on divider
			divider = over - elapsed2 * period;
		}
		
		// Must mask for cases when divider has gone past target
		t.divider = divider & 0xFF;
	}
}
