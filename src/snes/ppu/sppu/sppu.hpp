class sPPU : public PPU {
public:
  #include "mmio/mmio.hpp"

  void enter();
  void add_clocks(unsigned);

  void power();
  void reset();

  void scanline();
  void frame();

  sPPU();

private:
  uint16 light_table[16][32768];
};

#if !defined(DEBUGGER)
  extern sPPU ppu;
#else
class sPPUDebugger : public sPPU, public PPUDebugger {
public:
  bool bg1_enabled[2];
  bool bg2_enabled[2];
  bool bg3_enabled[2];
  bool bg4_enabled[2];
  bool oam_enabled[4];

  inline bool interlace() { return false; }
  inline bool overscan() { return false; }
};
extern sPPUDebugger ppu;
#endif
