class sAPU : public APU {
public:
#include "core/core.h"
#include "memory/memory.h"
#include "timing/timing.h"

thread_t thread;

struct {
  uint8  opcode;
  bool   in_opcode;

//timing
  uint32 clocks_executed;

//$f1
  bool   iplrom_enabled;

//$f2
  uint8  dsp_addr;
} status;

//sapu.cpp
  void   main();

  void   run();
  void   power();
  void   reset();

  sAPU();
  ~sAPU();
};
