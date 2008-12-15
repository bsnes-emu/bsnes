class DSP3 : public Memory {
public:
  void init();
  void enable();
  void power();
  void reset();

  uint8 read (uint addr);
  void  write(uint addr, uint8 data);
};

extern DSP3 dsp3;
