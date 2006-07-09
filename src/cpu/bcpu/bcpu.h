class bCPU : public CPU {
public:
#include "core/core.h"
#include "memory/memory.h"
#include "dma/dma.h"
#include "timing/timing.h"

enum { NTSC = 0, PAL = 1 };
uint8 region;

enum {
  DMASTATE_DMASYNC,
  DMASTATE_DMASYNC2,
  DMASTATE_DMASYNC3,
  DMASTATE_RUN,
  DMASTATE_CPUSYNC,

  HDMASTATE_IDMASYNC,
  HDMASTATE_IDMASYNC2,
  HDMASTATE_IDMASYNC3,
  HDMASTATE_ICPUSYNC,

  HDMASTATE_DMASYNC,
  HDMASTATE_DMASYNC2,
  HDMASTATE_DMASYNC3,
  HDMASTATE_RUN,
  HDMASTATE_CPUSYNC
};

struct {
  bool hdma;
  bool dma;
  bool irq;
  bool stp;
  bool wai;
} run_state;

struct {
  uint8  cycle_pos, cycle_count;
  uint8  opcode;
  uint32 cycles_executed;

  uint8  dma_state, hdma_state;
  uint32 dma_cycle_count, hdma_cycle_count;

//$4207-$420a
  uint16 virq_trigger, hirq_trigger;

//$2181-$2183
  uint32 wram_addr;

//$4016-$4017
  bool   joypad_strobe_latch;
  uint8  joypad1_read_pos, joypad2_read_pos;

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

//$4214-$4216
  uint16 r4214;
  uint16 r4216;
} status;

  inline bool   hdma_test();
  inline bool   nmi_test();
  inline bool   irq_test();

  inline uint8  pio_status();
  inline void   run();
  inline uint32 clocks_executed();
  inline void   scanline();
  inline void   frame();
  inline void   power();
  inline void   reset();

  inline void   irq_run();

//mmio commands
  void  mmio_reset();
  uint8 mmio_r2180();
  uint8 mmio_r4016();
  uint8 mmio_r4017();
  uint8 mmio_r4210();
  uint8 mmio_r4211();
  uint8 mmio_r4212();
  uint8 mmio_r4213();
  uint8 mmio_r4214();
  uint8 mmio_r4215();
  uint8 mmio_r4216();
  uint8 mmio_r4217();
  uint8 mmio_r4218();
  uint8 mmio_r4219();
  uint8 mmio_r421a();
  uint8 mmio_r421b();
  uint8 mmio_r43x0(uint8 i);
  uint8 mmio_r43x1(uint8 i);
  uint8 mmio_r43x2(uint8 i);
  uint8 mmio_r43x3(uint8 i);
  uint8 mmio_r43x4(uint8 i);
  uint8 mmio_r43x5(uint8 i);
  uint8 mmio_r43x6(uint8 i);
  uint8 mmio_r43x7(uint8 i);
  uint8 mmio_r43x8(uint8 i);
  uint8 mmio_r43x9(uint8 i);
  uint8 mmio_r43xa(uint8 i);
  uint8 mmio_r43xb(uint8 i);
  void  mmio_w2180(uint8 value);
  void  mmio_w2181(uint8 value);
  void  mmio_w2182(uint8 value);
  void  mmio_w2183(uint8 value);
  void  mmio_w4016(uint8 value);
  void  mmio_w4200(uint8 value);
  void  mmio_w4201(uint8 value);
  void  mmio_w4202(uint8 value);
  void  mmio_w4203(uint8 value);
  void  mmio_w4204(uint8 value);
  void  mmio_w4205(uint8 value);
  void  mmio_w4206(uint8 value);
  void  mmio_w4207(uint8 value);
  void  mmio_w4208(uint8 value);
  void  mmio_w4209(uint8 value);
  void  mmio_w420a(uint8 value);
  void  mmio_w420b(uint8 value);
  void  mmio_w420c(uint8 value);
  void  mmio_w420d(uint8 value);
  void  mmio_w43x0(uint8 value, uint8 i);
  void  mmio_w43x1(uint8 value, uint8 i);
  void  mmio_w43x2(uint8 value, uint8 i);
  void  mmio_w43x3(uint8 value, uint8 i);
  void  mmio_w43x4(uint8 value, uint8 i);
  void  mmio_w43x5(uint8 value, uint8 i);
  void  mmio_w43x6(uint8 value, uint8 i);
  void  mmio_w43x7(uint8 value, uint8 i);
  void  mmio_w43x8(uint8 value, uint8 i);
  void  mmio_w43x9(uint8 value, uint8 i);
  void  mmio_w43xa(uint8 value, uint8 i);
  void  mmio_w43xb(uint8 value, uint8 i);

  uint8 mmio_read (uint16 addr);
  void  mmio_write(uint16 addr, uint8 data);

enum { CYCLE_OPREAD, CYCLE_READ, CYCLE_WRITE, CYCLE_IO };
  inline void pre_exec_cycle();
  inline void exec_hdma();
  inline void exec_dma();
  inline void exec_cycle();
  inline void last_cycle();
  inline bool in_opcode();

  bCPU();
  ~bCPU();
};
