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

  auto op_adjust(fps, uint8_t&) -> void;
  auto op_adjust_addr(fps) -> void;
  auto op_adjust_dp(fps) -> void;
  auto op_adjust_dpw(int) -> void;
  auto op_adjust_dpx(fps) -> void;
  auto op_branch(bool) -> void;
  auto op_branch_bit() -> void;
  auto op_pull(uint8_t&) -> void;
  auto op_push(uint8) -> void;
  auto op_read_addr(fpb, uint8_t&) -> void;
  auto op_read_addri(fpb, uint8_t&) -> void;
  auto op_read_const(fpb, uint8_t&) -> void;
  auto op_read_dp(fpb, uint8_t&) -> void;
  auto op_read_dpi(fpb, uint8_t&, uint8_t&) -> void;
  auto op_read_dpw(fpw) -> void;
  auto op_read_idpx(fpb) -> void;
  auto op_read_idpy(fpb) -> void;
  auto op_read_ix(fpb) -> void;
  auto op_set_addr_bit() -> void;
  auto op_set_bit() -> void;
  auto op_set_flag(bool&, bool) -> void;
  auto op_test_addr(bool) -> void;
  auto op_transfer(uint8_t&, uint8_t&) -> void;
  auto op_write_addr(uint8_t&) -> void;
  auto op_write_addri(uint8_t&) -> void;
  auto op_write_dp(uint8_t&) -> void;
  auto op_write_dpi(uint8_t&, uint8_t&) -> void;
  auto op_write_dp_const(fpb) -> void;
  auto op_write_dp_dp(fpb) -> void;
  auto op_write_ix_iy(fpb) -> void;

  auto op_bne_dp() -> void;
  auto op_bne_dpdec() -> void;
  auto op_bne_dpx() -> void;
  auto op_bne_ydec() -> void;
  auto op_brk() -> void;
  auto op_clv() -> void;
  auto op_cmc() -> void;
  auto op_daa() -> void;
  auto op_das() -> void;
  auto op_div_ya_x() -> void;
  auto op_jmp_addr() -> void;
  auto op_jmp_iaddrx() -> void;
  auto op_jsp_dp() -> void;
  auto op_jsr_addr() -> void;
  auto op_jst() -> void;
  auto op_lda_ixinc() -> void;
  auto op_mul_ya() -> void;
  auto op_nop() -> void;
  auto op_plp() -> void;
  auto op_rti() -> void;
  auto op_rts() -> void;
  auto op_sta_idpx() -> void;
  auto op_sta_idpy() -> void;
  auto op_sta_ix() -> void;
  auto op_sta_ixinc() -> void;
  auto op_stw_dp() -> void;
  auto op_wait() -> void;
  auto op_xcn() -> void;
};

}
