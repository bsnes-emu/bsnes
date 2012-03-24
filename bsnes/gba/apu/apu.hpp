struct APU : Thread, Memory {
  static void Enter();
  void enter();
  void step(unsigned clocks);

  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);
  void power();
};

extern APU apu;
