#include "dsp1emu.h"

class DSP1 {
private:
Dsp1 dsp1;

public:
  void   init();
  void   enable();
  void   power();
  void   reset();

  bool   addr_decode(uint16 addr);
  uint8  read (uint16 addr);
  void   write(uint16 addr, uint8 data);
};

extern DSP1 dsp1;
