class bCPU;

class bCPUMMIO : public MMIO {
public:
bCPU *cpu;
  inline uint8 read (uint32 addr);
  inline void  write(uint32 addr, uint8 value);
  bCPUMMIO(bCPU *_cpu);
};

class bCPU : public CPU {
private:
void (bCPU::*optbl[256])();

enum { NTSC = 0, PAL = 1 };
uint8 region;

public:
#include "bcpu_timing.h"

uint8 apu_port[4];
  inline uint8 port_read (uint8 port);
  inline void  port_write(uint8 port, uint8 value);

enum {
  OPMODE_ADDR = 1, OPMODE_LONG = 2,
  OPMODE_DBR  = 3, OPMODE_PBR  = 4,
  OPMODE_DP   = 5, OPMODE_SP   = 6
};

CPUReg24 aa, rd;
uint8    dp, sp;

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
  uint8  joypad1_strobe_value, joypad2_strobe_value;
  uint8  joypad1_read_pos,     joypad2_read_pos;

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

//$43x0.d7
enum {
  DMA_CPUTOMMIO = 0,
  DMA_MMIOTOCPU = 1
};

struct {
  uint32 read_index; //set to 0 at beginning of DMA/HDMA

//$420b
  bool   active;
//$420c
  bool   hdma_enabled;
//$43x0
  uint8  dmap;
  bool   direction;
  bool   hdma_indirect;
  int8   incmode;
  bool   fixedxfer;
  uint8  xfermode;
//$43x1
  uint8  destaddr;
//$43x2-$43x3
  uint16 srcaddr;
//$43x4
  uint8  srcbank;
//$43x5-$43x6
  union {
    uint16 xfersize;
    uint16 hdma_iaddr;
  };
//$43x7
  uint8  hdma_ibank;
//$43x8-$43x9
  uint16 hdma_addr;
//$43xa
  uint8  hdma_line_counter;
//$43xb/$43xf
  uint8  hdma_unknown;

//hdma-specific
  bool   hdma_active;
  bool   hdma_do_transfer;
} channel[8];

  inline bool   hdma_test();
  inline bool   nmi_test();
  inline bool   irq_test();

  inline uint8  pio_status();
  inline void   run();
  inline uint32 cycles_executed();
  inline void   scanline();
  inline void   frame();
  inline void   power();
  inline void   reset();

  inline void   irq_run();

//dma commands
  inline void   dma_add_cycles(uint32 cycles);
  inline void   hdma_add_cycles(uint32 cycles);
  inline void   dma_run();
  inline void   hdma_run();
  inline void   hdma_update(uint8 i);
  inline uint8  hdma_enabled_channels();
  inline uint8  hdma_active_channels();
  inline void   hdmainit_activate();
  inline void   hdma_activate();
  inline void   dma_cputommio(uint8 i, uint8 index);
  inline void   dma_mmiotocpu(uint8 i, uint8 index);
  inline void   dma_write(uint8 i, uint8 index);
  inline uint32 dma_addr(uint8 i);
  inline uint32 hdma_addr(uint8 i);
  inline uint32 hdma_iaddr(uint8 i);
  inline uint16 hdma_mmio(uint8 i);
  inline uint8  hdma_read(uint8 i);
  inline void   hdma_write(uint8 i, uint8 x);
  inline void   dma_reset();

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
  void  mmio_w4017(uint8 value);
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

enum { CYCLE_OPREAD = 0, CYCLE_READ, CYCLE_WRITE, CYCLE_IO };
  inline void pre_exec_cycle();
  inline void exec_hdma();
  inline void exec_dma();
  inline void exec_cycle();
  inline void last_cycle();
  inline bool in_opcode();

//cpu extra-cycle conditions
  inline void cpu_c2();
  inline void cpu_c4(uint16 x, uint16 y);
  inline void cpu_c6(uint16 addr);
  inline void cpu_io();

  inline uint8  mem_read (uint32 addr);
  inline void   mem_write(uint32 addr, uint8 value);
  inline uint32 op_addr(uint8 mode, uint32 addr);
  inline uint8  op_read();
  inline uint8  op_read(uint8 mode, uint32 addr);
  inline void   op_write(uint8 mode, uint32 addr, uint8 value);
  inline uint8  stack_read();
  inline void   stack_write(uint8 value);

//opcode functions
  inline void init_op_tables();

//op_read
  inline void op_adc_b();
  inline void op_adc_w();
  inline void op_and_b();
  inline void op_and_w();
  inline void op_bit_b();
  inline void op_bit_w();
  inline void op_cmp_b();
  inline void op_cmp_w();
  inline void op_cpx_b();
  inline void op_cpx_w();
  inline void op_cpy_b();
  inline void op_cpy_w();
  inline void op_eor_b();
  inline void op_eor_w();
  inline void op_lda_b();
  inline void op_lda_w();
  inline void op_ldx_b();
  inline void op_ldx_w();
  inline void op_ldy_b();
  inline void op_ldy_w();
  inline void op_ora_b();
  inline void op_ora_w();
  inline void op_sbc_b();
  inline void op_sbc_w();
//op_rmw
  inline void op_inc_b();
  inline void op_inc_w();
  inline void op_dec_b();
  inline void op_dec_w();
  inline void op_asl_b();
  inline void op_asl_w();
  inline void op_lsr_b();
  inline void op_lsr_w();
  inline void op_rol_b();
  inline void op_rol_w();
  inline void op_ror_b();
  inline void op_ror_w();
  inline void op_trb_b();
  inline void op_trb_w();
  inline void op_tsb_b();
  inline void op_tsb_w();
#include "bcpu_op.h"

  bCPU();
  ~bCPU();
};
