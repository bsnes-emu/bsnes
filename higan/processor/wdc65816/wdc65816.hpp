//WDC 65C816 CPU core
//* Ricoh 5A22
//* Nintendo SA-1

#pragma once

namespace Processor {

struct WDC65816 {
  using fp = auto (WDC65816::*)() -> void;

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
  auto algorithmADC8() ->void;
  auto algorithmADC16() -> void;
  auto algorithmAND8() -> void;
  auto algorithmAND16() -> void;
  auto algorithmASL8() -> void;
  auto algorithmASL16() -> void;
  auto algorithmBIT8() -> void;
  auto algorithmBIT16() -> void;
  auto algorithmCMP8() -> void;
  auto algorithmCMP16() -> void;
  auto algorithmCPX8() -> void;
  auto algorithmCPX16() -> void;
  auto algorithmCPY8() -> void;
  auto algorithmCPY16() -> void;
  auto algorithmDEC8() -> void;
  auto algorithmDEC16() -> void;
  auto algorithmEOR8() -> void;
  auto algorithmEOR16() -> void;
  auto algorithmINC8() -> void;
  auto algorithmINC16() -> void;
  auto algorithmLDA8() -> void;
  auto algorithmLDA16() -> void;
  auto algorithmLDX8() -> void;
  auto algorithmLDX16() -> void;
  auto algorithmLDY8() -> void;
  auto algorithmLDY16() -> void;
  auto algorithmLSR8() -> void;
  auto algorithmLSR16() -> void;
  auto algorithmORA8() -> void;
  auto algorithmORA16() -> void;
  auto algorithmROL8() -> void;
  auto algorithmROL16() -> void;
  auto algorithmROR8() -> void;
  auto algorithmROR16() -> void;
  auto algorithmSBC8() -> void;
  auto algorithmSBC16() -> void;
  auto algorithmTRB8() -> void;
  auto algorithmTRB16() -> void;
  auto algorithmTSB8() -> void;
  auto algorithmTSB16() -> void;

  //instructions-read.cpp
  auto instructionImmediateRead8(fp) -> void;
  auto instructionImmediateRead16(fp) -> void;
  auto instructionBankRead8(fp) -> void;
  auto instructionBankRead16(fp) -> void;
  auto instructionBankRead8(fp, uint16) -> void;
  auto instructionBankRead16(fp, uint16) -> void;
  auto instructionLongRead8(fp, uint16 = 0) -> void;
  auto instructionLongRead16(fp, uint16 = 0) -> void;
  auto instructionDirectRead8(fp) -> void;
  auto instructionDirectRead16(fp) -> void;
  auto instructionDirectRead8(fp, uint16) -> void;
  auto instructionDirectRead16(fp, uint16) -> void;
  auto instructionIndirectRead8(fp) -> void;
  auto instructionIndirectRead16(fp) -> void;
  auto instructionIndexedIndirectRead8(fp) -> void;
  auto instructionIndexedIndirectRead16(fp) -> void;
  auto instructionIndirectIndexedRead8(fp) -> void;
  auto instructionIndirectIndexedRead16(fp) -> void;
  auto instructionIndirectLongRead8(fp, uint16 = 0) -> void;
  auto instructionIndirectLongRead16(fp, uint16 = 0) -> void;
  auto instructionStackRead8(fp) -> void;
  auto instructionStackRead16(fp) -> void;
  auto instructionIndirectStackRead8(fp) -> void;
  auto instructionIndirectStackRead16(fp) -> void;

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
  auto instructionINCImplied8(uint16&) -> void;
  auto instructionINCImplied16(uint16&) -> void;
  auto instructionDECImplied8(uint16&) -> void;
  auto instructionDECImplied16(uint16&) -> void;
  auto instructionASLImplied8() -> void;
  auto instructionASLImplied16() -> void;
  auto instructionLSRImplied8() -> void;
  auto instructionLSRImplied16() -> void;
  auto instructionROLImplied8() -> void;
  auto instructionROLImplied16() -> void;
  auto instructionRORImplied8() -> void;
  auto instructionRORImplied16() -> void;
  auto instructionBankModify8(fp op) -> void;
  auto instructionBankModify16(fp op) -> void;
  auto instructionBankIndexedModify8(fp op) -> void;
  auto instructionBankIndexedModify16(fp op) -> void;
  auto instructionDirectModify8(fp op) -> void;
  auto instructionDirectModify16(fp op) -> void;
  auto instructionDirectIndexedModify8(fp op) -> void;
  auto instructionDirectIndexedModify16(fp op) -> void;

  //instructions-pc.cpp
  auto instructionBranch(bool take = 1) -> void;
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
  auto instructionBlockMove8(int adjust) -> void;
  auto instructionBlockMove16(int adjust) -> void;
  auto instructionInterrupt(uint16) -> void;
  auto instructionSTP() -> void;
  auto instructionWAI() -> void;
  auto instructionXCE() -> void;
  auto instructionSetFlag(bool& flag) -> void;
  auto instructionClearFlag(bool& flag) -> void;
  auto instructionREP() -> void;
  auto instructionSEP() -> void;
  auto instructionTransfer8(uint16&, uint16&) -> void;
  auto instructionTransfer16(uint16&, uint16&) -> void;
  auto instructionTCS() -> void;
  auto instructionTSX8() -> void;
  auto instructionTSX16() -> void;
  auto instructionTXS() -> void;
  auto instructionPush8(uint16&) -> void;
  auto instructionPush16(uint16&) -> void;
  auto instructionPHD() -> void;
  auto instructionPHB() -> void;
  auto instructionPHK() -> void;
  auto instructionPHP() -> void;
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

  union Word {
    Word() : w(0) {}
    uint16 w;
    struct { uint8_t order_lsb2(l, h); };
  };

  union Long {
    Long() : d(0) {}
    uint32 d;
    struct { uint16_t order_lsb2(w, wh); };
    struct { uint8_t  order_lsb4(l, h, b, bh); };
  };

  struct Registers {
    Long pc;
    Word a;
    Word x;
    Word y;
    Word z;
    Word s;
    Word d;
    Flags p;
    uint8 db = 0;
    bool e = false;

    bool irq = false;   //IRQ pin (0 = low, 1 = trigger)
    bool wai = false;   //raised during wai, cleared after interrupt triggered
    bool stp = false;   //raised during stp, never cleared
    uint8 mdr = 0;      //memory data register
    uint16 vector = 0;  //interrupt vector address
  };

  Registers r;
  Long aa, rd;
  uint8 sp, dp;
};

}
