struct PPU : Processor {
  static void Enter();
  void enter();
  void step(unsigned clocks);

  void power();
};

extern PPU ppu;
