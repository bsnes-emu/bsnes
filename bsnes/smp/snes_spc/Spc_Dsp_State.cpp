// snes_spc 0.9.5. http://www.slack.net/~ant/

#include "Spc_Dsp.h"

#if !SPC_NO_COPY_STATE_FUNCS

#include "blargg_endian.h"

/* Copyright (C) 2007-2010 Shay Green. This module is free software; you
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

void SPC_State_Copier::copy( void* state, size_t size )
{
	func( buf, state, size );
}

int SPC_State_Copier::copy_int( int state, int size )
{
	byte s [2];
	SET_LE16( s, state );
	func( buf, &s, size );
	return GET_LE16( s );
}

void SPC_State_Copier::skip( int count )
{
	if ( count > 0 )
	{
		char temp [64];
		memset( temp, 0, sizeof temp );
		do
		{
			int n = sizeof temp;
			if ( n > count )
				n = count;
			count -= n;
			func( buf, temp, n );
		}
		while ( count );
	}
}

void SPC_State_Copier::extra()
{
	int n = 0;
	SPC_State_Copier& copier = *this;
	SPC_COPY( uint8_t, n );
	skip( n );
}

void Spc_Dsp::copy_state( unsigned char** io, copy_func_t copy )
{
	SPC_State_Copier copier( io, copy );
	
	// DSP registers
	copier.copy( regs, register_count );
	
	// Internal state
	
	// Voices
	int i;
	for ( i = 0; i < voice_count; i++ )
	{
		voice_t* v = &m.voices [i];
		
		// BRR buffer
		for ( int bi = 0; bi < brr_buf_size; bi++ )
		{
			int s = v->buf [bi];
			SPC_COPY(  int16_t, s );
			v->buf [bi] = v->buf [bi + brr_buf_size] = s;
		}
		
		SPC_COPY( uint16_t, v->interp_pos );
		SPC_COPY( uint16_t, v->brr_addr );
		SPC_COPY( uint16_t, v->env );
		SPC_COPY(  int16_t, v->hidden_env );
		SPC_COPY(  uint8_t, v->buf_pos );
		SPC_COPY(  uint8_t, v->brr_offset );
		SPC_COPY(  uint8_t, v->kon_delay );
		{
			int mode = v->env_mode;
			SPC_COPY(  uint8_t, mode );
			v->env_mode = (enum env_mode_t) mode;
		}
		SPC_COPY(  uint8_t, v->t_envx_out );
		
		copier.extra();
	}
	
	// Echo history
	for ( i = 0; i < echo_hist_size; i++ )
	{
		int j;
		for ( j = 0; j < 2; j++ )
		{
			int s = m.echo_hist_pos [i] [j];
			SPC_COPY( int16_t, s );
			m.echo_hist [i] [j] = s; // write back at offset 0
		}
	}
	m.echo_hist_pos = m.echo_hist;
	memcpy( &m.echo_hist [echo_hist_size], m.echo_hist, echo_hist_size * sizeof m.echo_hist [0] );
	
	// Misc
	SPC_COPY(  uint8_t, m.every_other_sample );
	SPC_COPY(  uint8_t, m.kon );
	
	SPC_COPY( uint16_t, m.noise );
	SPC_COPY( uint16_t, m.counter );
	SPC_COPY( uint16_t, m.echo_offset );
	SPC_COPY( uint16_t, m.echo_length );
	SPC_COPY(  uint8_t, m.phase );
	
	SPC_COPY(  uint8_t, m.new_kon );
	SPC_COPY(  uint8_t, m.endx_buf );
	SPC_COPY(  uint8_t, m.envx_buf );
	SPC_COPY(  uint8_t, m.outx_buf );
	
	SPC_COPY(  uint8_t, m.t_pmon );
	SPC_COPY(  uint8_t, m.t_non );
	SPC_COPY(  uint8_t, m.t_eon );
	SPC_COPY(  uint8_t, m.t_dir );
	SPC_COPY(  uint8_t, m.t_koff );
	
	SPC_COPY( uint16_t, m.t_brr_next_addr );
	SPC_COPY(  uint8_t, m.t_adsr0 );
	SPC_COPY(  uint8_t, m.t_brr_header );
	SPC_COPY(  uint8_t, m.t_brr_byte );
	SPC_COPY(  uint8_t, m.t_srcn );
	SPC_COPY(  uint8_t, m.t_esa );
	SPC_COPY(  uint8_t, m.t_echo_enabled );
	
	SPC_COPY(  int16_t, m.t_main_out [0] );
	SPC_COPY(  int16_t, m.t_main_out [1] );
	SPC_COPY(  int16_t, m.t_echo_out [0] );
	SPC_COPY(  int16_t, m.t_echo_out [1] );
	SPC_COPY(  int16_t, m.t_echo_in  [0] );
	SPC_COPY(  int16_t, m.t_echo_in  [1] );
	
	SPC_COPY( uint16_t, m.t_dir_addr );
	SPC_COPY( uint16_t, m.t_pitch );
	SPC_COPY(  int16_t, m.t_output );
	SPC_COPY( uint16_t, m.t_echo_ptr );
	SPC_COPY(  uint8_t, m.t_looped );
	
	copier.extra();
}
#endif
