//MOS Technologies MOS6502

#pragma once

namespace Processor {

struct MOS6502 {
  virtual auto read(uint16 addr) -> uint8 = 0;
  virtual auto write(uint16 addr, uint8 data) -> void = 0;
  virtual auto lastCycle() -> void = 0;
  virtual auto nmi(uint16& vector) -> void = 0;
  virtual auto readDebugger(uint16 addr) -> uint8 { return 0; }

  //mos6502.cpp
  auto mdr() const -> uint8;
  auto power() -> void;

  //memory.cpp
  auto idle() -> void;
  auto idlePageCrossed(uint16, uint16) -> void;
  auto idlePageAlways(uint16, uint16) -> void;
  auto opcode() -> uint8;
  auto operand() -> uint8;
  auto load(uint8 addr) -> uint8;
  auto store(uint8 addr, uint8 data) -> void;
  auto push(uint8 data) -> void;
  auto pull() -> uint8;

  //instruction.cpp
  auto interrupt(uint16 vector) -> void;
  auto instruction() -> void;

  //instructions.cpp
  using fp = auto (MOS6502::*)(uint8) -> uint8;
  auto ADC(uint8) -> uint8;
  auto AND(uint8) -> uint8;
  auto ASL(uint8) -> uint8;
  auto BIT(uint8) -> uint8;
  auto CMP(uint8) -> uint8;
  auto CPX(uint8) -> uint8;
  auto CPY(uint8) -> uint8;
  auto DEC(uint8) -> uint8;
  auto EOR(uint8) -> uint8;
  auto INC(uint8) -> uint8;
  auto LD (uint8) -> uint8;
  auto LSR(uint8) -> uint8;
  auto ORA(uint8) -> uint8;
  auto ROL(uint8) -> uint8;
  auto ROR(uint8) -> uint8;
  auto SBC(uint8) -> uint8;

  auto instructionAbsoluteModify(fp alu) -> void;
  auto instructionAbsoluteModify(fp alu, uint8 index) -> void;
  auto instructionAbsoluteRead(fp alu, uint8& data) -> void;
  auto instructionAbsoluteRead(fp alu, uint8& data, uint8 index) -> void;
  auto instructionAbsoluteWrite(uint8& data) -> void;
  auto instructionAbsoluteWrite(uint8& data, uint8 index) -> void;
  auto instructionBranch(bool take) -> void;
  auto instructionClear(bool& flag) -> void;
  auto instructionImmediate(fp alu, uint8& data) -> void;
  auto instructionImplied(fp alu, uint8& data) -> void;
  auto instructionIndirectXRead(fp alu, uint8& data) -> void;
  auto instructionIndirectXWrite(uint8& data) -> void;
  auto instructionIndirectYRead(fp alu, uint8& data) -> void;
  auto instructionIndirectYWrite(uint8& data) -> void;
  auto instructionPull(uint8& data) -> void;
  auto instructionPush(uint8& data) -> void;
  auto instructionSet(bool& flag) -> void;
  auto instructionTransfer(uint8& source, uint8& target, bool flag) -> void;
  auto instructionZeroPageModify(fp alu) -> void;
  auto instructionZeroPageModify(fp alu, uint8 index) -> void;
  auto instructionZeroPageRead(fp alu, uint8& data) -> void;
  auto instructionZeroPageRead(fp alu, uint8& data, uint8 index) -> void;
  auto instructionZeroPageWrite(uint8& data) -> void;
  auto instructionZeroPageWrite(uint8& data, uint8 index) -> void;

  auto instructionBRK() -> void;
  auto instructionJMPAbsolute() -> void;
  auto instructionJMPIndirect() -> void;
  auto instructionJSRAbsolute() -> void;
  auto instructionNOP() -> void;
  auto instructionPHP() -> void;
  auto instructionPLP() -> void;
  auto instructionRTI() -> void;
  auto instructionRTS() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  //disassembler.cpp
  auto disassemble(uint16 pc) -> string;

  struct Flags {
    bool c;  //carry
    bool z;  //zero
    bool i;  //interrupt disable
    bool d;  //decimal mode
    bool v;  //overflow
    bool n;  //negative

    inline operator uint() const {
      return c << 0 | z << 1 | i << 2 | d << 3 | v << 6 | n << 7;
    }

    inline auto& operator=(uint8 data) {
      c = data.bit(0);
      z = data.bit(1);
      i = data.bit(2);
      d = data.bit(3);
      v = data.bit(6);
      n = data.bit(7);
      return *this;
    }
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

  bool BCD = true;
};

}
