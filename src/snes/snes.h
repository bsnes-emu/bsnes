#include "interface/interface.h"
#include "scheduler/scheduler.h"
#include "tracer/tracer.h"

class VideoFilter;

class SNES {
public:
  enum Region { NTSC = 0, PAL = 1 };

  //system functions
  virtual void run();
  virtual void runtoframe();

  virtual void init();
  virtual void term();
  virtual void power();
  virtual void reset();

  virtual void frame();
  virtual void scanline();

  //PAL/NTSC
  Region region();
  void set_region(Region);

  #include "video/video.h"
  #include "audio/audio.h"
  #include "input/input.h"

  SNES();
  virtual ~SNES() {}

private:
  Region snes_region;
};

extern SNES snes;
