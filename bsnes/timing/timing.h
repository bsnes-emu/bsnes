#define CYCLES_PER_SCANLINE  1360
#define WRAM_REFRESH_DOT_POS  132

#define TIMING_NONE       0x00
#define TIMING_BANKCROSS  0x01
#define TIMING_REGD       0x02
#define TIMING_CONDITION2 0x02
#define TIMING_CONDITION4 0x04

class snes_timer {
public:
ulong master_cycles;
word vscan_pos, hscan_pos;
  ulong get_master_cycle_count(ulong offset);
  void  add_cpu_pcycles(ulong n);
  void  add_cpu_scycles(ulong n);
  void  add_cpu_mcycles(ulong n, ulong addr);
  void  add_cpu_icycles(ulong n);
  void  add_cpu_icycles(ulong n, ulong flags);
  void  add_cpu_cycles(ulong n);
  void  update_timer(void);
  snes_timer();
};
