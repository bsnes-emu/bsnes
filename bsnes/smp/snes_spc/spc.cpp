// snes_spc 0.9.5. http://www.slack.net/~ant/

#include "spc.h"

#include "Snes_Spc.h"
#include "Spc_Filter.h"

/* Copyright (C) 2004-2007 Shay Green. This module is free software; you
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

spc_t* spc_new( void )
{
	// be sure constants match
	assert( spc_sample_rate         == (int) spc_t::sample_rate );
	assert( spc_rom_size            == (int) spc_t::rom_size );
	assert( spc_clock_rate          == (int) spc_t::clock_rate );
	assert( spc_clocks_per_sample   == (int) spc_t::clocks_per_sample );
	assert( spc_port_count          == (int) spc_t::port_count );
	assert( spc_voice_count         == (int) spc_t::voice_count );
	assert( spc_tempo_unit          == (int) spc_t::tempo_unit );
	assert( spc_file_size           == (int) spc_t::spc_file_size );
	#if !SPC_NO_COPY_STATE_FUNCS
	assert( spc_state_size          == (int) spc_t::state_size );
	#endif
	
	spc_t* s = new spc_t;
	if ( s && s->init() )
	{
		delete s;
		s = 0;
	}
	return s;
}

void spc_delete          ( spc_t* s )                               { delete s; }
void spc_init_rom        ( spc_t* s, unsigned char const r [64] )   { s->init_rom( r ); }
int  spc_read_samples    ( spc_t* s, spc_sample_t out [], int n )   { return s->read_samples( out, n ); }
int  spc_samples_avail   ( spc_t const* s )                         { return s->samples_avail(); }
void spc_reset           ( spc_t* s )                               { s->reset(); }
void spc_soft_reset      ( spc_t* s )                               { s->soft_reset(); }
int  spc_read_port       ( spc_t* s, spc_time_t t, int p )          { return s->read_port( t, p ); }
void spc_write_port      ( spc_t* s, spc_time_t t, int p, int d )   { s->write_port( t, p, d ); }
void spc_end_frame       ( spc_t* s, spc_time_t t )                 { s->end_frame( t ); }  
void spc_mute_voices     ( spc_t* s, int mask )                     { s->mute_voices( mask ); }
void spc_disable_surround( spc_t* s, int disable )                  { s->disable_surround( disable ); }
void spc_set_tempo       ( spc_t* s, int tempo )                    { s->set_tempo( tempo ); }
spc_err_t spc_load_spc   ( spc_t* s, void const* p, long n )        { return s->load_spc( p, n ); }
void spc_clear_echo      ( spc_t* s )                               { s->clear_echo(); }
spc_err_t spc_play       ( spc_t* s, int count, short* out )        { return s->play( count, out ); }
spc_err_t spc_skip       ( spc_t* s, int count )                    { return s->skip( count ); }

#if !SPC_NO_COPY_STATE_FUNCS
void spc_copy_state      ( spc_t* s, unsigned char** p, spc_copy_func_t f ) { s->copy_state( p, f ); }
void spc_init_header     ( void* spc_out )                          { spc_t::init_header( spc_out ); }
void spc_save_spc        ( spc_t* s, void* spc_out )                { s->save_spc( spc_out ); }
int  spc_check_kon       ( spc_t* s )                               { return s->check_kon(); }
#endif

#if BLARGG_LEGACY
void spc_set_output      ( spc_t* s, spc_sample_t* p, int n )       { s->set_output( p, n ); }
int  spc_sample_count    ( spc_t const* s )                         { return s->sample_count(); }
#endif

SPC_Filter* spc_filter_new( void )                              { return new SPC_Filter; }
void spc_filter_delete( SPC_Filter* f )                         { delete f; }
void spc_filter_run( SPC_Filter* f, spc_sample_t* p, int s )    { f->run( p, s ); }
void spc_filter_clear( SPC_Filter* f )                          { f->clear(); }
void spc_filter_set_gain( SPC_Filter* f, int gain )             { f->set_gain( gain ); }
void spc_filter_set_bass( SPC_Filter* f, int bass )             { f->set_bass( bass ); }
