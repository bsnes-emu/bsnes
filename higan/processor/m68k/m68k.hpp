#pragma once

//Motorola M68000

namespace Processor {

struct M68K {
  virtual auto step(uint clocks) -> void = 0;
  virtual auto read(uint32 addr) -> uint8 = 0;
  virtual auto write(uint32 addr, uint8 data) -> void = 0;

  auto power() -> void;
  auto reset() -> void;
  auto instruction() -> void;

  //memory.cpp
  auto readByte(uint32 addr) -> uint8;
  auto readWord(uint32 addr) -> uint16;
  auto readLong(uint32 addr) -> uint32;
  auto readQuad(uint32 addr) -> uint64;

  struct Registers {
    //todo: this is almost certainly UB or IB due to alignment rules ...
    union {
      uint32_t d[8];
      struct { uint32_t d0, d1, d2, d3, d4, d5, d6, d7; };
    };
    union {
      uint32_t a[8];
      struct { uint32_t a0, a1, a2, a3, a4, a5, a6; union { uint32_t a7, sp; }; };
    };
    uint32 pc;
    union CCR {
      uint8_t value = 0;
      BooleanBitField<uint8_t, 0> c;  //carry
      BooleanBitField<uint8_t, 1> v;  //overflow
      BooleanBitField<uint8_t, 2> z;  //zero
      BooleanBitField<uint8_t, 3> n;  //negative
      BooleanBitField<uint8_t, 4> x;  //extend
    } ccr;
  } r;

  uint instructionsExecuted = 0;
};

}
