  uint16 vcounter();
  uint16 hcounter();
  uint16 hcycles();

  bool   interlace();
  bool   interlace_field();
  bool   overscan();
  uint16 region_scanlines();

  void   set_interlace(bool r);
  void   set_overscan(bool r);

  void   add_clocks(uint clocks);
  void   scanline();
  void   frame();

  void   opcode_edge();
  void   cycle_edge();
  void   last_cycle();
  uint32 clocks_executed();
  void   timing_reset();

//irq.cpp
  void   update_interrupts();
  void   poll_interrupts(int clocks);
  bool   nmi_read_pos_match(uint offset);
  bool   irq_read_pos_match(uint offset);
  bool   irq_pos_valid();
  bool   nmi_test();
  bool   irq_test();
