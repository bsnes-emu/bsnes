  uint16 vcounter();
  uint16 hcounter();
  uint16 hclock();

  bool   interlace();
  bool   interlace_field();
  bool   overscan();
  uint16 region_scanlines();

  void   set_interlace(bool r);
  void   set_overscan(bool r);

  uint   dma_counter();

  void   add_clocks(uint clocks);
  void   scanline();
  void   frame();

  void   precycle_edge();
  void   cycle_edge();
  void   last_cycle();
  uint32 clocks_executed();

  void   timing_power();
  void   timing_reset();

//timeshift.cpp
  void   timeshift_forward (uint clocks, uint &v, uint &h);
  void   timeshift_backward(uint clocks, uint &v, uint &h);

//irq.cpp
enum { IRQ_TRIGGER_NEVER = 0x3fff };
  void   update_interrupts();
  void   poll_interrupts(uint clocks);
#ifdef FAVOR_SPEED
  void   poll_interrupts_range(uint clocks);
#endif
  bool   nmi_edge();
  bool   irq_edge();
  void   irqpos_update(uint16 addr);
  bool   irq_pos_valid();
  bool   nmi_test();
  bool   irq_test();

//joypad.cpp
  void   run_auto_joypad_poll();
