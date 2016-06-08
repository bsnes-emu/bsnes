//NEC uPD7720 (not supported)
//NEC uPD7725
//NEC uPD96050

#pragma once

namespace Processor {

struct uPD96050 {
  auto power() -> void;
  auto exec() -> void;
  auto serialize(serializer&) -> void;

  auto execOP(uint24 opcode) -> void;
  auto execRT(uint24 opcode) -> void;
  auto execJP(uint24 opcode) -> void;
  auto execLD(uint24 opcode) -> void;

  auto readSR() -> uint8;
  auto writeSR(uint8 data) -> void;

  auto readDR() -> uint8;
  auto writeDR(uint8 data) -> void;

  auto readDP(uint12 addr) -> uint8;
  auto writeDP(uint12 addr, uint8 data) -> void;

  auto disassemble(uint14 ip) -> string;

  enum class Revision : uint { uPD7725, uPD96050 } revision;
  uint24 programROM[16384];
  uint16 dataROM[2048];
  uint16 dataRAM[2048];

  struct Flag {
    union {
      uint8_t data = 0;
      BitField<uint8_t, 5> s1;
      BitField<uint8_t, 4> s0;
      BitField<uint8_t, 3> c;
      BitField<uint8_t, 2> z;
      BitField<uint8_t, 1> ov1;
      BitField<uint8_t, 0> ov0;
    };

    inline operator uint() const { return data & 0x3f; }
    inline auto& operator=(uint value) { return data = value, *this; }
    inline auto& operator=(const Flag& value) { return data = value.data, *this; }
  };

  struct Status {
    union {
      uint16_t data = 0;
      BitField<uint16_t, 15> rqm;
      BitField<uint16_t, 14> usf1;
      BitField<uint16_t, 13> usf0;
      BitField<uint16_t, 12> drs;
      BitField<uint16_t, 11> dma;
      BitField<uint16_t, 10> drc;
      BitField<uint16_t,  9> soc;
      BitField<uint16_t,  8> sic;
      BitField<uint16_t,  7> ei;
      BitField<uint16_t,  1> p1;
      BitField<uint16_t,  0> p0;
    };

    inline operator uint() const { return data & 0xff83; }
    inline auto& operator=(uint value) { return data = value, *this; }
  };

  struct Regs {
    uint16 stack[16];    //LIFO
    VariadicNatural pc;  //program counter
    VariadicNatural rp;  //ROM pointer
    VariadicNatural dp;  //data pointer
    uint4 sp;            //stack pointer
    int16 k;
    int16 l;
    int16 m;
    int16 n;
    int16 a;             //accumulator
    int16 b;             //accumulator
    Flag flaga;
    Flag flagb;
    uint16 tr;           //temporary register
    uint16 trb;          //temporary register
    Status sr;           //status register
    uint16 dr;           //data register
    uint16 si;
    uint16 so;
  } regs;
};

}
