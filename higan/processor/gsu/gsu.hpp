#pragma once

namespace Processor {

struct GSU {
  #include "registers.hpp"

  virtual auto step(unsigned clocks) -> void = 0;

  virtual auto stop() -> void = 0;
  virtual auto color(uint8 source) -> uint8 = 0;
  virtual auto plot(uint8 x, uint8 y) -> void = 0;
  virtual auto rpix(uint8 x, uint8 y) -> uint8 = 0;

  virtual auto pipe() -> uint8 = 0;
  virtual auto rombuffer_sync() -> void = 0;
  virtual auto rombuffer_read() -> uint8 = 0;
  virtual auto rambuffer_sync() -> void = 0;
  virtual auto rambuffer_read(uint16 addr) -> uint8 = 0;
  virtual auto rambuffer_write(uint16 addr, uint8 data) -> void = 0;
  virtual auto cache_flush() -> void = 0;

  virtual auto bus_read(uint24 addr, uint8 data = 0x00) -> uint8 = 0;
  virtual auto bus_write(uint24 addr, uint8 data) -> void = 0;

  //gsu.cpp
  auto power() -> void;
  auto reset() -> void;

  //instructions.cpp
  template<int> auto op_adc_i();
  template<int> auto op_adc_r();
  template<int> auto op_add_i();
  template<int> auto op_add_r();
  auto op_alt1();
  auto op_alt2();
  auto op_alt3();
  template<int> auto op_and_i();
  template<int> auto op_and_r();
  auto op_asr();
  auto op_bge();
  auto op_bcc();
  auto op_bcs();
  auto op_beq();
  template<int> auto op_bic_i();
  template<int> auto op_bic_r();
  auto op_blt();
  auto op_bmi();
  auto op_bne();
  auto op_bpl();
  auto op_bra();
  auto op_bvc();
  auto op_bvs();
  auto op_cache();
  auto op_cmode();
  template<int> auto op_cmp_r();
  auto op_color();
  template<int> auto op_dec_r();
  auto op_div2();
  auto op_fmult();
  template<int> auto op_from_r();
  auto op_getb();
  auto op_getbl();
  auto op_getbh();
  auto op_getbs();
  auto op_getc();
  auto op_hib();
  template<int> auto op_ibt_r();
  template<int> auto op_inc_r();
  template<int> auto op_iwt_r();
  template<int> auto op_jmp_r();
  template<int> auto op_ldb_ir();
  template<int> auto op_ldw_ir();
  template<int> auto op_link();
  template<int> auto op_ljmp_r();
  template<int> auto op_lm_r();
  template<int> auto op_lms_r();
  auto op_lmult();
  auto op_lob();
  auto op_loop();
  auto op_lsr();
  auto op_merge();
  template<int> auto op_mult_i();
  template<int> auto op_mult_r();
  auto op_nop();
  auto op_not();
  template<int> auto op_or_i();
  template<int> auto op_or_r();
  auto op_plot();
  auto op_ramb();
  auto op_rol();
  auto op_romb();
  auto op_ror();
  auto op_rpix();
  template<int> auto op_sbc_r();
  auto op_sbk();
  auto op_sex();
  template<int> auto op_sm_r();
  template<int> auto op_sms_r();
  template<int> auto op_stb_ir();
  auto op_stop();
  template<int> auto op_stw_ir();
  template<int> auto op_sub_i();
  template<int> auto op_sub_r();
  auto op_swap();
  template<int> auto op_to_r();
  template<int> auto op_umult_i();
  template<int> auto op_umult_r();
  template<int> auto op_with_r();
  template<int> auto op_xor_i();
  template<int> auto op_xor_r();

  //table.cpp
  auto (GSU::*opcode_table[1024])() -> void;
  auto initialize_opcode_table() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  //disassembler.cpp
  auto disassemble_opcode(char* output) -> void;
  auto disassemble_alt0(char* output) -> void;
  auto disassemble_alt1(char* output) -> void;
  auto disassemble_alt2(char* output) -> void;
  auto disassemble_alt3(char* output) -> void;
};

}
