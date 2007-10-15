class DSP3 { public:
  void init();
  void enable();
  void power();
  void reset();

  uint8 read (uint16 addr);
  void  write(uint16 addr, uint8 data);
};

extern DSP3 dsp3;
