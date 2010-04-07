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

  struct {
    bool interlace;
  } display;

  void enter();
  void add_clocks(unsigned);

  void power();
  void reset();

  void scanline();
  void frame();

  sPPU();
};

#if defined(DEBUGGER)
  #include "debugger/debugger.hpp"
  extern sPPUDebugger ppu;
#else
  extern sPPU ppu;
#endif
