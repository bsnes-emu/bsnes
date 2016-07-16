#pragma once

//Motorola M68000

namespace Processor {

struct M68K {
  enum : uint { Byte = 1, Word = 2, Long = 4 };

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

  auto read(uint size, uint32 addr) -> uint32;
  auto readPC(uint size = Word) -> uint32;

  //ea.cpp
  struct EA {
    uint mode;
    uint reg;

    boolean valid;
    uint32 address;
  };

  auto sign(uint size, uint32 data) -> int32;

  auto address(uint size, EA& ea) -> uint32;
  auto read(uint size, EA& ea) -> uint32;
  auto write(uint size, EA& ea, uint32 data) -> void;

  //instruction.cpp
  auto trap() -> void;
  auto instruction() -> void;

  //instructions.cpp
  auto testCondition(uint4 condition) -> bool;

  auto instructionANDI(uint size, EA modify) -> void;
  auto instructionBCC(uint condition, uint displacement) -> void;
  auto instructionLEA(uint target, EA source) -> void;
  auto instructionMOVE(uint size, EA target, EA source) -> void;
  auto instructionMOVEA(uint size, uint target, EA source) -> void;
  auto instructionMOVEM(uint direction, uint size, EA source) -> void;
  auto instructionMOVEQ(uint target, uint immediate) -> void;
  auto instructionMOVE_USP(uint direction, uint reg) -> void;
  auto instructionNOP() -> void;
  auto instructionTST(uint size, EA source) -> void;

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
  auto disassembleANDI(uint size, EA modify) -> string;
  auto disassembleBCC(uint condition, uint displacement) -> string;
  auto disassembleLEA(uint target, EA source) -> string;
  auto disassembleMOVE(uint size, EA target, EA source) -> string;
  auto disassembleMOVEA(uint size, uint target, EA source) -> string;
  auto disassembleMOVEM(uint direction, uint size, EA source) -> string;
  auto disassembleMOVEQ(uint target, uint immediate) -> string;
  auto disassembleMOVE_USP(uint direction, uint reg) -> string;
  auto disassembleNOP() -> string;
  auto disassembleTST(uint size, EA source) -> string;

  auto _readByte(uint32 addr) -> uint8;
  auto _readWord(uint32 addr) -> uint16;
  auto _readLong(uint32 addr) -> uint32;
  auto _readPC(uint size = Word) -> uint32;
  auto _immediate(uint size) -> string;
  auto _address(uint size, EA& ea) -> string;
  auto _read(uint size, EA& ea) -> string;
  auto _write(uint size, EA& ea) -> string;
  auto _branch(uint displacement) -> string;
  auto _suffix(uint size) -> string;
  auto _condition(uint condition) -> string;

  uint32 _pc;
  function<string ()> disassembleTable[65536];
};

}
