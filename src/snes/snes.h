#include "interface/interface.h"
#include "scheduler/scheduler.h"
#include "tracer/tracer.h"

class VideoFilter;

class SNES {
protected:
uint8  snes_region;

public:
enum { NTSC = 0, PAL = 1 };

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
  uint8  region();
  void   set_region(uint8 new_region);

#include "video/video.h"
#include "audio/audio.h"
#include "input/input.h"

  SNES();
  virtual ~SNES() {}
};

extern SNES snes;

#include "video/filter.h"
