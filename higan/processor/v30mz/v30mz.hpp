//NEC V30MZ

#pragma once

namespace Processor {

struct V30MZ {
  using Size = uint;
  enum : uint { Byte = 1, Word = 2, Long = 4 };
  struct ModRM;

  virtual auto wait(uint clocks = 1) -> void = 0;
  virtual auto read(uint20 addr) -> uint8 = 0;
  virtual auto write(uint20 addr, uint8 data) -> void = 0;
  virtual auto in(uint16 port) -> uint8 = 0;
  virtual auto out(uint16 port, uint8 data) -> void = 0;

  auto exec() -> void;
  auto instruction() -> void;
  auto interrupt(uint8 vector) -> void;
  auto power() -> void;

  //registers.cpp
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
  auto alAdc(Size, uint16, uint16) -> uint16;
  auto alAdd(Size, uint16, uint16) -> uint16;
  auto alAnd(Size, uint16, uint16) -> uint16;
  auto alDec(Size, uint16        ) -> uint16;
  auto alDiv(Size, uint32, uint32) -> uint32;
  auto alDivi(Size, int32,  int32) -> uint32;
  auto alInc(Size, uint16        ) -> uint16;
  auto alMul(Size, uint16, uint16) -> uint32;
  auto alMuli(Size, int16,  int16) -> uint32;
  auto alNeg(Size, uint16        ) -> uint16;
  auto alNot(Size, uint16        ) -> uint16;
  auto alOr (Size, uint16, uint16) -> uint16;
  auto alRcl(Size, uint16, uint5 ) -> uint16;
  auto alRcr(Size, uint16, uint5 ) -> uint16;
  auto alRol(Size, uint16, uint4 ) -> uint16;
  auto alRor(Size, uint16, uint4 ) -> uint16;
  auto alSal(Size, uint16, uint5 ) -> uint16;
  auto alSar(Size, uint16, uint5 ) -> uint16;
  auto alSbb(Size, uint16, uint16) -> uint16;
  auto alSub(Size, uint16, uint16) -> uint16;
  auto alShl(Size, uint16, uint5 ) -> uint16;
  auto alShr(Size, uint16, uint5 ) -> uint16;
  auto alXor(Size, uint16, uint16) -> uint16;

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
  auto opIncReg(uint16&);
  auto opDecReg(uint16&);
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
  auto opPushReg(uint16&);
  auto opPopReg(uint16&);
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
  auto opClearFlag(bool&);
  auto opSetFlag(bool&);

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
  auto opMoveRegImm(uint8&);
  auto opMoveRegImm(uint16&);
  auto opMoveMemImm(Size);
  auto opExchange(uint16&, uint16&);
  auto opExchangeMemReg(Size);
  auto opLoadEffectiveAddressRegMem();
  auto opLoadSegmentMem(uint16&);

  //instructions-string.cpp
  auto opInString(Size);
  auto opOutString(Size);
  auto opMoveString(Size);
  auto opCompareString(Size);
  auto opStoreString(Size);
  auto opLoadString(Size);
  auto opScanString(Size);

  //disassembler.cpp
  auto disassemble(uint16 cs, uint16 ip, bool registers = true, bool bytes = true) -> string;

  struct State {
    bool halt;
    bool prefix;
  } state;

  struct Prefix {
    maybe<uint16> segment;
    maybe<bool> repeat;
  } prefix;

  struct ModRM {
    uint2 mod;
    uint3 reg;
    uint3 mem;

    uint16 address;
    uint16 segment;
  } modrm;

  struct Registers {
    uint16 ip;
    union { uint16 ax; struct { uint8 order_lsb2(al, ah); }; };
    union { uint16 bx; struct { uint8 order_lsb2(bl, bh); }; };
    union { uint16 cx; struct { uint8 order_lsb2(cl, ch); }; };
    union { uint16 dx; struct { uint8 order_lsb2(dl, dh); }; };
    uint16 si;
    uint16 di;
    uint16 bp;
    uint16 sp;
    uint16 cs;
    uint16 ds;
    uint16 es;
    uint16 ss;

    uint8*  b[8]{&al, &cl, &dl, &bl, &ah, &ch, &dh, &bh};
    uint16* w[8]{&ax, &cx, &dx, &bx, &sp, &bp, &si, &di};
    uint16* s[8]{&es, &cs, &ss, &ds, &es, &cs, &ss, &ds};

    struct Flags {
      //registers.cpp
      operator uint16() const;
      auto operator=(uint16 data);

      bool m;  //mode
      bool v;  //overflow
      bool d;  //direction
      bool i;  //interrupt
      bool b;  //break
      bool s;  //sign
      bool z;  //zero
      bool h;  //half-carry
      bool p;  //parity
      bool c;  //carry
    } f;
  } r;
};

}
