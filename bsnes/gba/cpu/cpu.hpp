struct CPU : Processor::ARM, Thread, Memory {
  StaticMemory iwram;
  StaticMemory ewram;

  static void Enter();
  void enter();
  void step(unsigned clocks);
  uint32 bus_read(uint32 addr, uint32 size);
  void bus_write(uint32 addr, uint32 size, uint32 word);

  void power();

  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);

  CPU();
};

extern CPU cpu;
