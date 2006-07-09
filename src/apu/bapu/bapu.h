class bAPU : public APU {
public:
#include "core/core.h"
#include "memory/memory.h"
#include "timing/timing.h"

struct {
  uint8  cycle_pos, opcode;
  uint32 clocks_executed;

//$f1
  bool   iplrom_enabled;

//$f2
  uint8  dsp_addr;
} status;

  inline void run();
  inline void power();
  inline void reset();

  bAPU();
  ~bAPU();
};
