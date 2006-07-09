class sAPUTimer {
public:
uint8 cycle_frequency, target;
uint8 stage1_ticks, stage2_ticks, stage3_ticks;
bool  enabled;
  inline void   tick();
} t0, t1, t2;

  inline void   add_clocks(int clocks);
  inline void   tick_timers();
  inline uint32 clocks_executed();
