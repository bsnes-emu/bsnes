#pragma once

//Motorola M68000

namespace Processor {

struct M68K {
  M68K();

  virtual auto step(uint clocks) -> void = 0;
  virtual auto read(uint32 addr) -> uint8 = 0;
  virtual auto write(uint32 addr, uint8 data) -> void = 0;

  auto power() -> void;
  auto reset() -> void;

  //memory.cpp
  auto readByte(uint32 addr) -> uint8;
  auto readWord(uint32 addr) -> uint16;
  auto readLong(uint32 addr) -> uint32;

  auto readWordPC() -> uint16;
  auto readLongPC() -> uint32;

  auto readAbsolute(uint2 size, uint32 addr) -> uint32;

  //ea.cpp
  struct EA {
    EA(uint2 size, uint3 mode, uint3 reg) : size(size), mode(mode), reg(reg) {}

    uint2 size;
    uint3 mode;
    uint3 reg;

    boolean valid;
    uint32 address;
  };

  auto signExtend(uint2 size, uint32 data) -> int32;

  auto address(EA& ea) -> uint32;
  auto read(EA& ea) -> uint32;
  auto write(EA& ea, uint32 data) -> void;

  //instruction.cpp
  auto trap() -> void;
  auto instruction() -> void;

  //instructions.cpp
  auto testCondition(uint4 condition) -> bool;

  auto instructionBCC(uint4 condition, uint8 displacementByte) -> void;
  auto instructionLEA(uint3 wr, EA ea) -> void;
  auto instructionMOVEM(uint1 direction, EA ea) -> void;
  auto instructionNOP() -> void;
  auto instructionTST(EA ea) -> void;

  //disassembler.cpp
  auto disassemble(uint32 pc) -> string;
  auto disassembleRegisters() -> string;

  enum : uint { Byte = 0, Word = 1, Long = 2 };

  struct Registers {
    auto d(uint3 reg) -> uint32&;
    auto a(uint3 reg) -> uint32&;

    uint32 d0, d1, d2, d3, d4, d5, d6, d7;
    uint32 a0, a1, a2, a3, a4, a5, a6, usp, ssp;
    uint32 pc;

    union {
      uint8 ccr;
      BooleanBitField<uint8_t, 0> c;  //carry
      BooleanBitField<uint8_t, 1> v;  //overflow
      BooleanBitField<uint8_t, 2> z;  //zero
      BooleanBitField<uint8_t, 3> n;  //negative
      BooleanBitField<uint8_t, 4> x;  //extend
    };

    Registers() : ccr(0) {}
  } r;

  uint16 opcode = 0;
  uint instructionsExecuted = 0;

  function<void ()> instructionTable[65536];
};

}
