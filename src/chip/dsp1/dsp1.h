#include "dsp1emu.h"

class DSP1 : public Memory {
private:
  Dsp1 dsp1;
  bool addr_decode(uint16 addr);

public:
  void init();
  void enable();
  void power();
  void reset();

  uint8 read(uint addr);
  void write(uint addr, uint8 data);
};

extern DSP1 dsp1;
