#pragma once

//Hitachi HG51B169 (HG51BS family/derivative?)

namespace Processor {

struct HG51B {
  virtual auto step(uint clocks) -> void;
  virtual auto isROM(uint24 address) -> bool = 0;
  virtual auto isRAM(uint24 address) -> bool = 0;
  virtual auto read(uint24 address) -> uint8 = 0;
  virtual auto write(uint24 address, uint8 data) -> void = 0;
  virtual auto lock() -> void;
  virtual auto halt() -> void;
  auto wait(uint24 address) -> uint;
  auto main() -> void;
  auto execute() -> void;
  auto advance() -> void;
  auto suspend() -> void;
  auto cache() -> void;
  auto dma() -> void;
  auto running() const -> bool;
  auto busy() const -> bool;

  auto power() -> void;
  auto serialize(serializer&) -> void;

  uint16 programRAM[2][256];
  uint24 dataROM[1024];
  uint8  dataRAM[3072];

protected:
  auto push() -> void;
  auto pull() -> void;
  auto sa() -> uint;
  auto ri() -> uint;
  auto np() -> void;
  auto instruction() -> void;

  //registers.cpp
  auto readRegister(uint7 address) -> uint24;
  auto writeRegister(uint7 address, uint24 data) -> void;

  struct Registers {
    uint16 p;
    uint16 pb;  //program bank
    uint8  pc;  //program counter

    boolean n;  //negative
    boolean z;  //zero
    boolean c;  //carry
    boolean i;  //interrupt

    uint24 a;   //accumulator
    uint24 acch;
    uint24 accl;
    uint24 busData;
    uint24 romData;
    uint24 ramData;
    uint24 busAddress;
    uint24 ramAddress;
    uint24 gpr[16];
  } r;

  struct IO {
    uint1 lock;
    uint1 halt = 1;
    uint1 irq;      //0 = enable, 1 = disable
    uint1 rom = 1;  //0 = 2 ROMs, 1 = 1 ROM
    uint8 vector[32];

    struct Wait {
      uint3 rom = 3;
      uint3 ram = 3;
    } wait;

    struct Suspend {
      uint1 enable;
      uint8 duration;
    } suspend;

    struct Cache {
      uint1  enable;
      uint1  page;
      uint1  lock[2];
      uint24 address[2];
      uint24 base;
      uint16 pb;
      uint8  pc;
    } cache;

    struct DMA {
      uint1  enable;
      uint24 source;
      uint24 target;
      uint16 length;
    } dma;

    struct Bus {
      uint1  enable;
      uint1  reading;
      uint1  writing;
      uint4  pending;
      uint24 address;
    } bus;
  } io;

  uint24 stack[8];
  uint16 opcode;
};

}
