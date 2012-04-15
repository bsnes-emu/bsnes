struct CPU : Processor::ARM, Thread, MMIO {
  StaticMemory iwram;
  StaticMemory ewram;
  #include "registers.hpp"
  #include "state.hpp"

  static void Enter();
  void enter();
  void step(unsigned clocks);

  void bus_idle(uint32 addr);
  uint32 bus_read(uint32 addr, uint32 size);
  void bus_write(uint32 addr, uint32 size, uint32 word);

  void power();

  uint8 read(uint32 addr);
  void write(uint32 addr, uint8 byte);

  void dma_run();
  void dma_transfer(Registers::DMA &dma);

  void timer_step(unsigned clocks);
  void timer_increment(unsigned n);

  CPU();
};

extern CPU cpu;
