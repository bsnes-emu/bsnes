class bAPU;

class bAPUTimer {
public:
uint8 cycle_frequency, target;
uint8 stage1_ticks, stage2_ticks, stage3_ticks;
bool  enabled;
  inline void add_cycles(int cycles);
};

class bAPU : public APU {
private:
typedef void (bAPU::*op)();
op optbl[256];

public:
uint16 dp, sp, rd, wr, bit, ya;

struct {
  uint8  cycle_pos, opcode;
  uint32 cycles_executed;

//$f1
  bool   iplrom_enabled;

//$f2
  uint8  dsp_addr;
}status;

bAPUTimer t0, t1, t2;

uint8 *spcram;
  inline uint8  spcram_read (uint16 addr);
  inline void   spcram_write(uint16 addr, uint8 value);
  inline uint8  port_read (uint8 port);
  inline void   port_write(uint8 port,  uint8 value);

  inline uint8 *get_spcram_handle();
  inline void   run();
  inline uint32 cycles_executed();
  inline void   power();
  inline void   reset();

  inline void   add_cycles(int cycles);

enum {
  OPMODE_ADDR = 0,
  OPMODE_DP = 1
};
  inline uint8 op_read();
  inline uint8 op_read (uint8 mode, uint16 addr);
  inline void  op_write(uint8 mode, uint16 addr, uint8 value);
  inline uint8 stack_read();
  inline void  stack_write(uint8 value);

  inline void exec_cycle();
  inline bool in_opcode();
  inline void init_op_table();

  inline uint8  op_adc (uint8  x, uint8  y);
  inline uint16 op_addw(uint16 x, uint16 y);
  inline uint8  op_and (uint8  x, uint8  y);
  inline uint8  op_cmp (uint8  x, uint8  y);
  inline uint16 op_cmpw(uint16 x, uint16 y);
  inline uint8  op_eor (uint8  x, uint8  y);
  inline uint8  op_inc (uint8  x);
  inline uint16 op_incw(uint16 x);
  inline uint8  op_dec (uint8  x);
  inline uint16 op_decw(uint16 x);
  inline uint8  op_or  (uint8  x, uint8  y);
  inline uint8  op_sbc (uint8  x, uint8  y);
  inline uint16 op_subw(uint16 x, uint16 y);
  inline uint8  op_asl (uint8  x);
  inline uint8  op_lsr (uint8  x);
  inline uint8  op_rol (uint8  x);
  inline uint8  op_ror (uint8  x);
#include "bapu_op.h"

  bAPU();
  ~bAPU();
};
