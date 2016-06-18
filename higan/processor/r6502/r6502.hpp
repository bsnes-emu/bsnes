//Ricoh 6502
//* Ricoh 2A03
//* Ricoh 2A07

#pragma once

namespace Processor {

struct R6502 {
  virtual auto read(uint16 addr) -> uint8 = 0;
  virtual auto write(uint16 addr, uint8 data) -> void = 0;
  virtual auto lastCycle() -> void = 0;
  virtual auto nmi(uint16& vector) -> void = 0;
  virtual auto readDebugger(uint16 addr) -> uint8 { return 0u; }

  auto mdr() const -> uint8;
  auto power() -> void;
  auto reset() -> void;
  auto interrupt() -> void;
  auto instruction() -> void;

  auto serialize(serializer&) -> void;

  //memory.cpp
  auto io() -> uint8;
  auto readPC() -> uint8;
  auto readSP() -> uint8;
  auto readZP(uint8 addr) -> uint8;

  auto writeSP(uint8 data) -> void;
  auto writeZP(uint8 addr, uint8 data) -> void;

  auto ioPage(uint16 x, uint16 y) -> void;
  auto ioPageAlways(uint16 x, uint16 y) -> void;

  //instructions.cpp
  using fp = auto (R6502::*)() -> void;
  auto fp_asl();
  auto fp_adc();
  auto fp_and();
  auto fp_bit();
  auto fp_cmp();
  auto fp_cpx();
  auto fp_cpy();
  auto fp_dec();
  auto fp_eor();
  auto fp_inc();
  auto fp_lda();
  auto fp_ldx();
  auto fp_ldy();
  auto fp_lsr();
  auto fp_ora();
  auto fp_rla();
  auto fp_rol();
  auto fp_ror();
  auto fp_rra();
  auto fp_sbc();
  auto fp_sla();
  auto fp_sra();

  auto op_branch(bool condition);
  auto op_clear_flag(uint bit);
  auto op_decrement(uint8& r);
  auto op_increment(uint8& r);
  auto op_pull(uint8& r);
  auto op_push(uint8& r);
  auto op_read_absolute(fp);
  auto op_read_absolute_x(fp);
  auto op_read_absolute_y(fp);
  auto op_read_immediate(fp);
  auto op_read_indirect_zero_page_x(fp);
  auto op_read_indirect_zero_page_y(fp);
  auto op_read_zero_page(fp);
  auto op_read_zero_page_x(fp);
  auto op_read_zero_page_y(fp);
  auto op_rmw_absolute(fp);
  auto op_rmw_absolute_x(fp);
  auto op_rmw_zero_page(fp);
  auto op_rmw_zero_page_x(fp);
  auto op_set_flag(uint bit);
  auto op_shift(fp);
  auto op_store_absolute(uint8& r);
  auto op_store_absolute_x(uint8& r);
  auto op_store_absolute_y(uint8& r);
  auto op_store_indirect_zero_page_x(uint8& r);
  auto op_store_indirect_zero_page_y(uint8& r);
  auto op_store_zero_page(uint8& r);
  auto op_store_zero_page_x(uint8& r);
  auto op_store_zero_page_y(uint8& r);
  auto op_transfer(uint8& s, uint8& d, bool flag);

  auto op_brk();
  auto op_jmp_absolute();
  auto op_jmp_indirect_absolute();
  auto op_jsr_absolute();
  auto op_nop();
  auto op_php();
  auto op_plp();
  auto op_rti();
  auto op_rts();

  auto op_arr_immediate();
  auto op_nop_absolute();
  auto op_nop_absolute_x();
  auto op_nop_immediate();
  auto op_nop_implied();
  auto op_nop_zero_page();
  auto op_nop_zero_page_x();

  //disassembler.cpp
  auto disassemble() -> string;

  #include "registers.hpp"
};

}
