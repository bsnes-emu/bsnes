struct CPUDebugger : ChipDebugger {
  bool property(unsigned id, string &name, string &value);

  //internal
  virtual unsigned mdr() { return 0; }

  //$2181-2183
  virtual unsigned wram_address() { return 0; }

  //$4016
  virtual bool joypad_strobe_latch() { return 0; }

  //$4200
  virtual bool nmi_enable() { return 0; }
  virtual bool hirq_enable() { return 0; }
  virtual bool virq_enable() { return 0; }
  virtual bool auto_joypad_poll() { return 0; }

  //$4201
  virtual unsigned pio_bits() { return 0; }

  //$4202
  virtual unsigned multiplicand() { return 0; }

  //$4203
  virtual unsigned multiplier() { return 0; }

  //$4204-$4205
  virtual unsigned dividend() { return 0; }

  //$4206
  virtual unsigned divisor() { return 0; }

  //$4207-$4208
  virtual unsigned htime() { return 0; }

  //$4209-$420a
  virtual unsigned vtime() { return 0; }

  //$420b
  virtual unsigned dma_enable() { return 0; }

  //$420c
  virtual unsigned hdma_enable() { return 0; }

  //$420d
  virtual bool fastrom_enable() { return 0; }

  //$43x0
  virtual bool dma_direction(unsigned) { return 0; }
  virtual bool dma_indirect(unsigned) { return 0; }
  virtual bool dma_reverse_transfer(unsigned) { return 0; }
  virtual bool dma_fixed_transfer(unsigned) { return 0; }
  virtual unsigned dma_transfer_mode(unsigned) { return 0; }

  //$43x1
  virtual unsigned dma_bbus_address(unsigned) { return 0; }

  //$43x2-$43x3
  virtual unsigned dma_abus_address(unsigned) { return 0; }

  //$43x4
  virtual unsigned dma_abus_bank(unsigned) { return 0; }

  //$43x5-$43x6
  virtual unsigned dma_transfer_size(unsigned) { return 0; }

  //$43x7
  virtual unsigned dma_indirect_bank(unsigned) { return 0; }

  //$43x8-$43x9
  virtual unsigned dma_table_address(unsigned addr) { return 0; }

  //$43xa
  virtual unsigned dma_line_counter(unsigned addr) { return 0; }
};
