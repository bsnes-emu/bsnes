#include "interface/interface.hpp"
#include "scheduler/scheduler.hpp"
#include "tracer/tracer.hpp"

class VideoFilter;

class SNES {
public:
  enum Region { NTSC = 0, PAL = 1 };
  enum RegionAutodetect { Autodetect = 2 };
  enum ExpansionPortDevice { ExpansionNone = 0, ExpansionBSX = 1 };

  //system functions
  virtual void run();
  virtual void runtoframe();

  virtual void init();
  virtual void term();
  virtual void power();
  virtual void reset();

  virtual void frame();
  virtual void scanline();

  //return *active* region / expansion port device information
  //settings cached upon power-on
  Region region();
  ExpansionPortDevice expansion();

  #include "video/video.hpp"
  #include "audio/audio.hpp"
  #include "input/input.hpp"

  SNES();
  virtual ~SNES() {}

private:
  unsigned snes_region;
  unsigned snes_expansion;
};

extern SNES snes;
