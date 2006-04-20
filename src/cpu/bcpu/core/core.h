/* External functions:
 *   void   last_cycle();
 *   void   cpu_io();
 *   uint8  mem_read(uint32 addr);
 *   void   mem_write(uint32 addr, uint8 value);
 */

void (bCPU::*optbl[256])();

CPUReg24 aa, rd;
uint8    dp, sp;

//op_read
  inline void   op_adc_b();
  inline void   op_adc_w();
  inline void   op_and_b();
  inline void   op_and_w();
  inline void   op_bit_b();
  inline void   op_bit_w();
  inline void   op_cmp_b();
  inline void   op_cmp_w();
  inline void   op_cpx_b();
  inline void   op_cpx_w();
  inline void   op_cpy_b();
  inline void   op_cpy_w();
  inline void   op_eor_b();
  inline void   op_eor_w();
  inline void   op_lda_b();
  inline void   op_lda_w();
  inline void   op_ldx_b();
  inline void   op_ldx_w();
  inline void   op_ldy_b();
  inline void   op_ldy_w();
  inline void   op_ora_b();
  inline void   op_ora_w();
  inline void   op_sbc_b();
  inline void   op_sbc_w();
//op_rmw
  inline void   op_inc_b();
  inline void   op_inc_w();
  inline void   op_dec_b();
  inline void   op_dec_w();
  inline void   op_asl_b();
  inline void   op_asl_w();
  inline void   op_lsr_b();
  inline void   op_lsr_w();
  inline void   op_rol_b();
  inline void   op_rol_w();
  inline void   op_ror_b();
  inline void   op_ror_w();
  inline void   op_trb_b();
  inline void   op_trb_w();
  inline void   op_tsb_b();
  inline void   op_tsb_w();

  inline void   cpu_c2();
  inline void   cpu_c4(uint16 x, uint16 y);
  inline void   cpu_c6(uint16 addr);

enum {
  OPMODE_ADDR, OPMODE_LONG,
  OPMODE_DBR,  OPMODE_PBR,
  OPMODE_DP,   OPMODE_SP
};
  inline uint32 op_addr(uint8 mode, uint32 addr);
  inline uint8  op_read();
  inline uint8  op_read(uint8 mode, uint32 addr);
  inline void   op_write(uint8 mode, uint32 addr, uint8 value);
  inline uint8  stack_read();
  inline void   stack_write(uint8 value);

  inline void   init_op_tables();

#include "op.h"
