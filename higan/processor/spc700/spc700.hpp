#pragma once

namespace Processor {

struct SPC700 {
  virtual auto io() -> void = 0;
  virtual auto read(uint16 addr) -> uint8 = 0;
  virtual auto write(uint16 addr, uint8 data) -> void = 0;
  virtual auto disassemblerRead(uint16 addr) -> uint8 = 0;

  auto instruction() -> void;

  auto serialize(serializer&) -> void;

  auto disassemble(uint16 addr, bool p) -> string;

  #include "registers.hpp"
  #include "memory.hpp"

  Regs regs;
  Word dp, sp, rd, wr, bit, ya;
  uint8 opcode;

protected:
  using fps = auto (SPC700::*)(uint8) -> uint8;
  using fpb = auto (SPC700::*)(uint8, uint8) -> uint8;
  using fpw = auto (SPC700::*)(uint16, uint16) -> uint16;

  auto op_adc(uint8, uint8) -> uint8;
  auto op_and(uint8, uint8) -> uint8;
  auto op_asl(uint8) -> uint8;
  auto op_cmp(uint8, uint8) -> uint8;
  auto op_dec(uint8) -> uint8;
  auto op_eor(uint8, uint8) -> uint8;
  auto op_inc(uint8) -> uint8;
  auto op_ld (uint8, uint8) -> uint8;
  auto op_lsr(uint8) -> uint8;
  auto op_or (uint8, uint8) -> uint8;
  auto op_rol(uint8) -> uint8;
  auto op_ror(uint8) -> uint8;
  auto op_sbc(uint8, uint8) -> uint8;
  auto op_st (uint8, uint8) -> uint8;
  auto op_adw(uint16, uint16) -> uint16;
  auto op_cpw(uint16, uint16) -> uint16;
  auto op_ldw(uint16, uint16) -> uint16;
  auto op_sbw(uint16, uint16) -> uint16;

  auto op_adjust(fps, uint8_t&);
  auto op_adjust_addr(fps);
  auto op_adjust_dp(fps);
  auto op_adjust_dpw(int);
  auto op_adjust_dpx(fps);
  auto op_branch(bool);
  auto op_branch_bit();
  auto op_pull(uint8_t&);
  auto op_push(uint8);
  auto op_read_addr(fpb, uint8_t&);
  auto op_read_addri(fpb, uint8_t&);
  auto op_read_const(fpb, uint8_t&);
  auto op_read_dp(fpb, uint8_t&);
  auto op_read_dpi(fpb, uint8_t&, uint8_t&);
  auto op_read_dpw(fpw);
  auto op_read_idpx(fpb);
  auto op_read_idpy(fpb);
  auto op_read_ix(fpb);
  auto op_set_addr_bit();
  auto op_set_bit();
  auto op_set_flag(bool&, bool);
  auto op_test_addr(bool);
  auto op_transfer(uint8_t&, uint8_t&);
  auto op_write_addr(uint8_t&);
  auto op_write_addri(uint8_t&);
  auto op_write_dp(uint8_t&);
  auto op_write_dpi(uint8_t&, uint8_t&);
  auto op_write_dp_const(fpb);
  auto op_write_dp_dp(fpb);
  auto op_write_ix_iy(fpb);

  auto op_bne_dp();
  auto op_bne_dpdec();
  auto op_bne_dpx();
  auto op_bne_ydec();
  auto op_brk();
  auto op_clv();
  auto op_cmc();
  auto op_daa();
  auto op_das();
  auto op_div_ya_x();
  auto op_jmp_addr();
  auto op_jmp_iaddrx();
  auto op_jsp_dp();
  auto op_jsr_addr();
  auto op_jst();
  auto op_lda_ixinc();
  auto op_mul_ya();
  auto op_nop();
  auto op_plp();
  auto op_rti();
  auto op_rts();
  auto op_sta_idpx();
  auto op_sta_idpy();
  auto op_sta_ix();
  auto op_sta_ixinc();
  auto op_stw_dp();
  auto op_wait();
  auto op_xcn();
};

}
