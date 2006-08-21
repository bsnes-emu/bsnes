class VideoFilter;

class SNES {
protected:
bool   is_debugger_enabled;
uint8  snes_region;

//CPU<>APU synchronization
struct {
  int64  counter, dsp_counter;
  int64  cpu_freq, apu_freq;
  int64  cpu_multbl[1024], apu_multbl[1024];
} sync;

  void   update_timing();

public:
enum { NTSC = 0, PAL = 1 };

//system functions
  virtual inline void run();
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

//debugging functions
enum {
  NO_ACTION = 0,
  CPU_EXEC_OPCODE_BEGIN, CPU_EXEC_OPCODE_END,
  APU_EXEC_OPCODE_BEGIN, APU_EXEC_OPCODE_END,
  MEM_READ,    MEM_WRITE,
  SPCRAM_READ, SPCRAM_WRITE,
  VRAM_READ,   VRAM_WRITE,
  OAM_READ,    OAM_WRITE,
  CGRAM_READ,  CGRAM_WRITE,
};
  virtual void debugger_enable();
  virtual void debugger_disable();
  virtual bool debugger_enabled();

//message functions
  virtual void notify(uint32 message, uint32 param1 = 0, uint32 param2 = 0);

  SNES();
  virtual ~SNES() {}
};

#include "video/filter.h"
