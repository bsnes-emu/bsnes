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

  virtual auto io() -> void = 0;
  virtual auto read(uint24 addr) -> uint8 = 0;
  virtual auto write(uint24 addr, uint8 data) -> void = 0;
  virtual auto lastCycle() -> void = 0;
  virtual auto interruptPending() const -> bool = 0;
  virtual auto interrupt() -> void;

  virtual auto disassemblerRead(uint24 addr) -> uint8 { return 0; }

  //r65816.cpp
  alwaysinline auto ioIRQ() -> void;
  alwaysinline auto io2() -> void;
  alwaysinline auto io4(uint16 x, uint16 y) -> void;
  alwaysinline auto io6(uint16 addr) -> void;

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
  auto op_read_dpr_b(fp, Reg16&);
  auto op_read_dpr_w(fp, Reg16&);
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
  auto op_write_addr_b(Reg16&);
  auto op_write_addr_w(Reg16&);
  auto op_write_addrr_b(Reg16&, Reg16&);
  auto op_write_addrr_w(Reg16&, Reg16&);
  auto op_write_longr_b(Reg16&);
  auto op_write_longr_w(Reg16&);
  auto op_write_dp_b(Reg16&);
  auto op_write_dp_w(Reg16&);
  auto op_write_dpr_b(Reg16&, Reg16&);
  auto op_write_dpr_w(Reg16&, Reg16&);
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
  auto op_adjust_imm_b(Reg16&, int);
  auto op_adjust_imm_w(Reg16&, int);
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
  auto op_jsr_long();
  auto op_jsr_iaddrx();
  auto op_rti();
  auto op_rts();
  auto op_rtl();

  //opcode_misc.cpp
  auto op_nop();
  auto op_wdm();
  auto op_xba();
  auto op_move_b(int adjust);
  auto op_move_w(int adjust);
  auto op_interrupt(uint16);
  auto op_stp();
  auto op_wai();
  auto op_xce();
  auto op_set_flag(uint bit);
  auto op_clear_flag(uint bit);
  auto op_pflag(bool);
  auto op_transfer_b(Reg16&, Reg16&);
  auto op_transfer_w(Reg16&, Reg16&);
  auto op_tcs();
  auto op_tsx_b();
  auto op_tsx_w();
  auto op_txs();
  auto op_push_b(Reg16&);
  auto op_push_w(Reg16&);
  auto op_phd();
  auto op_phb();
  auto op_phk();
  auto op_php();
  auto op_pull_b(Reg16&);
  auto op_pull_w(Reg16&);
  auto op_pld();
  auto op_plb();
  auto op_plp();
  auto op_pea();
  auto op_pei();
  auto op_per();

  //switch.cpp
  auto instruction() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  Registers r;
  Reg24 aa, rd;
  uint8 sp, dp;
};

}
