//NEC V30MZ

#pragma once

namespace Processor {

struct V30MZ {
  virtual auto wait(uint clocks = 1) -> void = 0;
  virtual auto read(uint20 addr) -> uint8 = 0;
  virtual auto write(uint20 addr, uint8 data) -> void = 0;
  virtual auto in(uint16 port) -> uint8 = 0;
  virtual auto out(uint16 port, uint8 data) -> void = 0;

  auto exec() -> void;
  auto execOpcode() -> void;
  auto power() -> void;

  //registers.cpp
  auto regb(uint8) -> uint8&;
  auto regw(uint8) -> uint16&;
  auto sreg(uint8) -> uint16&;

  //memory.cpp
  auto readbIP() -> uint8;
  auto readwIP() -> uint16;

  auto readSP() -> uint16;
  auto writeSP(uint16) -> void;

  auto readb(uint20 ea) -> uint8;
  auto readw(uint20 ea) -> uint16;

  auto readb(uint16 rs, uint16 ea) -> uint8;
  auto readw(uint16 rs, uint16 ea) -> uint16;

  auto readModRM(uint8) -> uint20;
  auto readbModRM(uint8) -> uint8;
  auto readwModRM(uint8) -> uint16;

  //algorithms.cpp
  auto parity(uint16) const -> bool;

  auto albAnd(uint8, uint8) -> uint8;
  auto alwAnd(uint16, uint16) -> uint16;

  //instructions.cpp
  auto opbXorRegisterModRM();
  auto opwXorRegisterModRM();
  auto opJumpIf(bool);
  auto opbMoveRegisterModRM();
  auto opwMoveRegisterModRM();
  auto opMoveSegmentRegisterModRM();
  auto opNoOperation();
  auto opTestAL();
  auto opTestAX();
  auto opbMoveRegisterImmediate(uint8&);
  auto opwMoveRegisterImmediate(uint16&);
  auto opReturn();
  auto opbIn();
  auto opwIn();
  auto opbOut();
  auto opwOut();
  auto opCallNear();
  auto opJumpFar();
  auto opbInDX();
  auto opwInDX();
  auto opbOutDX();
  auto opwOutDX();
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
