class bClock : public Clock {
private:
struct {
bool changed, notify_ppu;
uint8 count, new_count, pos;
}frameskip;
bool signal_scanline, signal_frame;
  inline void frameskip_update_status();
  inline void inc_vcounter();
  inline void dram_refresh_test();

public:
  struct {
    uint32 frequency;
    uint32 pos, last_pos;
    uint32 frame_cycles, frame_lines, frame_pos;
    uint16 line_cycles;
    bool   dram_refreshed; //whether or not dram has been refreshed on this line
    uint16 dram_refresh_pos;
  }cc1;
  struct {
    uint32 frequency;
    uint32 pos, last_pos;
  }cc2;

  inline void set_frameskip(uint8 fs);

  inline void add_cc1_cycles(uint32 cycles);
  inline void add_cc2_cycles(uint32 cycles);
  inline void sync();
  inline void run();
  inline void power();
  inline void reset();

  bClock();
  ~bClock();
};
