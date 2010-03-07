struct SMPDebugger : ChipDebugger {
  bool property(unsigned id, string &name, string &value);

  //$00f0
  virtual unsigned clock_speed() { return 0; }
  virtual bool mmio_disable() { return 0; }
  virtual bool ram_writable() { return 0; }

  //$00f1
  virtual bool iplrom_enable() { return 0; }

  //$00f2
  virtual unsigned dsp_address() { return 0; }
};
