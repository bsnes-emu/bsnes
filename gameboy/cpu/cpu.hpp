struct CPU : Processor, MMIO {
  #include "core/core.hpp"
  #include "mmio/mmio.hpp"
  #include "timing/timing.hpp"

  struct Status {
    bool ime;
  } status;

  uint8 wram[8192];
  uint8 hram[128];

  static void Main();
  void main();
  void power();
  void reset();
  CPU();
};

extern  CPU cpu;
