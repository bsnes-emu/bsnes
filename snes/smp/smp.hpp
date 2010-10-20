class SMP : public Processor, public SMPcore {
public:
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_cpu();
  alwaysinline void synchronize_dsp();

  uint8 port_read(uint8 port);
  void port_write(uint8 port, uint8 data);

  void power();
  void reset();

  void serialize(serializer&);
  SMP();
  ~SMP();

private:
  #include "memory/memory.hpp"
  #include "timing/timing.hpp"

  static const uint8 iplrom[64];

  struct {
    //timing
    unsigned clock_counter;
    unsigned dsp_counter;
    unsigned timer_step;

    //$00f0
    uint8 clock_speed;
    uint8 timer_speed;
    bool timers_enabled;
    bool ram_disabled;
    bool ram_writable;
    bool timers_disabled;

    //$00f1
    bool iplrom_enabled;

    //$00f2
    uint8 dsp_addr;

    //$00f8,$00f9
    uint8 smp_f8, smp_f9;
  } status;

  static void Enter();
  void enter();
  debugvirtual void op_step();

  friend class SMPcore;
  friend class SMPDebugger;
};

#if defined(DEBUGGER)
  #include "debugger/debugger.hpp"
  extern SMPDebugger smp;
#else
  extern SMP smp;
#endif
