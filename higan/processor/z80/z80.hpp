#pragma once

//Zilog Z80

namespace Processor {

struct Z80 {
  struct Bus {
    virtual auto read(uint16 addr) -> uint8 = 0;
    virtual auto write(uint16 addr, uint8 data) -> void = 0;
    virtual auto in(uint8 addr) -> uint8 = 0;
    virtual auto out(uint8 addr, uint8 data) -> void = 0;
  };

  virtual auto step(uint clocks) -> void = 0;

  //z80.cpp
  auto power() -> void;
  auto reset() -> void;

  auto parity(uint8) const -> bool;

  //memory.cpp
  auto wait(uint clocks = 1) -> void;
  auto opcode() -> uint8;
  auto operand() -> uint8;
  auto operands() -> uint16;
  auto displace(uint16&) -> uint16;
  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 data) -> void;
  auto in(uint8 addr) -> uint8;
  auto out(uint8 addr, uint8 data) -> void;

  //instruction.cpp
  auto trap(uint8 prefix, uint8 code) -> void;
  auto instruction() -> void;
  auto instruction__(uint8 code) -> void;
  auto instructionCB(uint8 code) -> void;
  auto instructionED(uint8 code) -> void;

  //instructions.cpp
  auto ADD(uint8, uint8, bool = false) -> uint8;
  auto AND(uint8, uint8) -> uint8;
  auto OR (uint8, uint8) -> uint8;
  auto SUB(uint8, uint8, bool = false) -> uint8;
  auto XOR(uint8, uint8) -> uint8;

  auto instructionADC_a_irr(uint16&) -> void;
  auto instructionADC_a_r(uint8&) -> void;
  auto instructionADD_a_irr(uint16&) -> void;
  auto instructionADD_a_r(uint8&) -> void;
  auto instructionAND_a_irr(uint16&) -> void;
  auto instructionAND_a_r(uint8&) -> void;
  auto instructionCP_a_irr(uint16& x) -> void;
  auto instructionCP_a_n() -> void;
  auto instructionCP_a_r(uint8& x) -> void;
  auto instructionDI() -> void;
  auto instructionEI() -> void;
  auto instructionHALT() -> void;
  auto instructionIM_o(uint2) -> void;
  auto instructionIN_a_in() -> void;
  auto instructionJP_c_nn(bool) -> void;
  auto instructionJR_c_e(bool) -> void;
  auto instructionLD_inn_a() -> void;
  auto instructionLD_irr_n(uint16&) -> void;
  auto instructionLD_irr_r(uint16&, uint8&) -> void;
  auto instructionLD_r_n(uint8&) -> void;
  auto instructionLD_r_irr(uint8&, uint16&) -> void;
  auto instructionLD_r_r(uint8&, uint8&) -> void;
  auto instructionLD_rr_nn(uint16&) -> void;
  auto instructionNOP() -> void;
  auto instructionOR_a_irr(uint16&) -> void;
  auto instructionOR_a_r(uint8&) -> void;
  auto instructionSBC_a_irr(uint16&) -> void;
  auto instructionSBC_a_r(uint8&) -> void;
  auto instructionSUB_a_irr(uint16&) -> void;
  auto instructionSUB_a_r(uint8&) -> void;
  auto instructionXOR_a_irr(uint16&) -> void;
  auto instructionXOR_a_r(uint8&) -> void;

  //disassembler.cpp
  auto disassemble(uint16 pc) -> string;
  auto disassemble__(uint16 pc, uint8 prefix, uint8 code) -> string;
  auto disassembleCB(uint16 pc, uint8 prefix, uint8 code) -> string;
  auto disassembleED(uint16 pc, uint8 prefix, uint8 code) -> string;

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

    union {
      uint16_t ix;
      struct { uint8_t order_msb2(ixh, ixl); };
    };

    union {
      uint16_t iy;
      struct { uint8_t order_msb2(iyh, iyl); };
    };

    union {
      uint16_t ir;
      struct { uint8_t order_msb2(i, r); };
    };

    uint16_t sp;
    uint16_t pc;

    boolean halt;  //HALT instruction executed
    boolean iff1;  //interrupt flip-flop 1
    boolean iff2;  //interrupt flip-flop 2
    uint2 im;      //interrupt mode (0-2)

    uint8 prefix;
    uint8 flag;
  } r;

  Bus* bus = nullptr;

private:
  uint64 instructionsExecuted = 0;
};

}
