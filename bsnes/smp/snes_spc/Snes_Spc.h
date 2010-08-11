// SNES SPC-700 APU emulator/SPC music file player

// snes_spc 0.9.5
#ifndef BLARGG_SNES_SPC_H
#define BLARGG_SNES_SPC_H

#include "spc.h"
#include "Spc_Core.h"
#include "blargg_endian.h"

BLARGG_NAMESPACE_BEGIN

struct spc_t : public Spc_Core {
public:
	// Must be called once before using. OK to call more than once.
	blargg_err_t init();
	
	// Sample pairs generated per second
	enum { sample_rate = 32000 };
	
	// Number of samples in internal buffer. Increased by 2 (stereo) every 32
	// clocks of emulation.
	int samples_avail() const;
	
	// Reads at most count samples from internal buffer and returns number
	// actually read, less if samples_avail() < count
	typedef short sample_t;
	int read_samples( sample_t out [], int count );
	
	// Removes samples without reading. Returns number actually removed.
	int remove_samples( int count );
	
// Sound control
	
	// Mutes voices corresponding to non-zero bits in mask.
	// Reduces emulation accuracy.
	enum { voice_count = 8 };
	void mute_voices( int mask );
	
	// If true, prevents channels and global volumes from being phase-negated.
	// Only supported by fast DSP.
	void disable_surround( bool disable = true );
	
// SPC music files

	// Loads SPC data into emulator
	enum { spc_min_file_size = 0x10180 };
	enum { spc_file_size     = 0x10200 };
	blargg_err_t load_spc( void const* in, long size );
	
	// Clears echo region if appropriate. Useful after loading an SPC, as many have
	// garbage in echo.
	void clear_echo();

	// Plays for count samples and write samples to out. Discards samples if out
	// is NULL. Count must be a multiple of 2 since output is stereo.
	blargg_err_t play( int count, sample_t out [] );
	
	// Skips count samples. When using fast DSP, this is several times faster than
	// simply calling play() and discarding samples.
	blargg_err_t skip( int count );
	
// only available when using accurate DSP
#if !SPC_NO_COPY_STATE_FUNCS
	
	// Writes minimal header to spc_out
	static void init_header( void* spc_out );

	// Saves emulator state as SPC file data. Writes spc_file_size bytes to spc_out.
	// Does not set up SPC header; use init_header() for that.
	void save_spc( void* spc_out );

	// Returns true if new key-on events occurred since last check. Useful for
	// trimming silence while saving an SPC.
	bool check_kon();
#endif

	enum { signature_size = 35 };
	
public:
	// "Overrides"
	void reset();
	void soft_reset();
	void end_frame( time_t );
	
	// Deprecated
	BLARGG_DEPRECATED( void set_output( sample_t* out, int out_size ); )
	BLARGG_DEPRECATED( int sample_count() const; )

private:
	struct spc_file_t
	{
		char signature [signature_size];
		byte has_id666;
		byte version;
		byte pcl, pch;
		byte a;
		byte x;
		byte y;
		byte psw;
		byte sp;
		char text [212];
		byte ram [0x10000];
		byte dsp [128];
		byte unused [0x40];
		byte ipl_rom [0x40];
	};
	
	static char const signature [signature_size + 1];
	
	int clocks_avail;
	blargg_vector<sample_t> buf;
	
	sample_t* user_out;
	BLARGG_MUTABLE int user_pos;
	int user_size;
	
	int raw_samples_avail() const;
	void reset_samples();
};

inline void Snes_Spc::mute_voices( int mask )
{
	dsp().mute_voices( mask );
}
	
inline void Snes_Spc::disable_surround( bool disable )
{
	dsp().disable_surround( disable );
}

#if !SPC_NO_COPY_STATE_FUNCS
inline bool Snes_Spc::check_kon()
{
	return dsp().check_kon();
}
#endif

BLARGG_NAMESPACE_END

#endif
