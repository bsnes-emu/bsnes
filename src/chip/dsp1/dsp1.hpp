#include "dsp1emu.hpp"

class DSP1 : public Memory {
private:
  Dsp1 dsp1;
  bool addr_decode(uint16 addr);

public:
  void init();
  void enable();
  void power();
  void reset();

  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);
};

extern DSP1 dsp1;
