// SPC emulation state save/load: copy_state(), save_spc()
// Separate file to avoid linking in unless needed

// snes_spc 0.9.5. http://www.slack.net/~ant/

#include "Snes_Spc.h"

#if !SPC_NO_COPY_STATE_FUNCS

#include <string.h>

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

void Snes_Spc::init_header( void* spc_out )
{
	spc_file_t* const spc = (spc_file_t*) spc_out;
	
	spc->has_id666 = 26; // has none
	spc->version   = 30;
	memcpy( spc, signature, sizeof spc->signature );
	memset( spc->text, 0, sizeof spc->text );
}

void Spc_Core::save_regs( byte out [reg_count], byte ports [port_count] )
{
	memcpy( out, regs, reg_count );
	
	int i;
	for ( i = 0; i < port_count; i++ )
		out [r_cpuio0 + i] = regs_in [r_cpuio0 + i];
	
	for ( i = 0; i < timer_count; i++ )
		out [r_t0out + i] = regs_in [r_t0out + i];
	
	memcpy( ports, &regs [r_cpuio0], port_count );
}

void Spc_Core::save_ram( byte out [65536] )
{
	memcpy( out, ram(), 65536 );
	
	if ( rom_enabled )
		memcpy( out + rom_addr, hi_ram, sizeof hi_ram );
}

void Snes_Spc::save_spc( void* spc_out )
{
	spc_file_t* const spc = (spc_file_t*) spc_out;
	
	// CPU
	spc->pcl = (byte) (cpu().pc >> 0);
	spc->pch = (byte) (cpu().pc >> 8);
	spc->a   = cpu().a;
	spc->x   = cpu().x;
	spc->y   = cpu().y;
	spc->psw = cpu().psw;
	spc->sp  = cpu().sp;
	
	// RAM, ROM
	save_ram( spc->ram );
	memset( spc->unused, 0, sizeof spc->unused );
	memcpy( spc->ipl_rom, rom(), sizeof spc->ipl_rom );
	
	// SMP registers
	byte out_ports [port_count]; // ignored
	save_regs( &spc->ram [regs_addr], out_ports );
	
	// DSP registers
	for ( int i = 0; i < Spc_Dsp::register_count; i++ )
		spc->dsp [i] = dsp().read( i );
}

void Spc_Core::copy_state( unsigned char** io, copy_func_t copy )
{
	SPC_State_Copier copier( io, copy );
	
	// Make state data more readable by putting 64K RAM, 16 SMP registers,
	// then DSP (with its 128 registers) first
	
	// RAM
	
	// Disable ROM so that entire RAM will be in ram(). If ROM was enabled,
	// will get re-enabled by load_regs() below.
	enable_rom( false );
	copier.copy( ram(), 0x10000 );
	
	// SMP registers
	{
		byte new_regs [reg_count];
		byte out_ports [port_count];
		
		save_regs( new_regs, out_ports );
		copier.copy( new_regs, sizeof new_regs );
		copier.copy( out_ports, sizeof out_ports );
		load_regs( new_regs, out_ports );
	}
	
	// CPU registers
	SPC_COPY( uint16_t, cpu().pc );
	SPC_COPY(  uint8_t, cpu().a );
	SPC_COPY(  uint8_t, cpu().x );
	SPC_COPY(  uint8_t, cpu().y );
	SPC_COPY(  uint8_t, cpu().psw );
	SPC_COPY(  uint8_t, cpu().sp );
	copier.extra();
	
	SPC_COPY( int16_t, spc_time );
	SPC_COPY( int16_t, dsp_time );
	
	// DSP
	dsp().copy_state( io, copy );
	
	// Timers
	for ( int i = 0; i < timer_count; i++ )
	{
		Timer* t = &timers [i];
		SPC_COPY( int16_t, t->time );
		SPC_COPY( uint8_t, t->divider );
		copier.extra();
	}
	copier.extra();
}
#endif
