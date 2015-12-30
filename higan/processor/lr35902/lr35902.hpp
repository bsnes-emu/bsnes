//Sharp LR35902 (Game Boy Z80-derivative)

#ifndef PROCESSOR_LR35902_HPP
#define PROCESSOR_LR35902_HPP

namespace Processor {

struct LR35902 {
  virtual auto op_io() -> void = 0;
  virtual auto op_read(uint16 addr) -> uint8 = 0;
  virtual auto op_write(uint16 addr, uint8 data) -> void = 0;
  virtual auto stop() -> bool = 0;
  virtual auto debugger_read(uint16 addr) -> uint8 { return 0u; }

  auto power() -> void;
  auto exec() -> void;
  auto execCB() -> void;

  auto serialize(serializer&) -> void;

  #include "registers.hpp"

privileged:
  auto op_xx();
  auto op_cb();

  //8-bit load commands
  template<uint x, uint y> auto op_ld_r_r();
  template<uint x> auto op_ld_r_n();
  template<uint x> auto op_ld_r_hl();
  template<uint x> auto op_ld_hl_r();
  auto op_ld_hl_n();
  template<uint x> auto op_ld_a_rr();
  auto op_ld_a_nn();
  template<uint x> auto op_ld_rr_a();
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
  template<uint x> auto op_ld_rr_nn();
  auto op_ld_nn_sp();
  auto op_ld_sp_hl();
  template<uint x> auto op_push_rr();
  template<uint x> auto op_pop_rr();

  //8-bit arithmetic commands
  auto opi_add_a(uint8 x);
  template<uint x> auto op_add_a_r();
  auto op_add_a_n();
  auto op_add_a_hl();

  auto opi_adc_a(uint8 x);
  template<uint x> auto op_adc_a_r();
  auto op_adc_a_n();
  auto op_adc_a_hl();

  auto opi_sub_a(uint8 x);
  template<uint x> auto op_sub_a_r();
  auto op_sub_a_n();
  auto op_sub_a_hl();

  auto opi_sbc_a(uint8 x);
  template<uint x> auto op_sbc_a_r();
  auto op_sbc_a_n();
  auto op_sbc_a_hl();

  auto opi_and_a(uint8 x);
  template<uint x> auto op_and_a_r();
  auto op_and_a_n();
  auto op_and_a_hl();

  auto opi_xor_a(uint8 x);
  template<uint x> auto op_xor_a_r();
  auto op_xor_a_n();
  auto op_xor_a_hl();

  auto opi_or_a(uint8 x);
  template<uint x> auto op_or_a_r();
  auto op_or_a_n();
  auto op_or_a_hl();

  auto opi_cp_a(uint8 x);
  template<uint x> auto op_cp_a_r();
  auto op_cp_a_n();
  auto op_cp_a_hl();

  template<uint x> auto op_inc_r();
  auto op_inc_hl();
  template<uint x> auto op_dec_r();
  auto op_dec_hl();
  auto op_daa();
  auto op_cpl();

  //16-bit arithmetic commands
  template<uint x> auto op_add_hl_rr();
  template<uint x> auto op_inc_rr();
  template<uint x> auto op_dec_rr();
  auto op_add_sp_n();
  auto op_ld_hl_sp_n();

  //rotate/shift commands
  auto op_rlca();
  auto op_rla();
  auto op_rrca();
  auto op_rra();
  template<uint x> auto op_rlc_r();
  auto op_rlc_hl();
  template<uint x> auto op_rl_r();
  auto op_rl_hl();
  template<uint x> auto op_rrc_r();
  auto op_rrc_hl();
  template<uint x> auto op_rr_r();
  auto op_rr_hl();
  template<uint x> auto op_sla_r();
  auto op_sla_hl();
  template<uint x> auto op_swap_r();
  auto op_swap_hl();
  template<uint x> auto op_sra_r();
  auto op_sra_hl();
  template<uint x> auto op_srl_r();
  auto op_srl_hl();

  //single-bit commands
  template<uint b, uint x> auto op_bit_n_r();
  template<uint b> auto op_bit_n_hl();
  template<uint b, uint x> auto op_set_n_r();
  template<uint b> auto op_set_n_hl();
  template<uint b, uint x> auto op_res_n_r();
  template<uint b> auto op_res_n_hl();

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
  template<uint x, bool y> auto op_jp_f_nn();
  auto op_jr_n();
  template<uint x, bool y> auto op_jr_f_n();
  auto op_call_nn();
  template<uint x, bool y> auto op_call_f_nn();
  auto op_ret();
  template<uint x, bool y> auto op_ret_f();
  auto op_reti();
  template<uint n> auto op_rst_n();

  //disassembler.cpp
  auto disassemble(uint16 pc) -> string;
  auto disassembleOpcode(uint16 pc) -> string;
  auto disassembleOpcodeCB(uint16 pc) -> string;
};

}

#endif
