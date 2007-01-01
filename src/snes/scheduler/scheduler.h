class Scheduler {
public:

cothread_t thread_snes;
cothread_t thread_cpu;
cothread_t thread_smp;

enum ActiveThread {
  THREAD_CPU,
  THREAD_SMP,
};

struct {
  uint cpu_freq;
  uint smp_freq;

  ActiveThread active;
  int64 cpusmp;
} clock;

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

  alwaysinline void addclocks_cpu(uint clocks) {
    clock.cpusmp -= clocks * (uint64)clock.smp_freq;
    if(clock.cpusmp < -(250000 * (int64)20000000))sync_cpusmp();
  }

  alwaysinline void addclocks_smp(uint clocks) {
    clock.cpusmp += clocks * (uint64)clock.cpu_freq;
    if(clock.cpusmp > +(250000 * (int64)20000000))sync_smpcpu();
  }

  void enter();
  void exit();
  void init();

  Scheduler();
};

extern Scheduler scheduler;
