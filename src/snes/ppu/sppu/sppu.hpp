class sPPU : public PPU {
public:
  #include "background/background.hpp"
  #include "mmio/mmio.hpp"
  #include "screen/screen.hpp"
  #include "sprite/sprite.hpp"
  #include "window/window.hpp"

  Background bg1;
  Background bg2;
  Background bg3;
  Background bg4;
  Sprite oam;
  Window window;
  Screen screen;

  void enter();
  void add_clocks(unsigned);

  void power();
  void reset();

  void scanline();
  void frame();

  sPPU();
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
