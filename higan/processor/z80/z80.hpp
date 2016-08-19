#pragma once

//Zilog Z80

namespace Processor {

struct Z80 {
  virtual auto wait() -> void = 0;
  virtual auto read(uint16 addr) -> uint8 = 0;
  virtual auto write(uint16 addr, uint8 data) -> void = 0;

  //z80.cpp
  auto power() -> void;
  auto reset() -> void;

  //instruction.cpp
  auto instruction() -> void;

  //instructions.cpp
  auto instructionDI() -> void;
  auto instructionNOP() -> void;

  struct Registers {
    union {
      uint16_t af;
      struct { uint8_t order_msb2(a, f); };
      union {
        BooleanBitField<uint16_t, 0> c;  //carry
        BooleanBitField<uint16_t, 1> s;  //subtract
        BooleanBitField<uint16_t, 2> v;  //overflow
      //BooleanBitField<uint16_t, 3> _;  //unused (copy of bit 3 of result)
        BooleanBitField<uint16_t, 4> h;  //half-carry
      //BooleanBitField<uint16_t, 5> _;  //unused (copy of bit 5 of result)
        BooleanBitField<uint16_t, 6> z;  //zero
        BooleanBitField<uint16_t, 7> n;  //negative
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
  } r;

private:
  uint64 instructionsExecuted = 0;
};

}
