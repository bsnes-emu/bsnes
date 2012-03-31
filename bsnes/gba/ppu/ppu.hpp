struct PPU : Thread, Memory {
  StaticMemory vram;
  StaticMemory oam;
  StaticMemory pram;
  #include "registers.hpp"
  uint16 *output;

  static void Enter();
  void enter();
  void step(unsigned clocks);

  void power();
  void scanline();
  void frame();

  uint32 read(uint32 addr, uint32 size);
  void write(uint32 addr, uint32 size, uint32 word);

  PPU();
  ~PPU();
};

extern PPU ppu;
