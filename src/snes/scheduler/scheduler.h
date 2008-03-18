class Scheduler {
public:
  cothread_t thread_snes;
  cothread_t thread_cpu;
  cothread_t thread_smp;
  cothread_t thread_ppu; //currently unused
  cothread_t thread_dsp;

  enum ActiveThread {
    THREAD_CPU,
    THREAD_SMP,
    THREAD_PPU,
    THREAD_DSP,
  };

  struct {
    uint cpu_freq;
    uint smp_freq;

    ActiveThread active;
    int64 cpusmp;
    int64 smpdsp;
  } clock;

  // CPU <> SMP

  alwaysinline void sync_cpusmp() {
    if(clock.cpusmp < 0) {
      clock.active = THREAD_SMP;
      co_switch(thread_smp);
    }
  }

  alwaysinline void sync_smpcpu() {
    if(clock.cpusmp >= 0) {
      clock.active = THREAD_CPU;
      co_switch(thread_cpu);
    }
  }

  // SMP <> DSP

  alwaysinline void sync_smpdsp() {
    if(clock.smpdsp < 0) {
      clock.active = THREAD_DSP;
      co_switch(thread_dsp);
    }
  }

  alwaysinline void sync_dspsmp() {
    if(clock.smpdsp >= 0) {
      clock.active = THREAD_SMP;
      co_switch(thread_smp);
    }
  }

  // Timing

  alwaysinline void addclocks_cpu(uint clocks) {
    clock.cpusmp -= clocks * (uint64)clock.smp_freq;
    if(clock.cpusmp < -(250000 * (int64)20000000)) sync_cpusmp();
  }

  alwaysinline void addclocks_smp(uint clocks) {
    clock.cpusmp += clocks * (uint64)clock.cpu_freq;
    if(clock.cpusmp > +(250000 * (int64)20000000)) sync_smpcpu();
    clock.smpdsp -= clocks;
    sync_smpdsp();
  }

  alwaysinline void addclocks_dsp(uint clocks) {
    clock.smpdsp += clocks;
    sync_dspsmp();
  }

  void enter();
  void exit();
  void init();

  Scheduler();
};

extern Scheduler scheduler;
