#pragma once

namespace Processor {

struct GSU {
  #include "registers.hpp"

  virtual auto step(uint clocks) -> void = 0;

  virtual auto stop() -> void = 0;
  virtual auto color(uint8 source) -> uint8 = 0;
  virtual auto plot(uint8 x, uint8 y) -> void = 0;
  virtual auto rpix(uint8 x, uint8 y) -> uint8 = 0;

  virtual auto pipe() -> uint8 = 0;
  virtual auto syncROMBuffer() -> void = 0;
  virtual auto readROMBuffer() -> uint8 = 0;
  virtual auto syncRAMBuffer() -> void = 0;
  virtual auto readRAMBuffer(uint16 addr) -> uint8 = 0;
  virtual auto writeRAMBuffer(uint16 addr, uint8 data) -> void = 0;
  virtual auto flushCache() -> void = 0;

  virtual auto read(uint24 addr, uint8 data = 0x00) -> uint8 = 0;
  virtual auto write(uint24 addr, uint8 data) -> void = 0;

  //gsu.cpp
  auto power() -> void;
  auto reset() -> void;

  //instructions.cpp
  auto op_add_adc(uint n);
  auto op_alt1();
  auto op_alt2();
  auto op_alt3();
  auto op_and_bic(uint n);
  auto op_asr_div2();
  auto op_branch(bool c);
  auto op_cache();
  auto op_color_cmode();
  auto op_dec(uint n);
  auto op_fmult_lmult();
  auto op_from_moves(uint n);
  auto op_getb();
  auto op_getc_ramb_romb();
  auto op_hib();
  auto op_ibt_lms_sms(uint n);
  auto op_inc(uint n);
  auto op_iwt_lm_sm(uint n);
  auto op_jmp_ljmp(uint n);
  auto op_link(uint n);
  auto op_load(uint n);
  auto op_lob();
  auto op_loop();
  auto op_lsr();
  auto op_merge();
  auto op_mult_umult(uint n);
  auto op_nop();
  auto op_not();
  auto op_or_xor(uint n);
  auto op_plot_rpix();
  auto op_rol();
  auto op_ror();
  auto op_sbk();
  auto op_sex();
  auto op_store(uint n);
  auto op_stop();
  auto op_sub_sbc_cmp(uint n);
  auto op_swap();
  auto op_to_move(uint n);
  auto op_with(uint n);

  //switch.cpp
  auto instruction(uint8 opcode) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  //disassembler.cpp
  auto disassembleOpcode(char* output) -> void;
  auto disassembleAlt0(char* output) -> void;
  auto disassembleAlt1(char* output) -> void;
  auto disassembleAlt2(char* output) -> void;
  auto disassembleAlt3(char* output) -> void;
};

}
