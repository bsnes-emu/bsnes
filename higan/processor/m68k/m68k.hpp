#pragma once

//Motorola M68000

namespace Processor {

struct M68K {
  enum : uint { Byte, Word, Long };

  M68K();

  virtual auto step(uint clocks) -> void = 0;
  virtual auto read(bool word, uint24 addr) -> uint16 = 0;
  virtual auto write(bool word, uint24 addr, uint16 data) -> void = 0;

  auto power() -> void;
  auto reset() -> void;

  //memory.cpp
  auto readAbsolute(uint2 size, uint32 addr) -> uint32;
  auto writeAbsolute(uint2 size, uint32 addr, uint32 data) -> void;

  auto readPC(uint2 size = Word) -> uint32;

  template<uint Size> auto read(uint32 addr) -> uint32;

  //ea.cpp
  struct EA {
    EA(uint3 mode, uint3 reg) : mode(mode), reg(reg) {
      if(mode == 7) mode += reg;  //optimization: convert modes {7; 0-4} to {8-11}
    }

    uint4 mode;
    uint3 reg;
  };

  template<uint Size> auto read(EA ea) -> uint32;
  template<uint Size> auto write(EA ea, uint32 data) -> void;

  auto address(uint8 ea) -> uint32;
  auto read(uint8 ea) -> uint32;
  auto write(uint8 ea, uint32 data) -> void;
  auto modify(uint8 ea, uint32 data, const function<uint32 (uint32, uint32)>& op) -> uint32;
  auto flush(uint8 ea, uint32 address) -> void;

  //instruction.cpp
  auto trap() -> void;
  auto instruction() -> void;

  //instructions.cpp
  auto testCondition(uint4 condition) -> bool;

  template<uint Size> auto clip(uint32 data) -> uint32;
  template<uint Size> auto sign(uint32 data) -> int32;

  template<uint Size> auto carry(uint32 result, uint32 source) -> bool;
  template<uint Size> auto overflow(uint32 result, uint32 source, uint32 target) -> bool;
  template<uint Size> auto zero(uint32 result) -> bool;
  template<uint Size> auto negative(uint32 result) -> bool;
  auto zero(uint2 size, uint32 result) -> bool;
  auto negative(uint2 size, uint32 result) -> bool;

  template<uint Size> auto instructionADD(uint3 reg, uint1 direction, EA ea) -> void;
  auto instructionANDI(uint8 ea) -> void;
  auto instructionBCC(uint4 condition, uint8 displacement) -> void;
  auto instructionLEA(uint3 target, uint8 source) -> void;
  auto instructionMOVE(uint8 to, uint8 from) -> void;
  auto instructionMOVEA(uint3 to, uint8 ea) -> void;
  auto instructionMOVEM(uint1 direction, uint8 ea) -> void;
  auto instructionMOVEQ(uint3 target, uint8 immediate) -> void;
  auto instructionMOVE_USP(uint1 direction, uint3 reg) -> void;
  auto instructionNOP() -> void;
  auto instructionTST(uint8 ea) -> void;

  //disassembler.cpp
  auto disassemble(uint32 pc) -> string;
  auto disassembleRegisters() -> string;

  struct Registers {
    auto d(uint3 r) -> uint32&;
    auto a(uint3 r) -> uint32&;

    uint32 d0, d1, d2, d3, d4, d5, d6, d7;
    uint32 a0, a1, a2, a3, a4, a5, a6, ssp, usp;
    uint32 pc;

    union {
      uint16 sr;
      BooleanBitField<uint16_t,   0> c;  //carry
      BooleanBitField<uint16_t,   1> v;  //overflow
      BooleanBitField<uint16_t,   2> z;  //zero
      BooleanBitField<uint16_t,   3> n;  //negative
      BooleanBitField<uint16_t,   4> x;  //extend
      NaturalBitField<uint16_t,8,10> i;  //interrupt mask
      BooleanBitField<uint16_t,  13> s;  //supervisor mode
      BooleanBitField<uint16_t,  15> t;  //trace mode
    };

    Registers() : sr(0) {}
  } r;

  uint16 opcode = 0;
  uint instructionsExecuted = 0;

  function<void ()> instructionTable[65536];

private:
  //disassembler.cpp
  template<uint Size> auto disassembleADD(uint3 reg, uint1 direction, EA ea) -> string;
  auto disassembleANDI(uint8 ea) -> string;
  auto disassembleBCC(uint4 condition, uint8 displacement) -> string;
  auto disassembleLEA(uint3 target, uint8 ea) -> string;
  auto disassembleMOVE(uint8 to, uint8 from) -> string;
  auto disassembleMOVEA(uint3 to, uint8 from) -> string;
  auto disassembleMOVEM(uint1 direction, uint8 ea) -> string;
  auto disassembleMOVEQ(uint3 target, uint8 immediate) -> string;
  auto disassembleMOVE_USP(uint1 direction, uint3 reg) -> string;
  auto disassembleNOP() -> string;
  auto disassembleTST(uint8 ea) -> string;

  auto _readByte(uint32 addr) -> uint8;
  auto _readWord(uint32 addr) -> uint16;
  auto _readLong(uint32 addr) -> uint32;
  auto _readPC(uint2 size = Word) -> uint32;
  auto _immediate(uint2 size) -> string;
  auto _address(uint8 ea) -> string;
  auto _read(uint8 ea) -> string;
  auto _write(uint8 ea) -> string;
  auto _branch(uint8 displacement) -> string;
  auto _suffix(uint2 size) -> string;
  auto _condition(uint4 condition) -> string;

  uint32 _pc;
  function<string ()> disassembleTable[65536];
};

}
