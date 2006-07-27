struct {
  uint16 v, hc;

  bool   interlace, interlace_field, overscan;
  uint16 line_cycles, frame_lines;

  bool   line_rendered;

  bool   dram_refreshed;
  uint16 dram_refresh_pos;

  uint8  dma_counter;

  uint16 hdmainit_trigger_pos;
  bool   hdmainit_triggered;

  bool   hdma_triggered;

  uint16 region_scanlines;

//nmi_pending, irq_pending are used by last_cycle()
//nmi_line = /NMI, nmi_read = $4210.7
//irq_line = /IRQ, irq_read = $4211.7
  bool   nmi_pending, nmi_line, nmi_read;
  bool   irq_pending, irq_line, irq_read;

//NMI is edge-sensitive, meaning it triggers when /NMI
//transitions from high to low. This value is set to 1
//when that happens, and cleared after the nmi_test()
//routine acknowledges it and invokes the NMI interrupt
  bool   nmi_transition;

//IRQ is level-sensitive, so it does not need to keep
//track of transitions from high to low. IRQs will
//continue to fire as long as /IRQ stays low.
//However, if a write to $4200 forces IRQs high at the
//exact same clock cycle that /IRQ goes low, the /IRQ
//will still occur. Hence the need for this variable.
  bool   irq_transition;

//position is relative to time.hc, set at start of each scanline
//-64 means no trigger point on this scanline
//$4210/$4211 status bits get set before /NMI and /IRQ go low,
//hence the need for two variables for each.
  int32  nmi_read_trigger_pos, nmi_line_trigger_pos;
  int32  irq_read_trigger_pos, irq_line_trigger_pos;
} time;

  inline uint16 vcounter();
  inline uint16 hcounter();
  inline uint16 hcycles();
  inline bool   interlace();
  inline bool   interlace_field();
  inline bool   overscan();
  inline uint16 region_scanlines();

  inline bool   nmi_trigger_pos_match(uint32 offset);
  inline bool   irq_trigger_pos_match(uint32 offset);

  inline void   update_nmi();
  inline void   update_irq();
  inline void   update_interrupts();
  inline void   poll_interrupts(int cycles);

  inline void   set_interlace(bool r);
  inline void   set_overscan (bool r);

  inline uint8  dma_counter();

  inline void   cycle_edge();
  inline void   add_cycles(int cycles);
  inline void   scanline();
  inline void   frame();
  inline void   time_reset();
