struct PPU : Thread, MMIO {
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

  uint8 read(uint32 addr);
  void write(uint32 addr, uint8 byte);

  PPU();
  ~PPU();
};

extern PPU ppu;
