//NEC V30MZ

#pragma once

namespace Processor {

struct V30MZ {
  using Size = const uint&;
  static const uint Byte;
  static const uint Word;

  virtual auto wait(uint clocks = 1) -> void = 0;
  virtual auto read(uint20 addr) -> uint8 = 0;
  virtual auto write(uint20 addr, uint8 data) -> void = 0;
  virtual auto in(uint16 port) -> uint8 = 0;
  virtual auto out(uint16 port, uint8 data) -> void = 0;

  auto exec() -> void;
  auto execOpcode() -> void;
  auto power() -> void;

  //registers.cpp
  auto getRegister(Size, uint8) -> uint16;
  auto setRegister(Size, uint8, uint16) -> void;

  auto getSegment(uint8) -> uint16;
  auto setSegment(uint8, uint16) -> void;

  //memory.cpp
  auto read(Size, uint16, uint16) -> uint16;
  auto write(Size, uint16, uint16, uint16) -> void;

  auto readIP(Size = Byte) -> uint16;

  auto readSP() -> uint16;
  auto writeSP(uint16) -> void;

  auto readModRM(uint8) -> uint32;
  auto readModRM(Size, uint8) -> uint16;

  //algorithms.cpp
  auto parity(uint16) const -> bool;
  auto alAnd(Size, uint16, uint16) -> uint16;

  //instructions.cpp
  auto opXorRegisterModRM(Size);
  auto opJumpIf(bool);
  auto opMoveRegisterModRM(Size);
  auto opMoveSegmentRegisterModRM();
  auto opNoOperation();
  auto opMoveString(Size);
  auto opTestAX(Size);
  auto opMoveRegisterImmediate(uint8&);
  auto opMoveRegisterImmediate(uint16&);
  auto opReturn();
  auto opIn(Size);
  auto opOut(Size);
  auto opCallNear();
  auto opJumpFar();
  auto opInDX(Size);
  auto opOutDX(Size);
  auto opRepeat(bool);
  auto opClearFlag(bool&);
  auto opSetFlag(bool&);

  //disassembler.cpp
  auto disassemble(uint16 cs, uint16 ip, bool registers = true) -> string;

  //state
  bool halt = false;
  uint executed = 0;

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

    struct Flags {
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
