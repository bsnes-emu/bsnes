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
  bool   enabled;
  uint   irq_delay;
  uint   irq_fire;
  uint   nmi_fire;
  uint   hw_math;
} counter;

enum {
  DMASTATE_INACTIVE,
  DMASTATE_DMASYNC,
  DMASTATE_RUN,
  DMASTATE_CPUSYNC,
};

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
  uint16 vblstart;

  bool   line_rendered;
  uint16 line_render_position;

  bool   dram_refreshed;
  uint16 dram_refresh_position;

  bool   hdmainit_triggered;
  uint16 hdmainit_trigger_position;

  bool   hdma_triggered;

  uint16 irq_delay;

  uint16 nmi_trigger_pos;
  bool   nmi_read, nmi_line, nmi_transition;
  bool   nmi_pending;

  uint16 virq_trigger_pos, hirq_trigger_pos;
  bool   irq_read, irq_line, irq_transition;
  bool   irq_lock, irq_pending;

//dma
  uint   dma_counter;
  uint   dma_clocks;
  uint   dma_state;
  bool   dma_pending;
  bool   hdma_pending;
  bool   hdmainit_pending;

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
