//WDC 65C816 CPU core
//* Ricoh 5A22
//* Nintendo SA-1

#pragma once

namespace Processor {

struct WDC65816 {
  virtual auto idle() -> void = 0;
  virtual auto read(uint24 addr) -> uint8 = 0;
  virtual auto write(uint24 addr, uint8 data) -> void = 0;
  virtual auto lastCycle() -> void = 0;
  virtual auto interruptPending() const -> bool = 0;
  virtual auto interrupt() -> void;
  virtual auto synchronizing() const -> bool = 0;

  virtual auto readDisassembler(uint24 addr) -> uint8 { return 0; }

  //wdc65816.cpp
  auto power() -> void;

  //memory.cpp
  inline auto idleIRQ() -> void;
  inline auto idle2() -> void;
  inline auto idle4(uint16 x, uint16 y) -> void;
  inline auto idle6(uint16 addr) -> void;
  inline auto fetch() -> uint8;
  inline auto pull() -> uint8;
         auto push(uint8 data) -> void;
  inline auto pullN() -> uint8;
  inline auto pushN(uint8 data) -> void;
  inline auto readDirect(uint addr) -> uint8;
  inline auto writeDirect(uint addr, uint8 data) -> void;
  inline auto readDirectN(uint addr) -> uint8;
  inline auto readBank(uint addr) -> uint8;
  inline auto writeBank(uint addr, uint8 data) -> void;
  inline auto readStack(uint addr) -> uint8;
  inline auto writeStack(uint addr, uint8 data) -> void;

  //algorithms.cpp
  using alu8 = auto (WDC65816::*)(uint8) -> uint8;
  using alu16 = auto (WDC65816::*)(uint16) -> uint16;

  auto algorithmADC8(uint8) -> uint8;
  auto algorithmADC16(uint16) -> uint16;
  auto algorithmAND8(uint8) -> uint8;
  auto algorithmAND16(uint16) -> uint16;
  auto algorithmASL8(uint8) -> uint8;
  auto algorithmASL16(uint16) -> uint16;
  auto algorithmBIT8(uint8) -> uint8;
  auto algorithmBIT16(uint16) -> uint16;
  auto algorithmCMP8(uint8) -> uint8;
  auto algorithmCMP16(uint16) -> uint16;
  auto algorithmCPX8(uint8) -> uint8;
  auto algorithmCPX16(uint16) -> uint16;
  auto algorithmCPY8(uint8) -> uint8;
  auto algorithmCPY16(uint16) -> uint16;
  auto algorithmDEC8(uint8) -> uint8;
  auto algorithmDEC16(uint16) -> uint16;
  auto algorithmEOR8(uint8) -> uint8;
  auto algorithmEOR16(uint16) -> uint16;
  auto algorithmINC8(uint8) -> uint8;
  auto algorithmINC16(uint16) -> uint16;
  auto algorithmLDA8(uint8) -> uint8;
  auto algorithmLDA16(uint16) -> uint16;
  auto algorithmLDX8(uint8) -> uint8;
  auto algorithmLDX16(uint16) -> uint16;
  auto algorithmLDY8(uint8) -> uint8;
  auto algorithmLDY16(uint16) -> uint16;
  auto algorithmLSR8(uint8) -> uint8;
  auto algorithmLSR16(uint16) -> uint16;
  auto algorithmORA8(uint8) -> uint8;
  auto algorithmORA16(uint16) -> uint16;
  auto algorithmROL8(uint8) -> uint8;
  auto algorithmROL16(uint16) -> uint16;
  auto algorithmROR8(uint8) -> uint8;
  auto algorithmROR16(uint16) -> uint16;
  auto algorithmSBC8(uint8) -> uint8;
  auto algorithmSBC16(uint16) -> uint16;
  auto algorithmTRB8(uint8) -> uint8;
  auto algorithmTRB16(uint16) -> uint16;
  auto algorithmTSB8(uint8) -> uint8;
  auto algorithmTSB16(uint16) -> uint16;

  //instructions-read.cpp
  auto instructionImmediateRead8(alu8) -> void;
  auto instructionImmediateRead16(alu16) -> void;
  auto instructionBankRead8(alu8) -> void;
  auto instructionBankRead16(alu16) -> void;
  auto instructionBankRead8(alu8, uint16) -> void;
  auto instructionBankRead16(alu16, uint16) -> void;
  auto instructionLongRead8(alu8, uint16 = 0) -> void;
  auto instructionLongRead16(alu16, uint16 = 0) -> void;
  auto instructionDirectRead8(alu8) -> void;
  auto instructionDirectRead16(alu16) -> void;
  auto instructionDirectRead8(alu8, uint16) -> void;
  auto instructionDirectRead16(alu16, uint16) -> void;
  auto instructionIndirectRead8(alu8) -> void;
  auto instructionIndirectRead16(alu16) -> void;
  auto instructionIndexedIndirectRead8(alu8) -> void;
  auto instructionIndexedIndirectRead16(alu16) -> void;
  auto instructionIndirectIndexedRead8(alu8) -> void;
  auto instructionIndirectIndexedRead16(alu16) -> void;
  auto instructionIndirectLongRead8(alu8, uint16 = 0) -> void;
  auto instructionIndirectLongRead16(alu16, uint16 = 0) -> void;
  auto instructionStackRead8(alu8) -> void;
  auto instructionStackRead16(alu16) -> void;
  auto instructionIndirectStackRead8(alu8) -> void;
  auto instructionIndirectStackRead16(alu16) -> void;

