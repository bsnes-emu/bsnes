#pragma once

//Motorola M68000

namespace Processor {

struct M68K {
  enum : bool { User, Supervisor };
  enum : uint { Byte, Word, Long };
  enum : bool { NoUpdate = 0, Reverse = 1 };

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
  struct DataRegister {
    explicit DataRegister(uint number_) : number(number_) {}
    uint3 number;
  };
  template<uint Size = Long> auto read(DataRegister reg) -> uint32;
  template<uint Size = Long> auto write(DataRegister reg, uint32 data) -> void;

  struct AddressRegister {
    explicit AddressRegister(uint number_) : number(number_) {}
    uint3 number;
  };
  template<uint Size = Long> auto read(AddressRegister reg) -> uint32;
  template<uint Size = Long> auto write(AddressRegister reg, uint32 data) -> void;

  auto readCCR() -> uint8;
  auto readSR() -> uint16;
  auto writeCCR(uint8 ccr) -> void;
  auto writeSR(uint16 sr) -> void;

  //memory.cpp
  template<uint Size> auto read(uint32 addr) -> uint32;
  template<uint Size, bool Order = 0> auto write(uint32 addr, uint32 data) -> void;
  template<uint Size = Word> auto readPC() -> uint32;
  template<uint Size> auto pop() -> uint32;
  template<uint Size> auto push(uint32 data) -> void;

  //effective-address.cpp
  struct EffectiveAddress {
    explicit EffectiveAddress(uint mode_, uint reg_) : mode(mode_), reg(reg_) {
      if(mode == 7) mode += reg;  //optimization: convert modes {7; 0-4} to {8-11}
    }

    uint4 mode;
    uint3 reg;

    boolean valid;
    uint32 address;
  };

  template<uint Size> auto fetch(EffectiveAddress& ea) -> uint32;
  template<uint Size, bool Update = 1> auto read(EffectiveAddress& ea) -> uint32;
  template<uint Size, bool Update = 1> auto write(EffectiveAddress& ea, uint32 data) -> void;
  template<uint Size> auto flush(EffectiveAddress& ea, uint32 data) -> void;

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

  template<uint Size> auto instructionADD(DataRegister dr, uint1 direction, EffectiveAddress ea) -> void;
  template<uint Size> auto instructionANDI(EffectiveAddress ea) -> void;
                      auto instructionANDI_TO_CCR() -> void;
                      auto instructionANDI_TO_SR() -> void;
                      auto instructionBCC(uint4 condition, uint8 displacement) -> void;
  template<uint Size> auto instructionBTST(DataRegister dr, EffectiveAddress ea) -> void;
  template<uint Size> auto instructionBTST(EffectiveAddress ea) -> void;
  template<uint Size> auto instructionCLR(EffectiveAddress ea) -> void;
  template<uint Size> auto instructionCMP(DataRegister dr, EffectiveAddress ea) -> void;
                      auto instructionDBCC(uint4 condition, DataRegister dr) -> void;
                      auto instructionEORI_TO_CCR() -> void;
                      auto instructionEORI_TO_SR() -> void;
                      auto instructionLEA(AddressRegister ar, EffectiveAddress ea) -> void;
  template<uint Size> auto instructionMOVE(EffectiveAddress to, EffectiveAddress from) -> void;
  template<uint Size> auto instructionMOVEA(AddressRegister ar, EffectiveAddress ea) -> void;
  template<uint Size> auto instructionMOVEM(uint1 direction, EffectiveAddress ea) -> void;
                      auto instructionMOVEQ(DataRegister dr, uint8 immediate) -> void;
                      auto instructionMOVE_FROM_SR(EffectiveAddress ea) -> void;
                      auto instructionMOVE_TO_CCR(EffectiveAddress ea) -> void;
                      auto instructionMOVE_TO_SR(EffectiveAddress ea) -> void;
                      auto instructionMOVE_USP(uint1 direction, AddressRegister ar) -> void;
                      auto instructionNOP() -> void;
                      auto instructionORI_TO_CCR() -> void;
                      auto instructionORI_TO_SR() -> void;
                      auto instructionRTS() -> void;
  template<uint Size> auto instructionTST(EffectiveAddress ea) -> void;

  //disassembler.cpp
  auto disassemble(uint32 pc) -> string;
  auto disassembleRegisters() -> string;

  struct Registers {
    uint32 d[8];
    uint32 a[8];
    uint32 sp;
    uint32 pc;

    bool c;   //carry
    bool v;   //overflow
    bool z;   //zero
    bool n;   //negative
    bool x;   //extend
    uint3 i;  //interrupt mask
    bool s;   //supervisor mode
    bool t;   //trace mode
  } r;

  uint16 opcode = 0;
  uint instructionsExecuted = 0;

  function<void ()> instructionTable[65536];

private:
  //disassembler.cpp
  template<uint Size> auto disassembleADD(DataRegister dr, uint1 direction, EffectiveAddress ea) -> string;
  template<uint Size> auto disassembleANDI(EffectiveAddress ea) -> string;
                      auto disassembleANDI_TO_CCR() -> string;
                      auto disassembleANDI_TO_SR() -> string;
                      auto disassembleBCC(uint4 condition, uint8 displacement) -> string;
  template<uint Size> auto disassembleBTST(DataRegister dr, EffectiveAddress ea) -> string;
  template<uint Size> auto disassembleBTST(EffectiveAddress ea) -> string;
  template<uint Size> auto disassembleCLR(EffectiveAddress ea) -> string;
  template<uint Size> auto disassembleCMP(DataRegister dr, EffectiveAddress ea) -> string;
                      auto disassembleDBCC(uint4 condition, DataRegister dr) -> string;
                      auto disassembleEORI_TO_CCR() -> string;
                      auto disassembleEORI_TO_SR() -> string;
                      auto disassembleLEA(AddressRegister ar, EffectiveAddress ea) -> string;
  template<uint Size> auto disassembleMOVE(EffectiveAddress to, EffectiveAddress from) -> string;
  template<uint Size> auto disassembleMOVEA(AddressRegister ar, EffectiveAddress ea) -> string;
  template<uint Size> auto disassembleMOVEM(uint1 direction, EffectiveAddress ea) -> string;
                      auto disassembleMOVEQ(DataRegister dr, uint8 immediate) -> string;
                      auto disassembleMOVE_FROM_SR(EffectiveAddress ea) -> string;
                      auto disassembleMOVE_TO_CCR(EffectiveAddress ea) -> string;
                      auto disassembleMOVE_TO_SR(EffectiveAddress ea) -> string;
                      auto disassembleMOVE_USP(uint1 direction, AddressRegister ar) -> string;
                      auto disassembleNOP() -> string;
                      auto disassembleORI_TO_CCR() -> string;
                      auto disassembleORI_TO_SR() -> string;
                      auto disassembleRTS() -> string;
  template<uint Size> auto disassembleTST(EffectiveAddress ea) -> string;

  template<uint Size> auto _read(uint32 addr) -> uint32;
  template<uint Size = Word> auto _readPC() -> uint32;
  auto _register(DataRegister dr) -> string;
  auto _register(AddressRegister ar) -> string;
  template<uint Size> auto _immediate() -> string;
  template<uint Size> auto _address(EffectiveAddress& ea) -> string;
  template<uint Size> auto _read(EffectiveAddress& ea) -> string;
  template<uint Size> auto _write(EffectiveAddress& ea) -> string;
  auto _branch(uint8 displacement) -> string;
  template<uint Size> auto _suffix() -> string;
  auto _condition(uint4 condition) -> string;

  uint32 _pc;
  function<string ()> disassembleTable[65536];
};

}
