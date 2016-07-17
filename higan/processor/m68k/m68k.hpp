#pragma once

//Motorola M68000

namespace Processor {

struct M68K {
  enum : uint { Byte = 1, Word = 2, Long = 3 };

  M68K();

  virtual auto step(uint clocks) -> void = 0;
  virtual auto read(bool word, uint24 addr) -> uint16 = 0;
  virtual auto write(bool word, uint24 addr, uint16 data) -> void = 0;

  auto power() -> void;
  auto reset() -> void;

  auto sign(uint2 size, uint32 data) -> int32;

  //memory.cpp
  auto readAbsolute(uint2 size, uint32 addr) -> uint32;
  auto writeAbsolute(uint2 size, uint32 addr, uint32 data) -> void;

  auto readPC(uint2 size = Word) -> uint32;

  //ea.cpp
  struct EA {
    EA(M68K* self, uint2 size, uint3 mode, uint3 reg);
    ~EA();

    auto pc() -> uint32&;
    auto d(uint3 reg) -> uint32&;
    auto a(uint3 reg) -> uint32&;
    auto readPC(uint2 size) -> uint32;
    auto read(uint32 addr) -> uint32;
    auto write(uint32 addr, uint32 data) -> void;

    auto fetch() -> uint32;
    auto read() -> uint32;
    auto write(uint32 data) -> void;
    auto flush() -> void;

    uint2 size;
    uint4 mode;
    uint3 reg;

    uint32 address;
    M68K* self;
  };

  //instruction.cpp
  auto trap() -> void;
  auto instruction() -> void;

  //instructions.cpp
  auto testCondition(uint4 condition) -> bool;

  auto instructionANDI(uint2 size, uint3 mode, uint3 reg) -> void;
  auto instructionBCC(uint4 condition, uint8 displacement) -> void;
  auto instructionLEA(uint3 target, uint3 mode, uint3 reg) -> void;
  auto instructionMOVE(uint2 size, uint3 targetReg, uint3 targetMode, uint3 sourceMode, uint3 sourceReg) -> void;
  auto instructionMOVEA(uint2 size, uint3 target, uint3 mode, uint3 reg) -> void;
  auto instructionMOVEM(uint1 direction, uint2 size, uint3 mode, uint3 reg) -> void;
  auto instructionMOVEQ(uint3 target, uint8 immediate) -> void;
  auto instructionMOVE_USP(uint1 direction, uint3 reg) -> void;
  auto instructionNOP() -> void;
  auto instructionTST(uint2 size, uint3 mode, uint3 reg) -> void;

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
  auto disassembleANDI(uint2 size, uint3 mode, uint3 reg) -> string;
  auto disassembleBCC(uint4 condition, uint8 displacement) -> string;
  auto disassembleLEA(uint3 target, uint3 mode, uint3 reg) -> string;
  auto disassembleMOVE(uint2 size, uint3 targetReg, uint3 targetMode, uint3 sourceMode, uint3 sourceReg) -> string;
  auto disassembleMOVEA(uint2 size, uint3 target, uint3 mode, uint3 reg) -> string;
  auto disassembleMOVEM(uint1 direction, uint2 size, uint3 mode, uint3 reg) -> string;
  auto disassembleMOVEQ(uint3 target, uint8 immediate) -> string;
  auto disassembleMOVE_USP(uint1 direction, uint3 reg) -> string;
  auto disassembleNOP() -> string;
  auto disassembleTST(uint2 size, uint3 mode, uint3 reg) -> string;

  auto _readByte(uint32 addr) -> uint8;
  auto _readWord(uint32 addr) -> uint16;
  auto _readLong(uint32 addr) -> uint32;
  auto _readPC(uint size = Word) -> uint32;
  auto _immediate(uint size) -> string;
  auto _address(uint2 size, uint3 mode, uint3 reg) -> string;
  auto _read(uint2 size, uint3 mode, uint3 reg) -> string;
  auto _write(uint2 size, uint3 mode, uint3 reg) -> string;
  auto _branch(uint8 displacement) -> string;
  auto _suffix(uint size) -> string;
  auto _condition(uint4 condition) -> string;

  uint32 _pc;
  function<string ()> disassembleTable[65536];
};

}
