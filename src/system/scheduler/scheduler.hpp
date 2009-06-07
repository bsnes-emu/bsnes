//scheduler thread relationships:
//S-PPU <-> S-CPU <-> cartridge co-processor
//           <|>
//          S-SMP <-> S-DSP

class Scheduler {
public:
  cothread_t thread_snes;
  cothread_t thread_cpu;  //S-CPU (5a22)
  cothread_t thread_cop;  //cartridge co-processor (SuperFX, SA-1, ...)
  cothread_t thread_smp;  //S-SMP (SPC700)
  cothread_t thread_ppu;  //S-PPU
  cothread_t thread_dsp;  //S-DSP

  enum ActiveThread {
    THREAD_CPU,
    THREAD_COP,
    THREAD_SMP,
    THREAD_PPU,
    THREAD_DSP,
  };

  struct {
    unsigned cpu_freq;
    unsigned smp_freq;

    ActiveThread active;
    int64_t cpucop;
    int64_t cpuppu;
    int64_t cpusmp;
    int64_t smpdsp;
  } clock;

  //==========
  //CPU <> COP
  //==========

  alwaysinline void sync_cpucop() {
    if(clock.cpucop < 0) {
      clock.active = THREAD_COP;
      co_switch(thread_cop);
    }
  }

  alwaysinline void sync_copcpu() {
    if(clock.cpucop >= 0) {
      clock.active = THREAD_CPU;
      co_switch(thread_cpu);
    }
  }

  //==========
  //CPU <> PPU
  //==========

  alwaysinline void sync_cpuppu() {
    if(clock.cpuppu < 0) {
      clock.active = THREAD_PPU;
      co_switch(thread_ppu);
    }
  }

  alwaysinline void sync_ppucpu() {
    if(clock.cpuppu >= 0) {
      clock.active = THREAD_CPU;
      co_switch(thread_cpu);
    }
  }

  //==========
  //CPU <> SMP
  //==========

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

  //==========
  //SMP <> DSP
  //==========

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

  //==========
  //add clocks
  //==========

  alwaysinline void addclocks_cpu(unsigned clocks) {
    clock.cpucop -= clocks;
    clock.cpuppu -= clocks;
    clock.cpusmp -= clocks * (uint64_t)clock.smp_freq;
  }

  alwaysinline void addclocks_cop(unsigned clocks) {
    clock.cpucop += clocks;
  }

  alwaysinline void addclocks_ppu(unsigned clocks) {
    clock.cpuppu += clocks;
  }

  alwaysinline void addclocks_smp(unsigned clocks) {
    clock.cpusmp += clocks * (uint64_t)clock.cpu_freq;
    clock.smpdsp -= clocks;
  }

  alwaysinline void addclocks_dsp(unsigned clocks) {
    clock.smpdsp += clocks;
  }

  void enter();
  void exit();
  void init();

  Scheduler();
};

extern Scheduler scheduler;
