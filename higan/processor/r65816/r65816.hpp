//WDC 65C816 CPU core
//* Ricoh 5A22
//* Nintendo SA-1

#pragma once

namespace Processor {

struct R65816 {
  #include "registers.hpp"
  #include "memory.hpp"
  #include "disassembler.hpp"

  using fp = auto (R65816::*)() -> void;

  virtual auto op_io() -> void = 0;
  virtual auto op_read(uint24 addr) -> uint8 = 0;
  virtual auto op_write(uint24 addr, uint8 data) -> void = 0;
  virtual auto last_cycle() -> void = 0;
  virtual auto interrupt_pending() -> bool = 0;
  virtual auto op_irq() -> void;

  virtual auto disassembler_read(uint24 addr) -> uint8 { return 0u; }

  //r65816.cpp
  alwaysinline auto op_io_irq() -> void;
  alwaysinline auto op_io_cond2() -> void;
  alwaysinline auto op_io_cond4(uint16 x, uint16 y) -> void;
  alwaysinline auto op_io_cond6(uint16 addr) -> void;

  //algorithms.cpp
  auto op_adc_b();
  auto op_adc_w();
  auto op_and_b();
  auto op_and_w();
  auto op_bit_b();
  auto op_bit_w();
  auto op_cmp_b();
  auto op_cmp_w();
  auto op_cpx_b();
  auto op_cpx_w();
  auto op_cpy_b();
  auto op_cpy_w();
  auto op_eor_b();
  auto op_eor_w();
  auto op_lda_b();
  auto op_lda_w();
  auto op_ldx_b();
  auto op_ldx_w();
  auto op_ldy_b();
  auto op_ldy_w();
  auto op_ora_b();
  auto op_ora_w();
  auto op_sbc_b();
  auto op_sbc_w();

  auto op_inc_b();
  auto op_inc_w();
  auto op_dec_b();
  auto op_dec_w();
  auto op_asl_b();
  auto op_asl_w();
  auto op_lsr_b();
  auto op_lsr_w();
  auto op_rol_b();
  auto op_rol_w();
  auto op_ror_b();
  auto op_ror_w();
  auto op_trb_b();
  auto op_trb_w();
  auto op_tsb_b();
  auto op_tsb_w();

  //opcode_read.cpp
  auto op_read_const_b(fp);
  auto op_read_const_w(fp);
  auto op_read_bit_const_b();
  auto op_read_bit_const_w();
  auto op_read_addr_b(fp);
  auto op_read_addr_w(fp);
  auto op_read_addrx_b(fp);
  auto op_read_addrx_w(fp);
  auto op_read_addry_b(fp);
  auto op_read_addry_w(fp);
  auto op_read_long_b(fp);
  auto op_read_long_w(fp);
  auto op_read_longx_b(fp);
  auto op_read_longx_w(fp);
  auto op_read_dp_b(fp);
  auto op_read_dp_w(fp);
  auto op_read_dpr_b(fp, reg16_t&);
  auto op_read_dpr_w(fp, reg16_t&);
  auto op_read_idp_b(fp);
  auto op_read_idp_w(fp);
  auto op_read_idpx_b(fp);
  auto op_read_idpx_w(fp);
  auto op_read_idpy_b(fp);
  auto op_read_idpy_w(fp);
  auto op_read_ildp_b(fp);
  auto op_read_ildp_w(fp);
  auto op_read_ildpy_b(fp);
  auto op_read_ildpy_w(fp);
  auto op_read_sr_b(fp);
  auto op_read_sr_w(fp);
  auto op_read_isry_b(fp);
  auto op_read_isry_w(fp);

  //opcode_write.cpp
  auto op_write_addr_b(reg16_t&);
  auto op_write_addr_w(reg16_t&);
  auto op_write_addrr_b(reg16_t&, reg16_t&);
  auto op_write_addrr_w(reg16_t&, reg16_t&);
  auto op_write_longr_b(reg16_t&);
  auto op_write_longr_w(reg16_t&);
  auto op_write_dp_b(reg16_t&);
  auto op_write_dp_w(reg16_t&);
  auto op_write_dpr_b(reg16_t&, reg16_t&);
  auto op_write_dpr_w(reg16_t&, reg16_t&);
  auto op_sta_idp_b();
  auto op_sta_idp_w();
  auto op_sta_ildp_b();
  auto op_sta_ildp_w();
  auto op_sta_idpx_b();
  auto op_sta_idpx_w();
  auto op_sta_idpy_b();
  auto op_sta_idpy_w();
  auto op_sta_ildpy_b();
  auto op_sta_ildpy_w();
  auto op_sta_sr_b();
  auto op_sta_sr_w();
  auto op_sta_isry_b();
  auto op_sta_isry_w();

  //opcode_rmw.cpp
  auto op_adjust_imm_b(reg16_t&, signed);
  auto op_adjust_imm_w(reg16_t&, signed);
  auto op_asl_imm_b();
  auto op_asl_imm_w();
  auto op_lsr_imm_b();
  auto op_lsr_imm_w();
  auto op_rol_imm_b();
  auto op_rol_imm_w();
  auto op_ror_imm_b();
  auto op_ror_imm_w();
  auto op_adjust_addr_b(fp op);
  auto op_adjust_addr_w(fp op);
  auto op_adjust_addrx_b(fp op);
  auto op_adjust_addrx_w(fp op);
  auto op_adjust_dp_b(fp op);
  auto op_adjust_dp_w(fp op);
  auto op_adjust_dpx_b(fp op);
  auto op_adjust_dpx_w(fp op);

  //opcode_pc.cpp
  auto op_branch(bool flag, bool value);
  auto op_bra();
  auto op_brl();
  auto op_jmp_addr();
  auto op_jmp_long();
  auto op_jmp_iaddr();
  auto op_jmp_iaddrx();
  auto op_jmp_iladdr();
  auto op_jsr_addr();
  auto op_jsr_long_e();
  auto op_jsr_long_n();
  auto op_jsr_iaddrx_e();
  auto op_jsr_iaddrx_n();
  auto op_rti_e();
  auto op_rti_n();
  auto op_rts();
  auto op_rtl_e();
  auto op_rtl_n();

  //opcode_misc.cpp
  auto op_nop();
  auto op_wdm();
  auto op_xba();
  auto op_move_b(signed adjust);
  auto op_move_w(signed adjust);
  auto op_interrupt_e(uint16);
  auto op_interrupt_n(uint16);
  auto op_stp();
  auto op_wai();
  auto op_xce();
  auto op_flag(bool& flag, bool value);
  auto op_pflag_e(bool);
  auto op_pflag_n(bool);
  auto op_transfer_b(reg16_t&, reg16_t&);
  auto op_transfer_w(reg16_t&, reg16_t&);
  auto op_tcs_e();
  auto op_tcs_n();
  auto op_tsx_b();
  auto op_tsx_w();
  auto op_txs_e();
  auto op_txs_n();
  auto op_push_b(reg16_t&);
  auto op_push_w(reg16_t&);
  auto op_phd_e();
  auto op_phd_n();
  auto op_phb();
  auto op_phk();
  auto op_php();
  auto op_pull_b(reg16_t&);
  auto op_pull_w(reg16_t&);
  auto op_pld_e();
  auto op_pld_n();
  auto op_plb();
  auto op_plp_e();
  auto op_plp_n();
  auto op_pea_e();
  auto op_pea_n();
  auto op_pei_e();
  auto op_pei_n();
  auto op_per_e();
  auto op_per_n();

  //switch.cpp
  auto op_exec() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  regs_t regs;
  reg24_t aa, rd;
  uint8 sp, dp;
};

}
