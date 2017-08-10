//NEC V30MZ

#pragma once

namespace Processor {

struct V30MZ {
  using Size = uint;
  enum : uint { Byte = 1, Word = 2, Long = 4 };
  enum : uint {
    SegmentOverrideES  = 0x26,
    SegmentOverrideCS  = 0x2e,
    SegmentOverrideSS  = 0x36,
    SegmentOverrideDS  = 0x3e,
    Lock               = 0xf0,
    RepeatWhileNotZero = 0xf2,
    RepeatWhileZero    = 0xf3,
  };

  virtual auto wait(uint clocks = 1) -> void = 0;
  virtual auto read(uint20 addr) -> uint8 = 0;
  virtual auto write(uint20 addr, uint8 data) -> void = 0;
  virtual auto in(uint16 port) -> uint8 = 0;
  virtual auto out(uint16 port, uint8 data) -> void = 0;

  auto warning(string text) -> void;
  auto power() -> void;
  auto exec() -> void;

  //instruction.cpp
  auto interrupt(uint8 vector) -> void;
  auto instruction() -> void;

  //registers.cpp
  auto repeat() -> uint8;
  auto segment(uint16) -> uint16;

  auto getAcc(Size) -> uint32;
  auto setAcc(Size, uint32) -> void;

  //modrm.cpp
  auto modRM() -> void;

  auto getMem(Size, uint offset = 0) -> uint16;
  auto setMem(Size, uint16) -> void;

  auto getReg(Size) -> uint16;
  auto setReg(Size, uint16) -> void;

  auto getSeg() -> uint16;
  auto setSeg(uint16) -> void;

  //memory.cpp
  auto read(Size, uint16, uint16) -> uint32;
  auto write(Size, uint16, uint16, uint16) -> void;

  auto in(Size, uint16) -> uint16;
  auto out(Size, uint16, uint16) -> void;

  auto fetch(Size = Byte) -> uint16;
  auto pop() -> uint16;
  auto push(uint16) -> void;

  //algorithms.cpp
  auto parity(uint8) const -> bool;
  auto ADC (Size, uint16, uint16) -> uint16;
  auto ADD (Size, uint16, uint16) -> uint16;
  auto AND (Size, uint16, uint16) -> uint16;
  auto DEC (Size, uint16        ) -> uint16;
  auto DIV (Size, uint32, uint32) -> uint32;
  auto DIVI(Size,  int32,  int32) -> uint32;
  auto INC (Size, uint16        ) -> uint16;
  auto MUL (Size, uint16, uint16) -> uint32;
  auto MULI(Size,  int16,  int16) -> uint32;
  auto NEG (Size, uint16        ) -> uint16;
  auto NOT (Size, uint16        ) -> uint16;
  auto OR  (Size, uint16, uint16) -> uint16;
  auto RCL (Size, uint16,  uint5) -> uint16;
  auto RCR (Size, uint16,  uint5) -> uint16;
  auto ROL (Size, uint16,  uint4) -> uint16;
  auto ROR (Size, uint16,  uint4) -> uint16;
  auto SAL (Size, uint16,  uint5) -> uint16;
  auto SAR (Size, uint16,  uint5) -> uint16;
  auto SBB (Size, uint16, uint16) -> uint16;
  auto SUB (Size, uint16, uint16) -> uint16;
  auto SHL (Size, uint16,  uint5) -> uint16;
  auto SHR (Size, uint16,  uint5) -> uint16;
  auto XOR (Size, uint16, uint16) -> uint16;

  //instructions-adjust.cpp
  auto instructionDecimalAdjust(bool) -> void;
  auto instructionAsciiAdjust(bool) -> void;
  auto instructionAdjustAfterMultiply() -> void;
  auto instructionAdjustAfterDivide() -> void;

  //instructions-alu.cpp
  auto instructionAddMemReg(Size) -> void;
  auto instructionAddRegMem(Size) -> void;
  auto instructionAddAccImm(Size) -> void;
  auto instructionOrMemReg(Size) -> void;
  auto instructionOrRegMem(Size) -> void;
  auto instructionOrAccImm(Size) -> void;
  auto instructionAdcMemReg(Size) -> void;
  auto instructionAdcRegMem(Size) -> void;
  auto instructionAdcAccImm(Size) -> void;
  auto instructionSbbMemReg(Size) -> void;
  auto instructionSbbRegMem(Size) -> void;
  auto instructionSbbAccImm(Size) -> void;
  auto instructionAndMemReg(Size) -> void;
  auto instructionAndRegMem(Size) -> void;
  auto instructionAndAccImm(Size) -> void;
  auto instructionSubMemReg(Size) -> void;
  auto instructionSubRegMem(Size) -> void;
  auto instructionSubAccImm(Size) -> void;
  auto instructionXorMemReg(Size) -> void;
  auto instructionXorRegMem(Size) -> void;
  auto instructionXorAccImm(Size) -> void;
  auto instructionCmpMemReg(Size) -> void;
  auto instructionCmpRegMem(Size) -> void;
  auto instructionCmpAccImm(Size) -> void;
  auto instructionTestMemReg(Size) -> void;
  auto instructionTestAcc(Size) -> void;
  auto instructionMultiplySignedRegMemImm(Size) -> void;
  auto instructionIncReg(uint16_t&) -> void;
  auto instructionDecReg(uint16_t&) -> void;
  auto instructionSignExtendByte() -> void;
  auto instructionSignExtendWord() -> void;

