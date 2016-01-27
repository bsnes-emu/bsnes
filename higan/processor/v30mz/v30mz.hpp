//NEC V30MZ

#pragma once

namespace Processor {

struct V30MZ {
  virtual auto step(uint clocks) -> void = 0;
  virtual auto read(uint32 addr) -> uint8 = 0;
  virtual auto write(uint32 addr, uint8 data) -> void = 0;

  auto exec() -> void;
  auto power() -> void;

  struct Registers {
    struct { uint16 ax; uint8 order_lsb2(al, ah); };
    struct { uint16 cx; uint8 order_lsb2(cl, ch); };
    struct { uint16 dx; uint8 order_lsb2(dl, dh); };
    struct { uint16 bx; uint8 order_lsb2(bl, bh); };
    uint16 sp;
    uint16 bp;
    uint16 si;
    uint16 di;
    uint16 es;
    uint16 cs;
    uint16 ss;
    uint16 ds;
  } r;
};

}
