  alwaysinline uint16 vcounter() { return status.vcounter; }
  alwaysinline uint16 hcounter() { return status.hcounter; }
  uint16 hdot();
  unsigned dma_counter();

  void add_clocks(unsigned clocks);
  void scanline();
  void frame();

  void precycle_edge();
  void cycle_edge();
  void last_cycle();
  uint32 clocks_executed();

  void timing_power();
  void timing_reset();

  //timeshifting -- needed by NMI and IRQ timing
  struct History {
    struct Time {
      uint16 vcounter;
      uint16 hcounter;
    } time[32];
    unsigned index;
    alwaysinline void enqueue(uint16 vcounter, uint16 hcounter) {
      Time &t = time[index++];
      index &= 31;
      t.vcounter = vcounter;
      t.hcounter = hcounter;
    }
    alwaysinline void query(unsigned offset, uint16 &vcounter, uint16 &hcounter) {
      Time &t = time[(index - (offset >> 1)) & 31];
      vcounter = t.vcounter;
      hcounter = t.hcounter;
    }
    void reset() {
      index = 0;
      for(unsigned i = 0; i < 32; i++) time[i].vcounter = time[i].hcounter = 0;
    }
    History() { reset(); }
  } history;

  //irq.cpp
  enum { IRQ_TRIGGER_NEVER = 0x3fff };
  void update_interrupts();
  void poll_interrupts();
  void nmitimen_update(uint8 data);
  void hvtime_update(uint16 addr);
  bool rdnmi();
  bool timeup();

  bool irq_pos_valid();
  bool nmi_test();
  bool irq_test();

  //joypad.cpp
  void run_auto_joypad_poll();
