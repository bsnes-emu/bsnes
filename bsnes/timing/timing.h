//mask table (powers of 2)
#define TIMING_NONE       0x00
#define TIMING_CONDITION2 0x01 //DL != 0
#define TIMING_CONDITION4 0x02 //crossed page boundary or p.x = 0

typedef struct {
  word cx, x, y;
}latch_pos;

class snes_timer {
public:
ulong apu_cycles;
ulong master_cycles, prev_master_cycles;
struct {
  ulong cpu_cycles, apu_cycles;
}bridge;
word  hcycle_pos, vscan_pos, hscan_pos;
bool  vscan_wrapped, hscan_wrapped;
bool  dram_refreshed, nmi_triggered;
byte  *mem_speed_map, *sm_slowrom, *sm_fastrom;
ulong frame_cycles, frame_lines, line_cycles;
word  dram_refresh_pos;
latch_pos *latch_table_a, *latch_table_b, *latch_table;

  void  add_cpu_cycles (byte count, byte speed);
  void  add_cpu_pcycles(byte count);
  void  add_cpu_mcycles(byte count, ulong addr);
  void  add_cpu_scycles(byte count);
  void  add_cpu_icycles(byte count);

  ulong get_master_cycle_count(ulong offset);
  void  build_speed_map(void);
  void  build_dot_map(void);
  void  reset_clock(void);
  void  inc_vscan_pos(void);
  void  set_scan_pos(ulong cycle_pos, bool update_frame_info);
  void  set_speed_map(byte speed);
  void  update_dram_refresh_pos(void);
  void  update_timer(void);
  void  update_timer_events(void);
  snes_timer();
  ~snes_timer();
};
