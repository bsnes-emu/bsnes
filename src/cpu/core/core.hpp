class CPUcore {
public:
  #include "registers.hpp"
  #include "memory.hpp"
  #include "opcode_headers.hpp"
  #include "disasm/disasm.hpp"

  regs_t regs;
  reg24_t aa, rd;
  uint8_t sp, dp;

  virtual void op_io() = 0;
  virtual uint8_t op_read(uint32_t addr) = 0;
  virtual void op_write(uint32_t addr, uint8_t data) = 0;
  virtual void last_cycle() = 0;
  virtual bool interrupt_pending() = 0;

  void op_io_irq();
  void op_io_cond2();
  void op_io_cond4(uint16 x, uint16 y);
  void op_io_cond6(uint16 addr);

  void op_adc_b();
  void op_adc_w();
  void op_and_b();
  void op_and_w();
  void op_bit_b();
  void op_bit_w();
  void op_cmp_b();
  void op_cmp_w();
  void op_cpx_b();
  void op_cpx_w();
  void op_cpy_b();
  void op_cpy_w();
  void op_eor_b();
  void op_eor_w();
  void op_lda_b();
  void op_lda_w();
  void op_ldx_b();
  void op_ldx_w();
  void op_ldy_b();
  void op_ldy_w();
  void op_ora_b();
  void op_ora_w();
  void op_sbc_b();
  void op_sbc_w();

  void op_inc_b();
  void op_inc_w();
  void op_dec_b();
  void op_dec_w();
  void op_asl_b();
  void op_asl_w();
  void op_lsr_b();
  void op_lsr_w();
  void op_rol_b();
  void op_rol_w();
  void op_ror_b();
  void op_ror_w();
  void op_trb_b();
  void op_trb_w();
  void op_tsb_b();
  void op_tsb_w();

  void (CPUcore::**opcode_table)();
  void (CPUcore::*op_table[256 * 5])();
  void initialize_opcode_table();
  void update_table();

  enum {
    table_EM =    0,  // 8-bit accumulator,  8-bit index (emulation mode)
    table_MX =  256,  // 8-bit accumulator,  8-bit index
    table_Mx =  512,  // 8-bit accumulator, 16-bit index
    table_mX =  768,  //16-bit accumulator,  8-bit index
    table_mx = 1024,  //16-bit accumulator, 16-bit index
  };

  CPUcore();
};
