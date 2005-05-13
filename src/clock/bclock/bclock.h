class bClock : public Clock {
private:
struct {
bool changed, notify_ppu;
uint8 count, new_count, pos;
}frameskip;
  void frameskip_update_status();
  void inc_vcounter();
  void dram_refresh_test();

public:
  struct {
    uint32 frequency;
    uint32 pos, last_pos;
    uint32 frame_cycles, frame_lines, frame_pos;
    uint16 line_cycles;
    bool   dram_refreshed; //whether or not dram has been refreshed on this line
    uint16 dram_refresh_pos;

    bool   render_frame;
  }cc1;
  struct {
    uint32 frequency;
    uint32 pos, last_pos;
  }cc2;

  void set_frameskip(uint8 fs);
  bool update_frame();

  void add_cc1_cycles(uint32 cycles);
  void add_cc2_cycles(uint32 cycles);
  void sync();
  void run();
  void power();
  void reset();

  bClock();
  ~bClock();
};
