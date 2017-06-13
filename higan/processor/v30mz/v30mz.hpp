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
  auto alAdc (Size, uint16, uint16) -> uint16;
  auto alAdd (Size, uint16, uint16) -> uint16;
  auto alAnd (Size, uint16, uint16) -> uint16;
  auto alDec (Size, uint16        ) -> uint16;
  auto alDiv (Size, uint32, uint32) -> uint32;
  auto alDivi(Size,  int32,  int32) -> uint32;
  auto alInc (Size, uint16        ) -> uint16;
  auto alMul (Size, uint16, uint16) -> uint32;
  auto alMuli(Size,  int16,  int16) -> uint32;
  auto alNeg (Size, uint16        ) -> uint16;
  auto alNot (Size, uint16        ) -> uint16;
  auto alOr  (Size, uint16, uint16) -> uint16;
  auto alRcl (Size, uint16,  uint5) -> uint16;
  auto alRcr (Size, uint16,  uint5) -> uint16;
  auto alRol (Size, uint16,  uint4) -> uint16;
  auto alRor (Size, uint16,  uint4) -> uint16;
  auto alSal (Size, uint16,  uint5) -> uint16;
  auto alSar (Size, uint16,  uint5) -> uint16;
  auto alSbb (Size, uint16, uint16) -> uint16;
  auto alSub (Size, uint16, uint16) -> uint16;
  auto alShl (Size, uint16,  uint5) -> uint16;
  auto alShr (Size, uint16,  uint5) -> uint16;
  auto alXor (Size, uint16, uint16) -> uint16;

  //instructions-adjust.cpp
  auto opDecimalAdjust(bool);
  auto opAsciiAdjust(bool);
  auto opAdjustAfterMultiply();
  auto opAdjustAfterDivide();

  //instructions-alu.cpp
  auto opAddMemReg(Size);
  auto opAddRegMem(Size);
  auto opAddAccImm(Size);
  auto opOrMemReg(Size);
  auto opOrRegMem(Size);
  auto opOrAccImm(Size);
  auto opAdcMemReg(Size);
  auto opAdcRegMem(Size);
  auto opAdcAccImm(Size);
  auto opSbbMemReg(Size);
  auto opSbbRegMem(Size);
  auto opSbbAccImm(Size);
  auto opAndMemReg(Size);
  auto opAndRegMem(Size);
  auto opAndAccImm(Size);
  auto opSubMemReg(Size);
  auto opSubRegMem(Size);
  auto opSubAccImm(Size);
  auto opXorMemReg(Size);
  auto opXorRegMem(Size);
  auto opXorAccImm(Size);
  auto opCmpMemReg(Size);
  auto opCmpRegMem(Size);
  auto opCmpAccImm(Size);
  auto opTestMemReg(Size);
  auto opTestAcc(Size);
  auto opMultiplySignedRegMemImm(Size);
  auto opIncReg(uint16_t&);
  auto opDecReg(uint16_t&);
  auto opSignExtendByte();
  auto opSignExtendWord();

  //instructions-exec.cpp
  auto opLoop();
  auto opLoopWhile(bool);
  auto opJumpShort();
  auto opJumpIf(bool);
  auto opJumpNear();
  auto opJumpFar();
  auto opCallNear();
  auto opCallFar();
  auto opReturn();
  auto opReturnImm();
  auto opReturnFar();
  auto opReturnFarImm();
  auto opReturnInt();
  auto opInt3();
  auto opIntImm();
  auto opInto();
  auto opEnter();
  auto opLeave();
  auto opPushReg(uint16_t&);
  auto opPopReg(uint16_t&);
  auto opPushFlags();
  auto opPopFlags();
  auto opPushAll();
  auto opPopAll();
  auto opPushImm(Size);
  auto opPopMem();

  //instructions-flag.cpp
  auto opStoreFlagsAcc();
  auto opLoadAccFlags();
  auto opComplementCarry();
  auto opClearFlag(uint);
  auto opSetFlag(uint);

  //instructions-group.cpp
  auto opGroup1MemImm(Size, bool);
  auto opGroup2MemImm(Size, maybe<uint8> = {});
  auto opGroup3MemImm(Size);
  auto opGroup4MemImm(Size);

  //instructions-misc.cpp
  auto opSegment(uint16);
  auto opRepeat(bool);
  auto opLock();
  auto opWait();
  auto opHalt();
  auto opNop();
  auto opIn(Size);
  auto opOut(Size);
  auto opInDX(Size);
  auto opOutDX(Size);
  auto opTranslate();
  auto opBound();

  //instructions-move.cpp
  auto opMoveMemReg(Size);
  auto opMoveRegMem(Size);
  auto opMoveMemSeg();
  auto opMoveSegMem();
  auto opMoveAccMem(Size);
  auto opMoveMemAcc(Size);
  auto opMoveRegImm(uint8_t&);
  auto opMoveRegImm(uint16_t&);
  auto opMoveMemImm(Size);
  auto opExchange(uint16_t&, uint16_t&);
  auto opExchangeMemReg(Size);
  auto opLoadEffectiveAddressRegMem();
  auto opLoadSegmentMem(uint16_t&);

  //instructions-string.cpp
  auto opInString(Size);
  auto opOutString(Size);
  auto opMoveString(Size);
  auto opCompareString(Size);
  auto opStoreString(Size);
  auto opLoadString(Size);
  auto opScanString(Size);

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
