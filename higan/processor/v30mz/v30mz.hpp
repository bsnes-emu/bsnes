//NEC V30MZ

#pragma once

namespace Processor {

struct V30MZ {
  using Size = const uint&;
  static const uint Byte;  //= 1
  static const uint Word;  //= 2

  virtual auto wait(uint clocks = 1) -> void = 0;
  virtual auto read(uint20 addr) -> uint8 = 0;
  virtual auto write(uint20 addr, uint8 data) -> void = 0;
  virtual auto in(uint16 port) -> uint8 = 0;
  virtual auto out(uint16 port, uint8 data) -> void = 0;

  auto exec() -> void;
  auto execOpcode() -> void;
  auto power() -> void;

  //registers.cpp
  auto getAcc(Size) -> uint16;
  auto setAcc(Size, uint16) -> void;

  //modrm.cpp
  auto getReg(Size, uint8) -> uint16;
  auto setReg(Size, uint8, uint16) -> void;

  auto getSeg(uint8) -> uint16;
  auto setSeg(uint8, uint16) -> void;

  auto getMemAddress(uint8) -> uint32;
  auto getMem(Size, uint8) -> uint16;
  auto setMem(Size, uint8, uint16) -> void;

  //memory.cpp
  auto read(Size, uint16, uint16) -> uint16;
  auto write(Size, uint16, uint16, uint16) -> void;

  auto fetch(Size = Byte) -> uint16;
  auto pop() -> uint16;
  auto push(uint16) -> void;

  //algorithms.cpp
  auto parity(uint16) const -> bool;
  auto alAdc(Size, uint16, uint16) -> uint16;
  auto alAdd(Size, uint16, uint16) -> uint16;
  auto alAnd(Size, uint16, uint16) -> uint16;
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

  //instructions.cpp
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
  auto opPrefix(uint16&);
  auto opSubMemReg(Size);
  auto opSubRegMem(Size);
  auto opSubAccImm(Size);
  auto opXorMemReg(Size);
  auto opXorRegMem(Size);
  auto opXorAccImm(Size);
  auto opCmpMemReg(Size);
  auto opCmpRegMem(Size);
  auto opCmpAccImm(Size);
  auto opIncReg(uint16&);
  auto opDecReg(uint16&);
  auto opPushReg(uint16&);
  auto opPopReg(uint16&);
  auto opJumpIf(bool);
  auto opGroup1MemImm(Size, bool);
  auto opMoveMemReg(Size);
  auto opMoveRegMem(Size);
  auto opMoveSegMem();
  auto opNop();
  auto opCallFar();
  auto opMoveAccMem(Size);
  auto opMoveMemAcc(Size);
  auto opMoveString(Size);
  auto opTestAcc(Size);
  auto opStoreString(Size);
  auto opMoveRegImm(uint8&);
  auto opMoveRegImm(uint16&);
  auto opReturn();
  auto opMoveMemImm(Size);
  auto opRetFar();
  auto opGroup2MemImm(Size, maybe<uint8> = {});
  auto opIn(Size);
  auto opOut(Size);
  auto opCallNear();
  auto opJumpFar();
  auto opJumpShort();
  auto opInDX(Size);
  auto opOutDX(Size);
  auto opRepeat(bool);
  auto opClearFlag(bool&);
  auto opSetFlag(bool&);

  //disassembler.cpp
  auto disassemble(uint16 cs, uint16 ip, bool registers = true, bool bytes = true) -> string;

  //state
  bool halt = false;
  uint executed = 0;

  struct Registers {
    //registers.cpp
    auto byte(uint3) -> uint8&;
    auto word(uint3) -> uint16&;
    auto segment(uint2) -> uint16&;

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