  //instructions-write.cpp
  auto instructionBankWrite8(uint16&) -> void;
  auto instructionBankWrite16(uint16&) -> void;
  auto instructionBankWrite8(uint16&, uint16) -> void;
  auto instructionBankWrite16(uint16&, uint16) -> void;
  auto instructionLongWrite8(uint16 = 0) -> void;
  auto instructionLongWrite16(uint16 = 0) -> void;
  auto instructionDirectWrite8(uint16&) -> void;
  auto instructionDirectWrite16(uint16&) -> void;
  auto instructionDirectWrite8(uint16&, uint16) -> void;
  auto instructionDirectWrite16(uint16&, uint16) -> void;
  auto instructionIndirectWrite8() -> void;
  auto instructionIndirectWrite16() -> void;
  auto instructionIndexedIndirectWrite8() -> void;
  auto instructionIndexedIndirectWrite16() -> void;
  auto instructionIndirectIndexedWrite8() -> void;
  auto instructionIndirectIndexedWrite16() -> void;
  auto instructionIndirectLongWrite8(uint16 = 0) -> void;
  auto instructionIndirectLongWrite16(uint16 = 0) -> void;
  auto instructionStackWrite8() -> void;
  auto instructionStackWrite16() -> void;
  auto instructionIndirectStackWrite8() -> void;
  auto instructionIndirectStackWrite16() -> void;

  //instructions-modify.cpp
  auto instructionImpliedModify8(alu8, uint16&) -> void;
  auto instructionImpliedModify16(alu16, uint16&) -> void;
  auto instructionBankModify8(alu8) -> void;
  auto instructionBankModify16(alu16) -> void;
  auto instructionBankIndexedModify8(alu8) -> void;
  auto instructionBankIndexedModify16(alu16) -> void;
  auto instructionDirectModify8(alu8) -> void;
  auto instructionDirectModify16(alu16) -> void;
  auto instructionDirectIndexedModify8(alu8) -> void;
  auto instructionDirectIndexedModify16(alu16) -> void;

  //instructions-pc.cpp
  auto instructionBranch(bool = 1) -> void;
  auto instructionBRL() -> void;
  auto instructionJMPShort() -> void;
  auto instructionJMPLong() -> void;
  auto instructionJMPIndirect() -> void;
  auto instructionJMPIndexedIndirect() -> void;
  auto instructionJMPIndirectLong() -> void;
  auto instructionJSRShort() -> void;
  auto instructionJSRLong() -> void;
  auto instructionJSRIndexedIndirect() -> void;
  auto instructionRTI() -> void;
  auto instructionRTS() -> void;
  auto instructionRTL() -> void;

  //instructions-misc.cpp
  auto instructionBITImmediate8() -> void;
  auto instructionBITImmediate16() -> void;
  auto instructionNOP() -> void;
  auto instructionWDM() -> void;
  auto instructionXBA() -> void;
  auto instructionBlockMove8(int) -> void;
  auto instructionBlockMove16(int) -> void;
  auto instructionInterrupt(uint16) -> void;
  auto instructionSTP() -> void;
  auto instructionWAI() -> void;
  auto instructionXCE() -> void;
  auto instructionSetFlag(bool&) -> void;
  auto instructionClearFlag(bool&) -> void;
  auto instructionREP() -> void;
  auto instructionSEP() -> void;
  auto instructionTransfer8(uint16&, uint16&) -> void;
  auto instructionTransfer16(uint16&, uint16&) -> void;
  auto instructionTCS() -> void;
  auto instructionTSX8() -> void;
  auto instructionTSX16() -> void;
  auto instructionTXS() -> void;
  auto instructionPush8(uint8) -> void;
  auto instructionPush16(uint16) -> void;
  auto instructionPHD() -> void;
  auto instructionPull8(uint16&) -> void;
  auto instructionPull16(uint16&) -> void;
  auto instructionPLD() -> void;
  auto instructionPLB() -> void;
  auto instructionPLP() -> void;
  auto instructionPEA() -> void;
  auto instructionPEI() -> void;
  auto instructionPER() -> void;

  //instruction.cpp
  auto instruction() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  //disassembler.cpp
  auto disassemble() -> string;
  auto disassemble(uint24 addr, bool e, bool m, bool x) -> string;
  auto dreadb(uint24 addr) -> uint8;
  auto dreadw(uint24 addr) -> uint16;
  auto dreadl(uint24 addr) -> uint24;
  auto decode(uint8 mode, uint24 addr) -> uint24;

  struct Flags {
    bool c;  //carry
    bool z;  //zero
    bool i;  //interrupt disable
    bool d;  //decimal mode
    bool x;  //index register mode
    bool m;  //accumulator mode
    bool v;  //overflow
    bool n;  //negative

    inline operator uint() const {
      return c << 0 | z << 1 | i << 2 | d << 3 | x << 4 | m << 5 | v << 6 | n << 7;
    }

    inline auto& operator=(uint8 data) {
      c = data.bit(0);
      z = data.bit(1);
      i = data.bit(2);
      d = data.bit(3);
      x = data.bit(4);
      m = data.bit(5);
      v = data.bit(6);
      n = data.bit(7);
      return *this;
    }
  };

  struct Registers {
    uint24 pc;
    uint16 a;
    uint16 x;
    uint16 y;
    uint16 z;
    uint16 s;
    uint16 d;
    uint8 b;
    Flags p;
    bool e = false;

    bool irq = false;  //IRQ pin (0 = low, 1 = trigger)
    bool wai = false;  //raised during wai, cleared after interrupt triggered
    bool stp = false;  //raised during stp, never cleared
    uint8 mdr;         //memory data register
    uint16 vector;     //interrupt vector address
  } r;
};

}
