//NEC V30MZ

#pragma once

namespace Processor {

struct V30MZ {
  virtual auto wait(uint clocks = 1) -> void = 0;
  virtual auto read(uint20 addr) -> uint8 = 0;
  virtual auto write(uint20 addr, uint8 data) -> void = 0;
  virtual auto in(uint16 port) -> uint16 = 0;
  virtual auto out(uint16 port, uint16 data) -> void = 0;

  auto exec() -> void;
  auto execOpcode() -> void;
  auto power() -> void;

  //memory.cpp
  auto readByte() -> uint8;
  auto readWord() -> uint16;

  //instructions.cpp
  auto opJumpFar();
  auto opInByte();
  auto opInWord();
  auto opOutByte();
  auto opOutWord();
  auto opInDXByte();
  auto opInDXWord();
  auto opOutDXByte();
  auto opOutDXWord();
  auto opMoveRegisterImmediateByte(uint8&);
  auto opMoveRegisterImmediateWord(uint16&);
  auto opNoOperation();
  auto opClearFlag(bool&);
  auto opSetFlag(bool&);

  //disassembler.cpp
  auto disassemble(uint16 cs, uint16 ip, bool registers = true) -> string;

  //state
  bool halt = false;
  uint executed = 0;

  struct Registers {
    uint16 ip;
    union { struct { uint16 ax; uint8 order_lsb2(al, ah); }; };
    union { struct { uint16 bx; uint8 order_lsb2(bl, bh); }; };
    union { struct { uint16 cx; uint8 order_lsb2(cl, ch); }; };
    union { struct { uint16 dx; uint8 order_lsb2(dl, dh); }; };
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
