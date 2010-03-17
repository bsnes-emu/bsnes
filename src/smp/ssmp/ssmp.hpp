class sSMP : public SMP, public SMPcore {
public:
  void enter();
  debugvirtual void op_step();

  #include "memory/memory.hpp"
  #include "timing/timing.hpp"

  struct {
    uint8 opcode;

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

  //ssmp.cpp
  void power();
  void reset();

  void serialize(serializer&);
  sSMP();
  ~sSMP();

  friend class sSMPDebug;
};

#if defined(DEBUGGER)
  #include "debugger/debugger.hpp"
  extern sSMPDebugger smp;
#else
  extern sSMP smp;
#endif
