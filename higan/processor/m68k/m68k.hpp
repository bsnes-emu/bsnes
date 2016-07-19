#pragma once

//Motorola M68000

namespace Processor {

struct M68K {
  enum : uint { Byte, Word, Long };
  enum : bool { NoUpdate = 0 };

  M68K();

  virtual auto step(uint clocks) -> void = 0;
  virtual auto read(bool word, uint24 addr) -> uint16 = 0;
  virtual auto write(bool word, uint24 addr, uint16 data) -> void = 0;

  auto power() -> void;
  auto reset() -> void;

  //registers.cpp
  struct Register {
    Register(uint number) : number(number) {}

    uint4 number;
  };

  template<uint Size = Long> auto read(Register reg) -> uint32;
  template<uint Size = Long> auto write(Register reg, uint32 value) -> void;

  //memory.cpp
  template<uint Size> auto read(uint32 addr) -> uint32;
  template<uint Size> auto write(uint32 addr, uint32 data) -> void;
  template<uint Size = Word> auto readPC() -> uint32;

  //ea.cpp
  struct EA {
    EA(uint mode_, uint reg_) : mode(mode_), reg(reg_) {
      if(mode == 7) mode += reg.number;  //optimization: convert modes {7; 0-4} to {8-11}
      if(mode != 0) reg.number += 8;     //optimization: linear index to all registers: d0-d7; a0-a7
    }

    uint4 mode;
    Register reg;

    boolean valid;
    uint32 address;
  };

  template<uint Size> auto fetch(EA& ea) -> uint32;
  template<uint Size, bool Update = 1> auto read(EA& ea) -> uint32;
  template<uint Size, bool Update = 1> auto write(EA& ea, uint32 data) -> void;
  template<uint Size> auto flush(EA& ea, uint32 data) -> void;

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

  template<uint Size> auto instructionADD(Register rd, uint1 direction, EA ea) -> void;
  template<uint Size> auto instructionANDI(EA ea) -> void;
                      auto instructionBCC(uint4 condition, uint8 displacement) -> void;
                      auto instructionLEA(Register ra, EA ea) -> void;
  template<uint Size> auto instructionMOVE(EA to, EA from) -> void;
  template<uint Size> auto instructionMOVEA(Register ra, EA ea) -> void;
  template<uint Size> auto instructionMOVEM(uint1 direction, EA ea) -> void;
                      auto instructionMOVEQ(Register rd, uint8 immediate) -> void;
                      auto instructionMOVE_USP(uint1 direction, Register ra) -> void;
                      auto instructionNOP() -> void;
  template<uint Size> auto instructionTST(EA ea) -> void;

  //disassembler.cpp
  auto disassemble(uint32 pc) -> string;
  auto disassembleRegisters() -> string;

  struct Registers {
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
  template<uint Size> auto disassembleADD(Register rd, uint1 direction, EA ea) -> string;
  template<uint Size> auto disassembleANDI(EA ea) -> string;
                      auto disassembleBCC(uint4 condition, uint8 displacement) -> string;
                      auto disassembleLEA(Register ra, EA ea) -> string;
  template<uint Size> auto disassembleMOVE(EA to, EA from) -> string;
  template<uint Size> auto disassembleMOVEA(Register ra, EA ea) -> string;
  template<uint Size> auto disassembleMOVEM(uint1 direction, EA ea) -> string;
                      auto disassembleMOVEQ(Register rd, uint8 immediate) -> string;
                      auto disassembleMOVE_USP(uint1 direction, Register ra) -> string;
                      auto disassembleNOP() -> string;
  template<uint Size> auto disassembleTST(EA ea) -> string;

  template<uint Size> auto _read(uint32 addr) -> uint32;
  template<uint Size = Word> auto _readPC() -> uint32;
  auto _register(Register r) -> string;
  template<uint Size> auto _immediate() -> string;
  template<uint Size> auto _address(EA ea) -> string;
  template<uint Size> auto _read(EA ea) -> string;
  template<uint Size> auto _write(EA ea) -> string;
  auto _branch(uint8 displacement) -> string;
  template<uint Size> auto _suffix() -> string;
  auto _condition(uint4 condition) -> string;

  uint32 _pc;
  function<string ()> disassembleTable[65536];
};

}