  //instructions-exec.cpp
  auto instructionLoop() -> void;
  auto instructionLoopWhile(bool) -> void;
  auto instructionJumpShort() -> void;
  auto instructionJumpIf(bool) -> void;
  auto instructionJumpNear() -> void;
  auto instructionJumpFar() -> void;
  auto instructionCallNear() -> void;
  auto instructionCallFar() -> void;
  auto instructionReturn() -> void;
  auto instructionReturnImm() -> void;
  auto instructionReturnFar() -> void;
  auto instructionReturnFarImm() -> void;
  auto instructionReturnInt() -> void;
  auto instructionInt3() -> void;
  auto instructionIntImm() -> void;
  auto instructionInto() -> void;
  auto instructionEnter() -> void;
  auto instructionLeave() -> void;
  auto instructionPushReg(uint16_t&) -> void;
  auto instructionPopReg(uint16_t&) -> void;
  auto instructionPushFlags() -> void;
  auto instructionPopFlags() -> void;
  auto instructionPushAll() -> void;
  auto instructionPopAll() -> void;
  auto instructionPushImm(Size) -> void;
  auto instructionPopMem() -> void;

  //instructions-flag.cpp
  auto instructionStoreFlagsAcc() -> void;
  auto instructionLoadAccFlags() -> void;
  auto instructionComplementCarry() -> void;
  auto instructionClearFlag(uint) -> void;
  auto instructionSetFlag(uint) -> void;

  //instructions-group.cpp
  auto instructionGroup1MemImm(Size, bool) -> void;
  auto instructionGroup2MemImm(Size, maybe<uint8> = {}) -> void;
  auto instructionGroup3MemImm(Size) -> void;
  auto instructionGroup4MemImm(Size) -> void;

  //instructions-misc.cpp
  auto instructionSegment(uint16) -> void;
  auto instructionRepeat(bool) -> void;
  auto instructionLock() -> void;
  auto instructionWait() -> void;
  auto instructionHalt() -> void;
  auto instructionNop() -> void;
  auto instructionIn(Size) -> void;
  auto instructionOut(Size) -> void;
  auto instructionInDX(Size) -> void;
  auto instructionOutDX(Size) -> void;
  auto instructionTranslate() -> void;
  auto instructionBound() -> void;

  //instructions-move.cpp
  auto instructionMoveMemReg(Size) -> void;
  auto instructionMoveRegMem(Size) -> void;
  auto instructionMoveMemSeg() -> void;
  auto instructionMoveSegMem() -> void;
  auto instructionMoveAccMem(Size) -> void;
  auto instructionMoveMemAcc(Size) -> void;
  auto instructionMoveRegImm(uint8_t&) -> void;
  auto instructionMoveRegImm(uint16_t&) -> void;
  auto instructionMoveMemImm(Size) -> void;
  auto instructionExchange(uint16_t&, uint16_t&) -> void;
  auto instructionExchangeMemReg(Size) -> void;
  auto instructionLoadEffectiveAddressRegMem() -> void;
  auto instructionLoadSegmentMem(uint16_t&) -> void;

  //instructions-string.cpp
  auto instructionInString(Size) -> void;
  auto instructionOutString(Size) -> void;
  auto instructionMoveString(Size) -> void;
  auto instructionCompareString(Size) -> void;
  auto instructionStoreString(Size) -> void;
  auto instructionLoadString(Size) -> void;
  auto instructionScanString(Size) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  //disassembler.cpp
  auto disassemble(uint16 cs, uint16 ip, bool registers = true, bool bytes = true) -> string;

  struct State {
    bool halt;    //set to true for hlt instruction; blocks execution until next interrupt
    bool poll;    //set to false to suppress interrupt polling between CPU instructions
    bool prefix;  //set to true for prefix instructions; prevents flushing of Prefix struct
  } state;

  uint8 opcode;
  vector<uint8> prefixes;

  struct ModRM {
    uint2 mod;
    uint3 reg;
    uint3 mem;

    uint16 segment;
    uint16 address;
  } modrm;

  struct Registers {
    union { uint16_t ax; struct { uint8_t order_lsb2(al, ah); }; };
    union { uint16_t cx; struct { uint8_t order_lsb2(cl, ch); }; };
    union { uint16_t dx; struct { uint8_t order_lsb2(dl, dh); }; };
    union { uint16_t bx; struct { uint8_t order_lsb2(bl, bh); }; };
    uint16_t sp;
    uint16_t bp;
    uint16_t si;
    uint16_t di;
    uint16_t es;
    uint16_t cs;
    uint16_t ss;
    uint16_t ds;
    uint16_t ip;

    uint8_t*  b[8]{&al, &cl, &dl, &bl, &ah, &ch, &dh, &bh};
    uint16_t* w[8]{&ax, &cx, &dx, &bx, &sp, &bp, &si, &di};
    uint16_t* s[8]{&es, &cs, &ss, &ds, &es, &cs, &ss, &ds};

    struct Flags {
      union {
        uint16_t data = 0;
        BooleanBitField<uint16_t, 15> m;  //mode
        BooleanBitField<uint16_t, 11> v;  //overflow
        BooleanBitField<uint16_t, 10> d;  //direction
        BooleanBitField<uint16_t,  9> i;  //interrupt
        BooleanBitField<uint16_t,  8> b;  //break
        BooleanBitField<uint16_t,  7> s;  //sign
        BooleanBitField<uint16_t,  6> z;  //zero
        BooleanBitField<uint16_t,  4> h;  //half-carry
        BooleanBitField<uint16_t,  2> p;  //parity
        BooleanBitField<uint16_t,  0> c;  //carry
      };

      operator uint() const { return data & 0x8fd5 | 0x7002; }
      auto& operator =(uint value) { return data  = value, *this; }
      auto& operator&=(uint value) { return data &= value, *this; }
      auto& operator|=(uint value) { return data |= value, *this; }
      auto& operator^=(uint value) { return data ^= value, *this; }
    } f;
  } r;
};

}
