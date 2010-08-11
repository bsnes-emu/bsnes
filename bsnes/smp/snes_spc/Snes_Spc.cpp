// SPC emulation support: init, sample buffering, reset, SPC loading

// snes_spc 0.9.5. http://www.slack.net/~ant/

#include "Snes_Spc.h"

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

// Extra silence added at beginning, to handle cases where fast DSP falls
// behind and thus doesn't generate all the samples needed.
int const extra_samples = 2;

int const stereo = 2; // for clarity

char const Snes_Spc::signature [signature_size + 1] =
		"SNES-SPC700 Sound File Data v0.30\x1A\x1A";

blargg_err_t Snes_Spc::init()
{
	RETURN_ERR( Spc_Core::init() );
	RETURN_ERR( buf.resize( sample_rate / 30 * stereo ) );
	reset_samples();
	
	return blargg_ok;
}

void Snes_Spc::reset_samples()
{
	user_out  = NULL;
	user_pos  = 0;
	user_size = 0;
	
	clocks_avail = 0;
	memset( buf.begin(), 0, extra_samples * sizeof buf [0] );
	dsp().set_output( buf.begin() + extra_samples, buf.end() );
}

void Snes_Spc::reset()
{
	reset_samples();
	Spc_Core::reset();
}

void Snes_Spc::soft_reset()
{
	reset_samples();
	Spc_Core::soft_reset();
}

inline int Snes_Spc::raw_samples_avail() const
{
	return dsp().output_ptr() - buf.begin();
}

int Snes_Spc::samples_avail() const
{
	return (unsigned) clocks_avail / clocks_per_sample * stereo;
}

int Snes_Spc::remove_samples( int count )
{
	count = min( count, samples_avail() );
	int remain = raw_samples_avail() - count;
	if ( remain < 0 )
	{
		check( false );
		count += remain;
		remain = 0;
	}
	
	clocks_avail -= clocks_per_sample / stereo * count;
	memmove( buf.begin(), buf.begin() + count, remain * sizeof buf [0] );
	dsp().set_output( buf.begin() + remain, buf.end() );
	
	return count;
}

int Snes_Spc::read_samples( sample_t out [], int count )
{
	count = min( count, samples_avail() );
	
	memcpy( out, buf.begin(), count * sizeof buf [0] );
	
	remove_samples( count );
	
	return count;
}

// Keep track of number of samples we should have generated so far, since
// fast DSP might be slightly ahead or behind.
void Snes_Spc::end_frame( time_t end )
{
	clocks_avail += end;
	Spc_Core::end_frame( end );
	
	int excess = samples_avail() - raw_samples_avail();
	
	// Worst-case, DSP can be 6+36 clocks ahead, and clocks_avail%32==31,
	// generating an extra 4 samples.
	check( excess >= -4 - extra_samples );
	
	if ( excess > 0 )
	{
		// DSP should never get behind what's needed, given that it
		// starts with extra_samples in buffer.
		clocks_avail -= clocks_per_sample / stereo * excess;
		check( false );
	}
}

#if BLARGG_LEGACY

void Snes_Spc::set_output( sample_t* out, int out_size )
{
	user_out  = out;
	user_pos  = 0;
	user_size = out_size;
}

int Snes_Spc::sample_count() const
{
	int more = read_samples( user_out + user_pos, user_size - user_pos );
	CONST_CAST(int&,user_pos) += more;
	return user_pos;
}

#endif

blargg_err_t Snes_Spc::load_spc( void const* data, long size )
{
	spc_file_t const* const spc = (spc_file_t const*) data;
	
	// be sure compiler didn't insert any padding into spc_file_t
	assert( sizeof (spc_file_t) == spc_min_file_size + 0x80 );
	
	// Check signature and file size
	if ( size < signature_size || memcmp( spc, signature, 27 ) )
		return "Not an SPC file";
	
	if ( size < spc_min_file_size )
		return "Corrupt SPC file";
	
	reset();
	
	// CPU registers
	cpu().pc  = spc->pch * 0x100 + spc->pcl;
	cpu().a   = spc->a;
	cpu().x   = spc->x;
	cpu().y   = spc->y;
	cpu().psw = spc->psw;
	cpu().sp  = spc->sp;
	
	// RAM and registers
	memcpy( ram(), spc->ram, 0x10000 );
	ram_loaded();
	static byte const new_ports [port_count] = { };
	load_regs( spc->ram + 0xF0, new_ports );
	
	// DSP registers
	dsp().load( spc->dsp );
	
	return blargg_ok;
}

void Snes_Spc::clear_echo()
{
	if ( !(dsp().read( Spc_Dsp::r_flg ) & 0x20) )
	{
		int addr = 0x100 * dsp().read( Spc_Dsp::r_esa );
		int end  = addr + 0x800 * (dsp().read( Spc_Dsp::r_edl ) & 0x0F);
		
		addr = max( addr, 0x200 );
		end  = min( end, 0x10000 ); // if it wraps around, stop at end of RAM
		
		// 0xFF = STOP, so if this overwrites any code, SPC won't play,
		// rather than behave strangely
		memset( &ram() [addr], 0xFF, end - addr );
	}
}


//// Sample output

blargg_err_t Snes_Spc::play( int count, sample_t out [] )
{
	assert( count % 2 == 0 ); // must be even
	
	// Refill and empty buffer until satisfied
	while ( count > 0 )
	{
		if ( samples_avail() == 0 )
			end_frame( clocks_per_sample / stereo * (buf.size() - 16) );
		
		int n;
		if ( out )
		{
			n = read_samples( out, count );
			out += n;
		}
		else
		{
			n = min( samples_avail(), count );
			remove_samples( n );
		}
		
		count -= n;
	}
	
	return error();
}

blargg_err_t Snes_Spc::skip( int count )
{
	assert( count % 2 == 0 );
	
	count -= remove_samples( count );
	
	int const final_skip = sample_rate * stereo;
	
	if ( count >= 2 * final_skip )
	{
		end_frame_skip( clocks_per_sample / stereo * (count - final_skip) );
		clear_echo();
		
		count = final_skip;
	}
	
	return play( count, NULL );
}
