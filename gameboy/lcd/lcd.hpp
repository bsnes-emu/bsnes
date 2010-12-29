struct LCD : Processor, MMIO {
  #include "mmio/mmio.hpp"

  struct Status {
    unsigned ly;
  } status;

  uint8 vram[8192];
  uint8 oam[160];

  void power();
  void reset();
};

extern LCD lcd;
