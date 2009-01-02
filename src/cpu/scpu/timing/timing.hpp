  enum {
    EventNone,
    EventIrqLockRelease,
    EventAluLockRelease,
    EventDramRefresh,
    EventHdmaInit,
    EventHdmaRun,

    //cycle edge
    EventFlagHdmaInit = 1 << 0,
    EventFlagHdmaRun  = 1 << 1,
  };

  //timing.cpp
  unsigned dma_counter();

  void add_clocks(unsigned clocks);
  void scanline();

  alwaysinline void precycle_edge();
  alwaysinline void cycle_edge();
  void last_cycle();

  void timing_power();
  void timing_reset();

  //irq.cpp
  void update_interrupts();
  void poll_interrupts();
  void nmitimen_update(uint8 data);
  void hvtime_update(uint16 addr);
  bool rdnmi();
  bool timeup();

  bool nmi_test();
  bool irq_test();

  //joypad.cpp
  void run_auto_joypad_poll();

  //event.cpp
  void queue_event(unsigned);  //deltaqueue callback function
