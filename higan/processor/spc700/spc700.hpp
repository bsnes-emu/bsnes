#pragma once

namespace Processor {

struct SPC700 {
  virtual auto idle() -> void = 0;
  virtual auto read(uint16 addr) -> uint8 = 0;
  virtual auto write(uint16 addr, uint8 data) -> void = 0;
  virtual auto readDisassembler(uint16 addr) -> uint8 = 0;

  //spc700.cpp
  auto power() -> void;

  //instruction.cpp
  auto instruction() -> void;

  //memory.cpp
  auto fetch() -> uint8;
  auto pull() -> uint8;
  auto push(uint8 data) -> void;
  auto load(uint8 addr) -> uint8;
  auto store(uint8 addr, uint8 data) -> void;

  //algorithms.cpp
  auto algorithmADC(uint8, uint8) -> uint8;
  auto algorithmAND(uint8, uint8) -> uint8;
  auto algorithmASL(uint8) -> uint8;
  auto algorithmCMP(uint8, uint8) -> uint8;
  auto algorithmDEC(uint8) -> uint8;
  auto algorithmEOR(uint8, uint8) -> uint8;
  auto algorithmINC(uint8) -> uint8;
  auto algorithmLD (uint8, uint8) -> uint8;
  auto algorithmLSR(uint8) -> uint8;
  auto algorithmOR (uint8, uint8) -> uint8;
  auto algorithmROL(uint8) -> uint8;
  auto algorithmROR(uint8) -> uint8;
  auto algorithmSBC(uint8, uint8) -> uint8;
  auto algorithmST (uint8, uint8) -> uint8;
  auto algorithmADW(uint16, uint16) -> uint16;
  auto algorithmCPW(uint16, uint16) -> uint16;
  auto algorithmLDW(uint16, uint16) -> uint16;
  auto algorithmSBW(uint16, uint16) -> uint16;

  //instructions.cpp
  using fps = auto (SPC700::*)(uint8) -> uint8;
  using fpb = auto (SPC700::*)(uint8, uint8) -> uint8;
  using fpw = auto (SPC700::*)(uint16, uint16) -> uint16;

  auto instructionImpliedModify(fps, uint8&) -> void;
  auto instructionAbsoluteModify(fps) -> void;
  auto instructionDirectPageModify(fps) -> void;
  auto instructionDirectPageModifyWord(int) -> void;
  auto instructionDirectPageXModify(fps) -> void;
  auto instructionBranch(bool) -> void;
  auto instructionPull(uint8&) -> void;
  auto instructionPush(uint8) -> void;
  auto instructionAbsoluteRead(fpb, uint8&) -> void;
  auto instructionAbsoluteIndexedRead(fpb, uint8&) -> void;
  auto instructionImmediateRead(fpb, uint8&) -> void;
  auto instructionDirectPageRead(fpb, uint8&) -> void;
  auto instructionDirectPageIndexedRead(fpb, uint8&, uint8&) -> void;
  auto instructionDirectPageReadWord(fpw) -> void;
  auto instructionIndirectPageXRead(fpb) -> void;
  auto instructionIndirectPageYRead(fpb) -> void;
  auto instructionIndirectXRead(fpb) -> void;
  auto instructionAbsoluteModifyBit() -> void;
  auto instructionFlagClear(bool&) -> void;
  auto instructionFlagSet(bool&) -> void;
  auto instructionTransfer(uint8&, uint8&) -> void;
  auto instructionAbsoluteWrite(uint8&) -> void;
  auto instructionAbsoluteIndexedWrite(uint8&) -> void;
  auto instructionDirectPageWrite(uint8&) -> void;
  auto instructionDirectPageIndexedWrite(uint8&, uint8&) -> void;
  auto instructionDirectPageWriteImmediate(fpb) -> void;
  auto instructionDirectPageWriteDirectPage(fpb) -> void;
  auto instructionIndirectXWriteIndirectY(fpb) -> void;

  auto instructionBBC(uint3) -> void;
  auto instructionBBS(uint3) -> void;
  auto instructionBNEDirectPage() -> void;
  auto instructionBNEDirectPageDecrement() -> void;
  auto instructionBNEDirectPageX() -> void;
  auto instructionBNEYDecrement() -> void;
  auto instructionBRK() -> void;
  auto instructionCLR(uint3) -> void;
  auto instructionCLV() -> void;
  auto instructionCMC() -> void;
  auto instructionDAA() -> void;
  auto instructionDAS() -> void;
  auto instructionDIV() -> void;
  auto instructionJMPAbsolute() -> void;
  auto instructionJMPIndirectAbsoluteX() -> void;
  auto instructionJSPDirectPage() -> void;
  auto instructionJSRAbsolute() -> void;
  auto instructionJST(uint4) -> void;
  auto instructionLDAIndirectXIncrement() -> void;
  auto instructionMUL() -> void;
  auto instructionNOP() -> void;
  auto instructionPLP() -> void;
  auto instructionRTI() -> void;
  auto instructionRTS() -> void;
  auto instructionSET(uint3) -> void;
  auto instructionSTAIndirectPageX() -> void;
  auto instructionSTAIndirectPageY() -> void;
  auto instructionSTAIndirectX() -> void;
  auto instructionSTAIndirectXIncrement() -> void;
  auto instructionSTP() -> void;
  auto instructionSTWDirectPage() -> void;
  auto instructionTRBAbsolute() -> void;
  auto instructionTSBAbsolute() -> void;
  auto instructionWAI() -> void;
  auto instructionXCN() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  //disassembler.cpp
  auto disassemble(uint16 addr, bool p) -> string;

  struct Flags {
    bool c;  //carry
    bool z;  //zero
    bool i;  //interrupt disable
    bool h;  //half-carry
    bool b;  //break
    bool p;  //page
    bool v;  //overflow
    bool n;  //negative

    inline operator uint() const {
      return c << 0 | z << 1 | i << 2 | h << 3 | b << 4 | p << 5 | v << 6 | n << 7;
    }

    inline auto& operator=(uint8 data) {
      c = data.bit(0);
      z = data.bit(1);
      i = data.bit(2);
      h = data.bit(3);
      b = data.bit(4);
      p = data.bit(5);
      v = data.bit(6);
      n = data.bit(7);
      return *this;
    }
  };

  struct Registers {
    union Pair {
      Pair() : w(0) {}
      uint16 w;
      struct Byte { uint8 order_lsb2(l, h); } byte;
    } pc, ya;
    uint8 x, s;
    Flags p;
  } r;

  struct Register {
    union {
      uint16_t w = 0;
      NaturalBitField<uint16_t, 0,  7> l;
      NaturalBitField<uint16_t, 8, 15> h;
    };

    inline operator uint() const { return w; }
    inline auto operator=(const Register& value) { w = value.w; }

    inline auto operator++(int) { uint value = w++; return value; }
    inline auto operator--(int) { uint value = w--; return value; }

    inline auto& operator++() { return ++w, *this; }
    inline auto& operator--() { return --w, *this; }

    inline auto& operator =(uint value) { return w  = value, *this; }
    inline auto& operator&=(uint value) { return w &= value, *this; }
    inline auto& operator|=(uint value) { return w |= value, *this; }
    inline auto& operator^=(uint value) { return w ^= value, *this; }
    inline auto& operator+=(uint value) { return w += value, *this; }
    inline auto& operator-=(uint value) { return w -= value, *this; }
  };

  Register dp, sp, rd, wr, bit;
  uint8 opcode;
};

}
