/** SNES SPC-700 APU emulator and SPC music file player \file */

/* snes_spc 0.9.5 */
#ifndef SPC_H
#define SPC_H

#include <stddef.h>

#ifdef __cplusplus
	extern "C" {
#endif


/** First parameter of most functions is spc_t*, or const spc_t* if nothing is
changed. Once one of these functions returns an error, the object should not
be used any further, other than to delete it. */
typedef struct spc_t spc_t;

/** Pointer to error, or NULL if function was successful. See error functions
below. */
#ifndef spc_err_t /* (#ifndef allows better testing of library) */
	typedef const char* spc_err_t;
#endif

enum { /** Sample pairs generated per second */
spc_sample_rate = 32000 };


/**** Creation/deletion ****/

/** Creates new SPC emulator. NULL if out of memory. */
spc_t* spc_new( void );

/** Frees SPC emulator. OK to pass NULL. */
void spc_delete( spc_t* );


/**** SPC music file playback *****/

/** Loads SPC data into emulator */
spc_err_t spc_load_spc( spc_t*, void const* spc_in, long size );

/** Clears echo region. Useful after loading an SPC as many have garbage in echo. */
void spc_clear_echo( spc_t* );

/** Plays for count samples and writes samples to out. Discards samples if out
is NULL. Count must be a multiple of 2 since output is stereo. */
spc_err_t spc_play( spc_t*, int count, short* out );

/** Skips count samples. Several times faster than spc_play(). */
spc_err_t spc_skip( spc_t*, int count );


/**** Sound options ****/

/** Mutes voices corresponding to non-zero bits in mask. */
void spc_mute_voices( spc_t*, int mask );
enum { spc_voice_count = 8 };

/** If true, prevents channels and global volumes from being phase-negated.
Some SPC music does this to achieve a pseudo-surround-sound effect that is
unpleasant to listen to through headphones. */
void spc_disable_surround( spc_t*, int disable );

/** Sets music tempo, where spc_tempo_unit = normal,
spc_tempo_unit / 2 = half speed, etc. */
void spc_set_tempo( spc_t*, int );
enum { spc_tempo_unit = 0x100 };


/**** Emulator use ****/

/** Clock count relative to current time frame */
typedef int spc_time_t;

enum { /** Number of clocks per second */
spc_clock_rate = 1024000 };

enum { /** One sample pair is generated after this many clocks */
spc_clocks_per_sample = 32 };

enum { spc_rom_size = 0x40 };

/** Sets IPL ROM data. Library does not include ROM data. Most SPC music files
don't need ROM, but a full emulator must provide this. */
void spc_init_rom( spc_t*, const unsigned char rom [spc_rom_size] );

/** Resets SPC-700 to power-on state. */
void spc_reset( spc_t* );

/** Emulates pressing reset switch on SNES. */
void spc_soft_reset( spc_t* );

enum { spc_port_count = 4 /**< Number of ports, indexed from 0 */ };

/** Reads from port at specified time */
int  spc_read_port ( spc_t*, spc_time_t, int port );

/** Writes to port at specified time */
void spc_write_port( spc_t*, spc_time_t, int port, int data );

/** Emulates to end_time, starts a new time frame at 0, and makes all resulting
samples available. */
void spc_end_frame( spc_t*, spc_time_t end_time );

/** Number of samples available for reading. Always even. */
int spc_samples_avail( const spc_t* );

/** 16-bit signed sample */
typedef short spc_sample_t;

/** Writes at most count samples to out, removes them from internal buffer,
and returns number of samples actually written. Output is in stereo, so
count must be even. */
int spc_read_samples( spc_t*, spc_sample_t out [], int count );


/**** State save/load ****/

/** Not available when using fast DSP */

/** Callback used for state save/load. Called with io value passed to
spc_copy_state(). State points to size bytes of data to save or load,
as desired. */
typedef void (*spc_copy_func_t)( unsigned char** io, void* state, size_t size );

/** Saves/loads exact emulator state, using callback to save/load data.
The callback determines which action occurs. */
void spc_copy_state( spc_t*, unsigned char** io, spc_copy_func_t );
enum { spc_state_size = 67 * 1024  /**< maximum space needed when saving */ };

/** Writes minimal SPC file header to spc_out */
void spc_init_header( void* spc_out );

/** Saves emulator state as SPC file data. Writes spc_file_size bytes to spc_out.
Does not set up SPC header; use spc_init_header() for that. */
void spc_save_spc( spc_t*, void* spc_out );
enum { spc_file_size = 0x10200 /**> spc_out must have this many bytes allocated */ };

/** Returns non-zero if new key-on events occurred since last check. Useful for 
trimming silence while saving an SPC. */
int spc_check_kon( spc_t* );


/**** Sound filter ****/

/** Simple filter to more closely match sound output of SNES. Applies slight
high-pass and low-pass filtering. */

/** Filter functions take a pointer to this */
typedef struct Spc_Filter Spc_Filter;

/** Creates new filter. NULL if out of memory. */
Spc_Filter* spc_filter_new( void );

/** Frees filter. OK to pass NULL. */
void spc_filter_delete( Spc_Filter* );

/** Filters count samples of stereo sound in place. Count must be a multiple of 2. */
void spc_filter_run( Spc_Filter*, spc_sample_t io [], int count );

/** Clears filter to silence */
void spc_filter_clear( Spc_Filter* );

/** Sets gain (volume), where spc_filter_gain_unit is normal. Gains greater than
spc_filter_gain_unit are fine, since output is clamped to 16-bit sample range. */
void spc_filter_set_gain( Spc_Filter*, int gain );
enum { spc_filter_gain_unit = 0x100 };

/** Sets amount of bass (logarithmic scale) */
void spc_filter_set_bass( Spc_Filter*, int bass );
enum {
	spc_filter_bass_none =  0,
	spc_filter_bass_norm =  8, /**< normal amount of bass */
	spc_filter_bass_max  = 31
};




/**** Deprecated ****/

/* Provided for compatibility with old code. May be removed from future release. */

typedef spc_t Snes_Spc;
typedef struct Spc_Filter SPC_Filter;

void spc_set_output( spc_t*, spc_sample_t* out, int out_size );

int spc_sample_count( const spc_t* );

#ifdef __cplusplus
	}
#endif

#endif
