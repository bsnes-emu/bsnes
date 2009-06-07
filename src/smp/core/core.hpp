class SMPcore {
public:
  #include "registers.hpp"
  #include "memory.hpp"
  #include "opcode_headers.hpp"
  #include "disasm/disasm.hpp"

  regs_t regs;
  uint16 dp, sp, rd, wr, bit, ya;

  virtual void op_io() = 0;
  virtual uint8_t op_read(uint16_t addr) = 0;
  virtual void op_write(uint16_t addr, uint8_t data) = 0;

  uint8  op_adc (uint8  x, uint8  y);
  uint16 op_addw(uint16 x, uint16 y);
  uint8  op_and (uint8  x, uint8  y);
  uint8  op_cmp (uint8  x, uint8  y);
  uint16 op_cmpw(uint16 x, uint16 y);
  uint8  op_eor (uint8  x, uint8  y);
  uint8  op_inc (uint8  x);
  uint8  op_dec (uint8  x);
  uint8  op_or  (uint8  x, uint8  y);
  uint8  op_sbc (uint8  x, uint8  y);
  uint16 op_subw(uint16 x, uint16 y);
  uint8  op_asl (uint8  x);
  uint8  op_lsr (uint8  x);
  uint8  op_rol (uint8  x);
  uint8  op_ror (uint8  x);

  void (SMPcore::*opcode_table[256])();
  void initialize_opcode_table();

  SMPcore();
};
