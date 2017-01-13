//Hudson Soft HuC6280A

#pragma once

namespace Processor {

struct HuC6280 {
  virtual auto step(uint clocks) -> void = 0;
  virtual auto read(uint16 addr) -> uint8 = 0;
  virtual auto write(uint16 addr, uint8 data) -> void = 0;
  virtual auto lastCycle() -> void = 0;

  auto power() -> void;

  //memory.cpp
  auto io() -> uint8;
  auto opcode() -> uint8;
  auto operand() -> uint8;

  //instruction.cpp
  auto instruction() -> void;

  //instructions.cpp
  auto instructionNOP() -> void;

  //disassembler.cpp
  virtual auto disassembleRead(uint16 pc) -> uint8 = 0;
  auto disassemble(uint16 pc) -> string;

  struct Flags {
    union {
      uint8_t data = 0;
      BooleanBitField<uint8_t, 0> c;  //carry
      BooleanBitField<uint8_t, 1> z;  //zero
      BooleanBitField<uint8_t, 2> i;  //interrupt
      BooleanBitField<uint8_t, 3> d;  //decimal
      BooleanBitField<uint8_t, 4> b;  //break
      BooleanBitField<uint8_t, 5> t;  //...
      BooleanBitField<uint8_t, 6> v;  //overflow
      BooleanBitField<uint8_t, 7> n;  //negative
    };

    inline operator uint() const { return data; }
    inline auto& operator =(uint value) { return data  = value, *this; }
    inline auto& operator&=(uint value) { return data &= value, *this; }
    inline auto& operator|=(uint value) { return data |= value, *this; }
    inline auto& operator^=(uint value) { return data ^= value, *this; }
  };

  struct Registers {
    uint8  a;
    uint8  x;
    uint8  y;
    uint8  s;
    uint16 pc;
    Flags  p;
    uint8  mdr;
  } r;
};

}
