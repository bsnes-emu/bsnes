#include "counter/counter.hpp"

class PPU : public Processor, public PPUCounter, public MMIO {
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

  uint16 *surface;
  uint16 *output;

  struct {
    bool frames_updated;
    unsigned frames_executed;
  } status;

  uint8 ppu1_version;
  uint8 ppu2_version;

  struct {
    bool interlace;
    bool overscan;
  } display;

  //synchronization
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_cpu();

  static void Enter();
  void enter();
  void add_clocks(unsigned);

  void power();
  void reset();

  void scanline();
  void frame();

  void serialize(serializer&);
  PPU();
  ~PPU();
};

#if defined(DEBUGGER)
  #include "debugger/debugger.hpp"
  extern PPUDebugger ppu;
#else
  extern PPU ppu;
#endif
