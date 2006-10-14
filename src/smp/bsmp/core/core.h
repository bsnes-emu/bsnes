void (bSMP::*optbl[256])();
uint16 dp, sp, rd, wr, bit, ya;
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

  inline void   exec();
  inline bool   in_opcode();
  inline void   init_op_table();

#include "op.h"
