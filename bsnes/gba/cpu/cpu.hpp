struct CPU : Processor::ARM, Thread {
  StaticMemory iram;
  StaticMemory eram;

  static void Enter();
  void enter();
  void step(unsigned clocks);
  uint32 bus_read(uint32 addr, uint32 size);
  void bus_write(uint32 addr, uint32 size, uint32 word);

  void power();

  CPU();
};

extern CPU cpu;
