struct PPU : Processor {
  StaticMemory vram;
  StaticMemory oam;
  StaticMemory pram;

  static void Enter();
  void enter();
  void step(unsigned clocks);

  void power();
  void frame();

  PPU();
};

extern PPU ppu;
