class bAPUTimer {
public:
uint8 cycle_frequency, target;
uint8 stage1_ticks, stage2_ticks, stage3_ticks;
bool  enabled;
  inline void add_cycles(int cycles);
} t0, t1, t2;
  inline void   add_cycles(int cycles);
  inline uint32 clocks_executed();
