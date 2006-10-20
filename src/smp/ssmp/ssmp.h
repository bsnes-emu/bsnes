class sSMP : public SMP {
public:
  void enter();

public:
#include "core/core.h"
#include "memory/memory.h"
#include "timing/timing.h"

struct {
  uint8  opcode;
  bool   in_opcode;

//timing
  uint32 clock_counter;
  uint32 dsp_counter;

//$00f0
  uint8  clock_speed;
  bool   mmio_disabled;
  bool   ram_writable;

//$00f1
  bool   iplrom_enabled;

//$00f2
  uint8  dsp_addr;
} status;

//ssmp.cpp
  void   power();
  void   reset();

  sSMP();
  ~sSMP();
};
