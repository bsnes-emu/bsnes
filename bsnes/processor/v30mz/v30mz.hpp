//NEC V30MZ (reduced functionality NEC V30 for embedded use)

//V30 missing instructions:
//  0f 10,11,18,19  test1
//  0f 12,13,1a,1b  clr1
//  0f 14,15,1c,1d  set1
//  0f 16,17,1e,1f  not1
//  0f 20           add4s
//  0f 22           sub4s
//  0f 26           cmp4s
//  0f 28           rol4
//  0f 2a           ror4
//  0f 31,39        ins
//  0f 33,3b        ext
//  0f ff           brkem (8080 emulation mode) [retem, calln]
//  64              repnc
//  65              repc
//  66,67           fpo2
//  d8-df           fpo1

//x86 variant instructions:
//  8f c0-c7  pop reg [CPU bug: pops from stack; fails to set register]
//  d4 xx     aam [ignores the immediate; always uses (base) 10]
//  d5 xx     aad [ignores the immediate; always uses (base) 10]
//  d6        xlat (mirror of d7) [this is salc on x86 CPUs]
//  f1        ??? [this is int 0x1 on x86 CPUs; said to be a two-byte NOP on V20; unknown on V30/V30MZ]
//  ff f8-ff  push (mirror of ff f0-f7)

//x86 unemulated variation:
//  after interrupts, NEC V20/V30 CPUs resume string instructions with prefixes intact. unlike x86 CPUs
//  I need more information on this behavior in order to emulate it ...
//  also, the opcode f1 behavior is not currently known

//V30 opcode prefix functionality:
//  there is a seven-level stack for opcode prefixes. once full, older prefixes are pushed off the stack

//other notes:
//  0f     pop cs (not nop) [on the V20; the V30 uses this for instruction extensions; unsure on the V30MZ]
//  8e xx  mov cs,modRM (works as expected; able to set CS)

//I currently emulate opcode 0f as pop cs, although it's unknown if that is correct.

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
    RepeatWhileZeroLo  = 0xf2,
    RepeatWhileZeroHi  = 0xf3,
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
  auto instructionRepeat() -> void;
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
  auto disassemble() -> string;
  auto disassemble(uint16 cs, uint16 ip) -> string;

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
