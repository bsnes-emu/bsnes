//Hudson Soft HuC6280

#pragma once

namespace Processor {

struct HuC6280 {
  virtual auto step(uint clocks) -> void = 0;
  virtual auto read(uint8 bank, uint13 addr) -> uint8 = 0;
  virtual auto write(uint8 bank, uint13 addr, uint8 data) -> void = 0;
  virtual auto store(uint2 addr, uint8 data) -> void = 0;
  virtual auto lastCycle() -> void = 0;

  //huc6280.cpp
  auto power() -> void;

  //memory.cpp
  inline auto load8(uint8) -> uint8;
  inline auto load16(uint16) -> uint8;
  inline auto store8(uint8, uint8) -> void;
  inline auto store16(uint16, uint8) -> void;

  auto io() -> void;
  auto opcode() -> uint8;
  auto operand() -> uint8;

  auto push(uint8) -> void;
  auto pull() -> uint8;

  //instructions.cpp
  using fp = auto (HuC6280::*)(uint8) -> uint8;
  auto algorithmADC(uint8) -> uint8;
  auto algorithmAND(uint8) -> uint8;
  auto algorithmASL(uint8) -> uint8;
  auto algorithmBIT(uint8) -> uint8;
  auto algorithmCMP(uint8) -> uint8;
  auto algorithmCPX(uint8) -> uint8;
  auto algorithmCPY(uint8) -> uint8;
  auto algorithmDEC(uint8) -> uint8;
  auto algorithmEOR(uint8) -> uint8;
  auto algorithmINC(uint8) -> uint8;
  auto algorithmLD (uint8) -> uint8;
  auto algorithmLSR(uint8) -> uint8;
  auto algorithmORA(uint8) -> uint8;
  auto algorithmROL(uint8) -> uint8;
  auto algorithmROR(uint8) -> uint8;
  auto algorithmSBC(uint8) -> uint8;
  auto algorithmTRB(uint8) -> uint8;
  auto algorithmTSB(uint8) -> uint8;

  using bp = auto (HuC6280::*)(uint16&, uint16&, bool) -> void;
  auto algorithmTAI(uint16&, uint16&, bool) -> void;
  auto algorithmTDD(uint16&, uint16&, bool) -> void;
  auto algorithmTIA(uint16&, uint16&, bool) -> void;
  auto algorithmTII(uint16&, uint16&, bool) -> void;
  auto algorithmTIN(uint16&, uint16&, bool) -> void;

  //instruction.cpp
  auto interrupt(uint16 vector) -> void;
  auto instruction() -> void;

  //instructions.cpp
  auto instructionAbsoluteModify(fp, uint8 = 0) -> void;
  auto instructionAbsoluteRead(fp, uint8&, uint8 = 0) -> void;
  auto instructionAbsoluteWrite(uint8, uint8 = 0) -> void;
  auto instructionBlockMove(bp) -> void;
  auto instructionBranch(bool) -> void;
  auto instructionBranchIfBitReset(uint3) -> void;
  auto instructionBranchIfBitSet(uint3) -> void;
  auto instructionBranchSubroutine() -> void;
  auto instructionBreak() -> void;
  auto instructionCallAbsolute() -> void;
  auto instructionChangeSpeedLow() -> void;
  auto instructionChangeSpeedHigh() -> void;
  auto instructionClear(uint8&) -> void;
  auto instructionClear(bool&) -> void;
  auto instructionImmediate(fp, uint8&) -> void;
  auto instructionImplied(fp, uint8&) -> void;
  auto instructionIndirectRead(fp, uint8&, uint8 = 0) -> void;
  auto instructionIndirectWrite(uint8, uint8 = 0) -> void;
  auto instructionIndirectYRead(fp, uint8&) -> void;
  auto instructionIndirectYWrite(uint8) -> void;
  auto instructionJumpAbsolute() -> void;
  auto instructionJumpIndirect(uint8 = 0) -> void;
  auto instructionMemory(fp) -> void;
  auto instructionNoOperation() -> void;
  auto instructionPull(uint8&) -> void;
  auto instructionPullP() -> void;
  auto instructionPush(uint8) -> void;
  auto instructionResetMemoryBit(uint3) -> void;
  auto instructionReturnInterrupt() -> void;
  auto instructionReturnSubroutine() -> void;
  auto instructionSet(bool&) -> void;
  auto instructionSetMemoryBit(uint3) -> void;
  auto instructionStoreImplied(uint2) -> void;
  auto instructionSwap(uint8&, uint8&) -> void;
  auto instructionTestAbsolute(uint8 = 0) -> void;
  auto instructionTestZeroPage(uint8 = 0) -> void;
  auto instructionTransfer(uint8&, uint8&) -> void;
  auto instructionTransferAccumulatorToMPR() -> void;
  auto instructionTransferMPRToAccumulator() -> void;
  auto instructionTransferXS() -> void;
  auto instructionZeroPageModify(fp, uint8 = 0) -> void;
  auto instructionZeroPageRead(fp, uint8&, uint8 = 0) -> void;
  auto instructionZeroPageWrite(uint8, uint8 = 0) -> void;

  //disassembler.cpp
  auto disassemble(uint16 pc) -> string;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  struct Flags {
    bool c;  //carry
    bool z;  //zero
    bool i;  //interrupt disable
    bool d;  //decimal mode
    bool b;  //break
    bool t;  //memory operation
    bool v;  //overflow
    bool n;  //negative

    inline operator uint8() const {
      return c << 0 | z << 1 | i << 2 | d << 3 | b << 4 | t << 5 | v << 6 | n << 7;
    }

    inline auto& operator=(uint8 data) {
      c = data.bit(0);
      z = data.bit(1);
      i = data.bit(2);
      d = data.bit(3);
      b = data.bit(4);
      t = data.bit(5);
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
    uint8  mpr[8];
    uint8  mdr;
    Flags  p;
    uint8  cs;  //code speed (3 = fast, 12 = slow)
  } r;
};

}
