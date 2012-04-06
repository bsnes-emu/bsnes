struct APU : Thread, MMIO {
  #include "registers.hpp"

  static void Enter();
  void enter();
  void step(unsigned clocks);

  uint8 read(uint32 addr);
  void write(uint32 addr, uint8 byte);
  void power();

  void runsequencer();
};

extern APU apu;
