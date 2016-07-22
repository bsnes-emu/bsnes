#pragma once

//Motorola M68000

namespace Processor {

struct M68K {
  enum : bool { User, Supervisor };
  enum : uint { Byte, Word, Long };
  enum : bool { NoUpdate = 0, Reverse = 1 };
  enum : uint { D0, D1, D2, D3, D4, D5, D6, D7, A0, A1, A2, A3, A4, A5, A6, A7 };

  enum : uint {
    DataRegisterDirect,
    AddressRegisterDirect,
    AddressRegisterIndirect,
    AddressRegisterIndirectWithPostIncrement,
    AddressRegisterIndirectWithPreDecrement,
    AddressRegisterIndirectWithDisplacement,
    AddressRegisterIndirectWithIndex,
    AbsoluteShortIndirect,
    AbsoluteLongIndirect,
    ProgramCounterIndirectWithDisplacement,
    ProgramCounterIndirectWithIndex,
    Immediate,
  };

  M68K();

  virtual auto step(uint clocks) -> void = 0;
  virtual auto read(bool word, uint24 addr) -> uint16 = 0;
  virtual auto write(bool word, uint24 addr, uint16 data) -> void = 0;

  auto power() -> void;
  auto reset() -> void;
  auto supervisor() -> bool;

  //registers.cpp
  struct Register {
    explicit Register(uint number_) : number(number_) {}

    uint4 number;
  };

  template<uint Size = Long> auto read(Register reg) -> uint32;
  template<uint Size = Long> auto write(Register reg, uint32 value) -> void;
  auto setSR(uint16 sr) -> void;

  //memory.cpp
  template<uint Size> auto read(uint32 addr) -> uint32;
  template<uint Size, bool Order = 0> auto write(uint32 addr, uint32 data) -> void;
  template<uint Size = Word> auto readPC() -> uint32;
  template<uint Size> auto pop() -> uint32;
  template<uint Size> auto push(uint32 data) -> void;

  //ea.cpp
  struct EA {
    explicit EA(uint mode_, uint reg_) : mode(mode_), reg(reg_) {
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

  template<uint Size> auto bits() -> uint;
  template<uint Size> auto mask() -> uint32;
  template<uint Size> auto clip(uint32 data) -> uint32;
  template<uint Size> auto sign(uint32 data) -> int32;

  template<uint Size> auto carry(uint32 result, uint32 source) -> bool;
  template<uint Size> auto overflow(uint32 result, uint32 source, uint32 target) -> bool;
  template<uint Size> auto zero(uint32 result) -> bool;
  template<uint Size> auto negative(uint32 result) -> bool;

  template<uint Size> auto instructionADD(Register rd, uint1 direction, EA ea) -> void;
  template<uint Size> auto instructionANDI(EA ea) -> void;
                      auto instructionBCC(uint4 condition, uint8 displacement) -> void;
  template<uint Size> auto instructionBTST(Register rd, EA ea) -> void;
  template<uint Size> auto instructionBTST(EA ea) -> void;
  template<uint Size> auto instructionCLR(EA ea) -> void;
  template<uint Size> auto instructionCMP(Register rd, EA ea) -> void;
                      auto instructionDBCC(uint4 condition, Register rd) -> void;
                      auto instructionLEA(Register ra, EA ea) -> void;
  template<uint Size> auto instructionMOVE(EA to, EA from) -> void;
  template<uint Size> auto instructionMOVEA(Register ra, EA ea) -> void;
  template<uint Size> auto instructionMOVEM(uint1 direction, EA ea) -> void;
                      auto instructionMOVEQ(Register rd, uint8 immediate) -> void;
                      auto instructionMOVE_FROM_SR(EA ea) -> void;
                      auto instructionMOVE_TO_SR(EA ea) -> void;
                      auto instructionMOVE_USP(uint1 direction, Register ra) -> void;
                      auto instructionNOP() -> void;
                      auto instructionRTS() -> void;
  template<uint Size> auto instructionTST(EA ea) -> void;

  //disassembler.cpp
  auto disassemble(uint32 pc) -> string;
  auto disassembleRegisters() -> string;

  struct Registers {
    uint32 da[16];  //a7 = primary stack pointer
    uint32 sp;      //sp = secondary stack pointer
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
  template<uint Size> auto disassembleBTST(Register rd, EA ea) -> string;
  template<uint Size> auto disassembleBTST(EA ea) -> string;
  template<uint Size> auto disassembleCLR(EA ea) -> string;
  template<uint Size> auto disassembleCMP(Register rd, EA ea) -> string;
                      auto disassembleDBCC(uint4 condition, Register rd) -> string;
                      auto disassembleLEA(Register ra, EA ea) -> string;
  template<uint Size> auto disassembleMOVE(EA to, EA from) -> string;
  template<uint Size> auto disassembleMOVEA(Register ra, EA ea) -> string;
  template<uint Size> auto disassembleMOVEM(uint1 direction, EA ea) -> string;
                      auto disassembleMOVEQ(Register rd, uint8 immediate) -> string;
                      auto disassembleMOVE_FROM_SR(EA ea) -> string;
                      auto disassembleMOVE_TO_SR(EA ea) -> string;
                      auto disassembleMOVE_USP(uint1 direction, Register ra) -> string;
                      auto disassembleNOP() -> string;
                      auto disassembleRTS() -> string;
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
