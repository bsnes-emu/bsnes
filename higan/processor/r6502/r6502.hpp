//Ricoh 6502
//* Ricoh 2A03
//* Ricoh 2A07

#pragma once

namespace Processor {

struct R6502 {
  virtual auto op_read(uint16 addr) -> uint8 = 0;
  virtual auto op_write(uint16 addr, uint8 data) -> void = 0;
  virtual auto last_cycle() -> void = 0;
  virtual auto nmi(uint16& vector) -> void = 0;
  virtual auto debugger_read(uint16 addr) -> uint8 { return 0u; }

  auto mdr() const -> uint8;
  auto power() -> void;
  auto reset() -> void;
  auto interrupt() -> void;
  auto exec() -> void;

  auto serialize(serializer&) -> void;

  //memory.cpp
  auto op_readpc() -> uint8;
  auto op_readpci() -> uint8;
  auto op_readsp() -> uint8;
  auto op_readzp(uint8 addr) -> uint8;

  auto op_writesp(uint8 data) -> void;
  auto op_writezp(uint8 addr, uint8 data) -> void;

  auto op_page(uint16 x, uint16 y) -> void;
  auto op_page_always(uint16 x, uint16 y) -> void;

  //instructions.cpp
  auto opf_asl();
  auto opf_adc();
  auto opf_and();
  auto opf_bit();
  auto opf_cmp();
  auto opf_cpx();
  auto opf_cpy();
  auto opf_dec();
  auto opf_eor();
  auto opf_inc();
  auto opf_lda();
  auto opf_ldx();
  auto opf_ldy();
  auto opf_lsr();
  auto opf_ora();
  auto opf_rla();
  auto opf_rol();
  auto opf_ror();
  auto opf_rra();
  auto opf_sbc();
  auto opf_sla();
  auto opf_sra();

  using fp = auto (R6502::*)() -> void;

  auto opi_branch(bool condition);
  auto opi_clear_flag(uint bit);
  auto opi_decrement(uint8& r);
  auto opi_increment(uint8& r);
  auto opi_pull(uint8& r);
  auto opi_push(uint8& r);
  auto opi_read_absolute(fp);
  auto opi_read_absolute_x(fp);
  auto opi_read_absolute_y(fp);
  auto opi_read_immediate(fp);
  auto opi_read_indirect_zero_page_x(fp);
  auto opi_read_indirect_zero_page_y(fp);
  auto opi_read_zero_page(fp);
  auto opi_read_zero_page_x(fp);
  auto opi_read_zero_page_y(fp);
  auto opi_rmw_absolute(fp);
  auto opi_rmw_absolute_x(fp);
  auto opi_rmw_zero_page(fp);
  auto opi_rmw_zero_page_x(fp);
  auto opi_set_flag(uint bit);
  auto opi_shift(fp);
  auto opi_store_absolute(uint8& r);
  auto opi_store_absolute_x(uint8& r);
  auto opi_store_absolute_y(uint8& r);
  auto opi_store_indirect_zero_page_x(uint8& r);
  auto opi_store_indirect_zero_page_y(uint8& r);
  auto opi_store_zero_page(uint8& r);
  auto opi_store_zero_page_x(uint8& r);
  auto opi_store_zero_page_y(uint8& r);
  auto opi_transfer(uint8& s, uint8& d, bool flag);

  auto op_brk();
  auto op_jmp_absolute();
  auto op_jmp_indirect_absolute();
  auto op_jsr_absolute();
  auto op_nop();
  auto op_php();
  auto op_plp();
  auto op_rti();
  auto op_rts();

  auto opill_arr_immediate();
  auto opill_nop_absolute();
  auto opill_nop_absolute_x();
  auto opill_nop_immediate();
  auto opill_nop_implied();
  auto opill_nop_zero_page();
  auto opill_nop_zero_page_x();

  //disassembler.cpp
  auto disassemble() -> string;

  #include "registers.hpp"
};

}
