#pragma once

//Hitachi HG51B169 (HG51BS family/derivative?)

namespace Processor {

struct HG51B {
  auto exec(uint24 addr) -> void;
  virtual auto read(uint24 addr) -> uint8 = 0;
  virtual auto write(uint24 addr, uint8 data) -> void = 0;

  auto power() -> void;
  auto serialize(serializer&) -> void;

//uint16 programROM[2][256];
  uint24 dataROM[1024];
  uint8  dataRAM[3072];

protected:
  auto push() -> void;
  auto pull() -> void;
  auto sa() -> uint;
  auto ri() -> uint;
  auto np() -> uint;
  auto instruction() -> void;

  //registers.cpp
  auto regRead(uint8 addr) const -> uint24;
  auto regWrite(uint8 addr, uint24 data) -> void;

  struct Registers {
    bool halt;

    uint24 pc;
    uint16 p;
    bool n;
    bool z;
    bool c;

    uint24 a;
    uint24 acch;
    uint24 accl;
    uint24 busdata;
    uint24 romdata;
    uint24 ramdata;
    uint24 busaddr;
    uint24 ramaddr;
    uint24 gpr[16];
  } regs;

  uint24 stack[8];
  uint16 opcode;
};

}
