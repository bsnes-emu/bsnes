class sCPU : public CPU {
public:
#include "core/core.h"
#include "dma/dma.h"
#include "memory/memory.h"
#include "mmio/mmio.h"
#include "timing/timing.h"

thread_t thread;

struct {
  bool   wai;
  bool   irq;
  uint16 irq_vector;
} event;

struct {
//core
  uint8  opcode;
  bool   in_opcode;

  uint   clock_count;
  uint   clocks_executed;

//timing
  bool   region;
  uint16 region_scanlines;
  uint16 vcounter, hcounter, hclock;
  bool   interlace, interlace_field;
  bool   overscan;
  uint16 field_lines, line_clocks;

  bool   line_rendered;
  bool   dram_refreshed;
  bool   hdmainit_triggered;
  bool   hdma_triggered;

  uint16 irq_delay;

  int16  nmi_read_pos, nmi_line_pos;
  bool   nmi_read, nmi_line, nmi_transition;
  bool   nmi_pending;

  int16  irq_read_pos, irq_line_pos;
  bool   irq_read, irq_line, irq_transition;
  bool   irq_pending;

//mmio

//$2181-$2183
  uint32 wram_addr;

//$4016-$4017
  bool   joypad_strobe_latch;
  uint32 joypad1_bits;
  uint32 joypad2_bits;

//$4200
  bool   nmi_enabled;
  bool   hirq_enabled, virq_enabled;
  bool   auto_joypad_poll;

//$4201
  uint8  pio;

//$4202-$4203
  uint8  mul_a, mul_b;

//$4204-$4206
  uint16 div_a;
  uint8  div_b;

//$4207-$420a
  uint16 hirq_pos, virq_pos;

//$4214-$4217
  uint16 r4214;
  uint16 r4216;

//$4218-$421f
  uint8  joy1l, joy1h;
  uint8  joy2l, joy2h;
  uint8  joy3l, joy3h;
  uint8  joy4l, joy4h;
} status;

  void   run();
  void   power();
  void   reset();

  sCPU();
  ~sCPU();
};
