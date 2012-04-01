struct CPU : Processor::ARM, Thread, MMIO {
  StaticMemory iwram;
  StaticMemory ewram;
  #include "registers.hpp"

  static void Enter();
  void enter();
  void step(unsigned clocks);
  uint32 bus_read(uint32 addr, uint32 size);
  void bus_write(uint32 addr, uint32 size, uint32 word);

  void power();

  uint8 read(uint32 addr);
  void write(uint32 addr, uint8 byte);

  CPU();
};

extern CPU cpu;
