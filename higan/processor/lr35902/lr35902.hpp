//Sharp LR35902 (Game Boy Z80-derivative)

#pragma once

namespace Processor {

struct LR35902 {
  virtual auto io() -> void = 0;
  virtual auto read(uint16 addr) -> uint8 = 0;
  virtual auto write(uint16 addr, uint8 data) -> void = 0;
  virtual auto stop() -> bool = 0;
  virtual auto debuggerRead(uint16 addr) -> uint8 { return 0; }

  //lr35902.cpp
  auto power() -> void;

  //instruction.cpp
  auto interrupt(uint16 vector) -> void;
  auto instruction() -> void;
  auto instructionCB() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  #include "registers.hpp"

private:
  auto op_xx();
  auto op_cb();

  //8-bit load commands
  auto op_ld_r_r(uint x, uint y);
  auto op_ld_r_n(uint x);
  auto op_ld_r_hl(uint x);
  auto op_ld_hl_r(uint x);
  auto op_ld_hl_n();
  auto op_ld_a_rr(uint x);
  auto op_ld_a_nn();
  auto op_ld_rr_a(uint x);
  auto op_ld_nn_a();
  auto op_ld_a_ffn();
  auto op_ld_ffn_a();
  auto op_ld_a_ffc();
  auto op_ld_ffc_a();
  auto op_ldi_hl_a();
  auto op_ldi_a_hl();
  auto op_ldd_hl_a();
  auto op_ldd_a_hl();

  //16-bit load commands
  auto op_ld_rr_nn(uint x);
  auto op_ld_nn_sp();
  auto op_ld_sp_hl();
  auto op_push_rr(uint x);
  auto op_pop_rr(uint x);

  //8-bit arithmetic commands
  auto opi_add_a(uint8 x);
  auto op_add_a_r(uint x);
  auto op_add_a_n();
  auto op_add_a_hl();

  auto opi_adc_a(uint8 x);
  auto op_adc_a_r(uint x);
  auto op_adc_a_n();
  auto op_adc_a_hl();

  auto opi_sub_a(uint8 x);
  auto op_sub_a_r(uint x);
  auto op_sub_a_n();
  auto op_sub_a_hl();

  auto opi_sbc_a(uint8 x);
  auto op_sbc_a_r(uint x);
  auto op_sbc_a_n();
  auto op_sbc_a_hl();

  auto opi_and_a(uint8 x);
  auto op_and_a_r(uint x);
  auto op_and_a_n();
  auto op_and_a_hl();

  auto opi_xor_a(uint8 x);
  auto op_xor_a_r(uint x);
  auto op_xor_a_n();
  auto op_xor_a_hl();

  auto opi_or_a(uint8 x);
  auto op_or_a_r(uint x);
  auto op_or_a_n();
  auto op_or_a_hl();

  auto opi_cp_a(uint8 x);
  auto op_cp_a_r(uint x);
  auto op_cp_a_n();
  auto op_cp_a_hl();

  auto op_inc_r(uint x);
  auto op_inc_hl();
  auto op_dec_r(uint x);
  auto op_dec_hl();
  auto op_daa();
  auto op_cpl();

  //16-bit arithmetic commands
  auto op_add_hl_rr(uint x);
  auto op_inc_rr(uint x);
  auto op_dec_rr(uint x);
  auto op_add_sp_n();
  auto op_ld_hl_sp_n();

  //rotate/shift commands
  auto op_rlca();
  auto op_rla();
  auto op_rrca();
  auto op_rra();
  auto op_rlc_r(uint x);
  auto op_rlc_hl();
  auto op_rl_r(uint x);
  auto op_rl_hl();
  auto op_rrc_r(uint x);
  auto op_rrc_hl();
  auto op_rr_r(uint x);
  auto op_rr_hl();
  auto op_sla_r(uint x);
  auto op_sla_hl();
  auto op_swap_r(uint x);
  auto op_swap_hl();
  auto op_sra_r(uint x);
  auto op_sra_hl();
  auto op_srl_r(uint x);
  auto op_srl_hl();

  //single-bit commands
  auto op_bit_n_r(uint b, uint x);
  auto op_bit_n_hl(uint b);
  auto op_set_n_r(uint b, uint x);
  auto op_set_n_hl(uint b);
  auto op_res_n_r(uint b, uint x);
  auto op_res_n_hl(uint b);

  //control commands
  auto op_ccf();
  auto op_scf();
  auto op_nop();
  auto op_halt();
  auto op_stop();
  auto op_di();
  auto op_ei();

  //jump commands
  auto op_jp_nn();
  auto op_jp_hl();
  auto op_jp_f_nn(uint x, bool y);
  auto op_jr_n();
  auto op_jr_f_n(uint x, bool y);
  auto op_call_nn();
  auto op_call_f_nn(uint x, bool y);
  auto op_ret();
  auto op_ret_f(uint x, bool y);
  auto op_reti();
  auto op_rst_n(uint n);

  //disassembler.cpp
  auto disassemble(uint16 pc) -> string;
  auto disassembleOpcode(uint16 pc) -> string;
  auto disassembleOpcodeCB(uint16 pc) -> string;
};

}
