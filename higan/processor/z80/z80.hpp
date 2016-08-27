#pragma once

//Zilog Z80

namespace Processor {

struct Z80 {
  virtual auto wait() -> void = 0;
  virtual auto read(uint16 addr) -> uint8 = 0;
  virtual auto write(uint16 addr, uint8 data) -> void = 0;
  virtual auto in(uint8 addr) -> uint8 = 0;
  virtual auto out(uint8 addr, uint8 data) -> void = 0;

  //z80.cpp
  auto power() -> void;
  auto reset() -> void;

  auto parity(uint8_t) const -> bool;

  //instruction.cpp
  auto trap(uint8_t prefix, uint8_t opcode) -> void;
  auto instruction() -> void;
  auto instructionDD() -> void;
  auto instructionED() -> void;
  auto instructionFD() -> void;

  //instructions.cpp
  auto CP(uint8 x) -> void;
  auto instructionCP_ihl() -> void;
  auto instructionCP_n() -> void;
  auto instructionCP_r(uint8_t&) -> void;
  auto instructionDI() -> void;
  auto instructionIM(uint) -> void;
  auto instructionIN_a_in() -> void;
  auto instructionIN_r_ic(uint8_t&) -> void;
  auto instructionJP_c_nn(bool) -> void;
  auto instructionJP_rr(uint16_t&) -> void;
  auto instructionJR_c(bool) -> void;
  auto instructionNOP() -> void;

  //disassembler.cpp
  auto disassemble(uint16 pc) -> string;
  auto disassembleOpcode(uint16 pc) -> string;
  auto disassembleOpcodeDD(uint16 pc) -> string;
  auto disassembleOpcodeED(uint16 pc) -> string;
  auto disassembleOpcodeFD(uint16 pc) -> string;

  struct Registers {
    union {
      uint16_t af;
      struct { uint8_t order_msb2(a, f); };
      union {
        BooleanBitField<uint16_t, 0> c;  //carry
        BooleanBitField<uint16_t, 1> n;  //add / subtract
        BooleanBitField<uint16_t, 2> p;  //parity
        BooleanBitField<uint16_t, 2> v;  //overflow
        BooleanBitField<uint16_t, 3> x;  //unused (copy of bit 3 of result)
        BooleanBitField<uint16_t, 4> h;  //half-carry
        BooleanBitField<uint16_t, 5> y;  //unused (copy of bit 5 of result)
        BooleanBitField<uint16_t, 6> z;  //zero
        BooleanBitField<uint16_t, 7> s;  //sign
      } p;
    };

    union {
      uint16_t bc;
      struct { uint8_t order_msb2(b, c); };
    };

    union {
      uint16_t de;
      struct { uint8_t order_msb2(d, e); };
    };

    union {
      uint16_t hl;
      struct { uint8_t order_msb2(h, l); };
    };

    uint16_t ix;
    uint16_t iy;
    uint16_t sp;
    uint16_t pc;

    uint8_t i;
    uint8_t r;

    boolean di;  //disable interrupt
    boolean ei;  //enable interrupt
    uint2 im;    //interrupt mode (0-2)

    uint8_t flag;
  } r;

private:
  uint64 instructionsExecuted = 0;
};

}
