class DSP4 : public Memory {
public:
  void init();
  void enable();
  void power();
  void reset();

  uint8 read (uint addr);
  void  write(uint addr, uint8 data);
};

extern DSP4 dsp4;
