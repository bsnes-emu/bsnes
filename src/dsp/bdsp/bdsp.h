class bDSP : public DSP {
public:
	void enter();
	
	uint8 read( uint8 addr );
	void write( uint8 addr, uint8 data );

	void power();
	void reset();
	
	bDSP();
	~bDSP();

	template<int n, typename T> inline T asr(const T x) {
	enum { bits = (sizeof(T) << 3) - n };
		return sclip<bits>(x >> n);
	}

public:

	enum { echo_hist_size = 8 };
	enum { register_count = 128 };
	enum { voice_count = 8 };
	
	enum env_mode_t { env_release, env_attack, env_decay, env_sustain };
	enum { brr_buf_size = 12 };
	struct voice_t
	{
		int buf [brr_buf_size*2];// decoded samples (twice the size to simplify wrap handling)
		int buf_pos;            // place in buffer where next samples will be decoded
		int interp_pos;         // relative fractional position in sample (0x1000 = 1.0)
		int brr_addr;           // address of current BRR block
		int brr_offset;         // current decoding offset in BRR block
		uint8* regs;            // pointer to voice's DSP registers
		int vbit;               // bitmask for voice: 0x01 for voice 0, 0x02 for voice 1, etc.
		int kon_delay;          // KON delay/current setup phase
		env_mode_t env_mode;
		int env;                // current envelope level
		int hidden_env;         // used by GAIN mode 7, very obscure quirk
		uint8 t_envx_out;
	};
private:
	
	struct state_t
	{
		uint8 regs [register_count];
		
		// Echo history keeps most recent 8 samples
		int echo_hist [echo_hist_size] [2];
		int echo_hist_pos;
		
		int every_other_sample; // toggles every sample
		int kon;                // KON value when last checked
		int noise;
		int counter;
		int echo_offset;        // offset from ESA in echo buffer
		int echo_length;        // number of bytes that echo_offset will stop at
		
		// Hidden registers also written to when main register is written to
		int new_kon;
		uint8 endx_buf;
		uint8 envx_buf;
		uint8 outx_buf;
		
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
		
		// internal state that is recalculated every sample
		int t_dir_addr;
		int t_pitch;
		int t_output;
		int t_looped;
		
		// left/right sums
		int t_main_out [2];
		int t_echo_out [2];
		
		voice_t voices [voice_count];
	};
	state_t m;
	uint8* ram;
	
	unsigned read_counter( int rate );
	
	void run_envelope( voice_t* const v );
	void decode_brr( voice_t* v );

	void voice_output( voice_t const* v, int ch );
	void voice_V1( voice_t* const );
	void voice_V2( voice_t* const );
	void voice_V3( voice_t* const );
	void voice_V3a( voice_t* const );
	void voice_V3b( voice_t* const );
	void voice_V3c( voice_t* const );
	void voice_V4( voice_t* const );
	void voice_V5( voice_t* const );
	void voice_V6( voice_t* const );
	void voice_V7( voice_t* const );
	void voice_V8( voice_t* const );
	void voice_V9( voice_t* const );
	void voice_V7_V4_V1( voice_t* const );
	void voice_V8_V5_V2( voice_t* const );
	void voice_V9_V6_V3( voice_t* const );

	int  calc_echo_output( int ch, int sample );
	
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
};

inline uint8 bDSP::read( uint8 addr )
{
	return m.regs [addr];
}

inline void bDSP::write( uint8 addr, uint8 data )
{
	m.regs [addr] = data;
	switch ( addr & 0x0F )
	{
	case v_envx:
		m.envx_buf = data;
		break;
		
	case v_outx:
		m.outx_buf = data;
		break;
	
	case 0x0C:
		if ( addr == r_kon )
			m.new_kon = data;
		
		if ( addr == r_endx ) // always cleared, regardless of data written
		{
			m.endx_buf = 0;
			m.regs [r_endx] = 0;
		}
		break;
	}
}
