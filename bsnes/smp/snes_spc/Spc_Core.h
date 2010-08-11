#ifndef BLARGG_SPC_CORE_H
#define BLARGG_SPC_CORE_H

#include "Spc_Dsp.h"
#include "blargg_endian.h"

BLARGG_NAMESPACE_BEGIN

class Spc_Core {
public:
	typedef BOOST::uint8_t byte;
	
	// Must be called once before using. OK to call more than once.
	blargg_err_t init();
	
	// Sets IPL ROM data. Default IPL ROM is zero-filled. Most SPC music files
	// don't need ROM, but a full emulator MUST provide this.
	enum { rom_size = 0x40 };
	void init_rom( byte const rom [rom_size] );
	
// Reset

	// Use functions in order listed here
	
	// Resets to power-on state
	void reset();

	// Emulates pressing reset on SNES
	void soft_reset();
	
	// Pointer to 64K RAM
	byte* ram();
	
	// S-SMP CPU registers
	struct regs_t
	{
		int pc;
		int a;
		int x;
		int y;
		int psw;
		int sp;
	};
	regs_t& cpu() { return cpu_regs; }
	
	// S-DSP
	      Spc_Dsp& dsp()       { return dsp_; }
	const Spc_Dsp& dsp() const { return dsp_; }
	
// Emulation
	
	// 1024000 SPC clocks per second, sample pair every 32 clocks
	typedef int time_t;
	enum { clock_rate = 1024000 };
	enum { clocks_per_sample = 32 };
	
	// Emulates port read/write at specified time
	enum { port_count = 4 };
	int  read_port ( time_t, int port );
	void write_port( time_t, int port, int data );

	// Runs SPC to end_time and starts a new time frame at 0
	void end_frame( time_t end_time );
	
// Sound control
	
	// Sets tempo, where tempo_unit = normal, tempo_unit/2 = half speed,
	// tempo_unit*2 = double speed, etc.
	enum { tempo_unit = 0x100 };
	void set_tempo( int );

// State save/load (only available with accurate DSP)

#if !SPC_NO_COPY_STATE_FUNCS
	// Saves/loads state
	enum { state_size = 67 * 1024 }; // maximum space needed when saving
	typedef Spc_Dsp::copy_func_t copy_func_t;
	void copy_state( unsigned char** io, copy_func_t );
#endif

protected:

	// Pointer to string describing CPU emulation error since last call, or NULL
	// if none. Clears error after returning.
	const char* error();
	
	enum { regs_addr = 0xF0 };
	enum { reg_count = 0x10 };
	
	// Saves registers in unified 16-byte format, and output ports
	// separately
	void save_regs( byte out [reg_count], byte out_ports [port_count] );
	
	// Loads registers from unified 16-byte format. Most registers are
	// last values S-SMP wrote. CPUIOx are last values S-CPU wrote to
	// APUIOx. TxOUT are values to load into timer counters. DSPDATA is
	// ignored. CONTROL is restored without disturbing CPUIOx and TxOUT.
	void load_regs( byte const in [reg_count], byte const ports [port_count] );
	
	void save_ram( byte out [65536] );
	
	// Call after loading new data into entire RAM
	void ram_loaded();
	
	byte const* rom() const     { return rom_; }
	
	void end_frame_skip( time_t );
	
public:
	BLARGG_DISABLE_NOTHROW
	
	enum { skipping_time = 127 };
private:
	// Time relative to m_spc_time. Speeds up code a bit by eliminating need to
	// constantly add m_spc_time to time from CPU. CPU uses time that ends at
	// 0 to eliminate reloading end time every instruction. It pays off.
	typedef int rel_time_t;
	
	// Main goal for emulator state is to have as little redundancy as possible.
	// That means the last values written to each SMP register, and any internal
	// state needed (e.g. timer prescaler and divider, but not counters, which
	// are stored in regs_in).
	
	// Registers
	enum {
        r_test     = 0x0, r_control  = 0x1,
        r_dspaddr  = 0x2, r_dspdata  = 0x3,
        r_cpuio0   = 0x4, r_cpuio1   = 0x5,
        r_cpuio2   = 0x6, r_cpuio3   = 0x7,
        r_f8       = 0x8, r_f9       = 0x9,
        r_t0target = 0xA, r_t1target = 0xB, r_t2target = 0xC,
        r_t0out    = 0xD, r_t1out    = 0xE, r_t2out    = 0xF
	};
	byte regs    [reg_count]; // last values SMP wrote
	byte regs_in [reg_count]; // values SMP should read, except timers
	
	const char* cpu_error;
	bool        echo_accessed;
	time_t      spc_time;
	rel_time_t  dsp_time;
	int         tempo;
	int         skipped_kon;
	int         skipped_koff;
	regs_t      cpu_regs;
	
	// IPL ROM
	enum { rom_addr = 0xFFC0 };
	bool        rom_enabled;
	byte rom_   [rom_size];
	byte hi_ram [rom_size];
	void enable_rom( bool enable );
	
	// Timers
	enum { timer2_shift = 4 };
	enum { timer_count  = 3 };
	struct Timer
	{
		rel_time_t time; // time of next stage 0 tick
		int divider;
	};
	Timer timers [timer_count];
	int   prescaler_base;
	void adjust_timers( int offset );
	void run_timer_   ( rel_time_t, int index );
	void run_timer    ( rel_time_t, int index );
	void enable_timers( rel_time_t, int mask );
	int  read_timer   ( rel_time_t, int index );
	
	Spc_Dsp dsp_;
	
	// 64K RAM with padding for CPU emulator to hit if it goes outside
	enum { padding_fill = 0xFF };
	enum { ram_padding = 0x100 };
	enum { ram_size = 0x10000 };
	byte padded_ram [ram_padding + ram_size + ram_padding];
	
	void cpu_write_smp_reg_( rel_time_t, int data, int reg );
	void cpu_write_smp_reg ( rel_time_t, int data, int reg );
	void cpu_write_high    ( rel_time_t, int data, int offset );
	void cpu_write         ( rel_time_t, int data, int addr );
	
	void dsp_write         ( rel_time_t, int data );
	int  dsp_read          ( rel_time_t );
	
	int cpu_read_smp_reg   ( rel_time_t, int reg );
	int cpu_read           ( rel_time_t, int addr );
	
	unsigned cpu_mem_bit   ( rel_time_t, byte const* pc );
	
	void write_apuio( int port, int data );
	void reset_common( int timer_counter_init );
	bool check_echo_access ( int addr );
	void run_until_( time_t end_time );
};

inline Spc_Core::byte* Spc_Core::ram()
{
	return &padded_ram [ram_padding];
}

inline int Spc_Core::read_port( time_t t, int port )
{
	assert( (unsigned) port < port_count );
	run_until_( t );
	return regs [r_cpuio0 + port];
}

inline void Spc_Core::write_apuio( int port, int data )
{
	regs_in [            r_cpuio0 + port] = data;
	ram()   [regs_addr + r_cpuio0 + port] = data;
}

inline void Spc_Core::write_port( time_t t, int port, int data )
{
	assert( (unsigned) port < port_count );
	run_until_( t );
	write_apuio( port, data );
}

inline void Spc_Core::end_frame_skip( rel_time_t end )
{
	assert( end % clocks_per_sample == 0 );
	
	skipped_kon  = 0;
	skipped_koff = 0;
	
	// Bump DSP by count, so it won't have to run at all
	dsp_time += end;
	end_frame( end );
	
	// Write most recent KOFF/KONs
	dsp_.write( Spc_Dsp::r_koff, skipped_koff & ~skipped_kon );
	dsp_.write( Spc_Dsp::r_kon , skipped_kon );
}

BLARGG_NAMESPACE_END

#endif
