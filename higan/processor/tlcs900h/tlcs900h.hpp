//Toshiba TLCS900/H

#pragma once

namespace Processor {

struct TLCS900H {
  using Byte = uint8;
  using Word = uint16;
  using Long = uint32;

  struct Register  { Byte value; };
  struct Memory    { Long value; };
  struct Immediate { Long value; };

  virtual auto read(uint32 address) -> uint8 = 0;
  virtual auto write(uint32 address, uint8 data) -> void = 0;

  template<typename Size> auto read(Immediate immediate) -> Size { return immediate.value; }

  //tlcs900h.cpp
  auto power() -> void;

  //registers.cpp
  template<typename Size> auto map(Register register) -> maybe<Size&>;
  template<typename Size> auto read(Register) -> Size;
  template<typename Size> auto write(Register, Size data) -> void;

  //memory.cpp
  template<typename Size = Byte> auto fetch() -> Size;
  template<typename Size> auto read(Memory memory) -> Size;
  template<typename Size> auto write(Memory memory, Size data) -> void;

  //algorithms.cpp
  template<typename Size> auto algorithmAdd(Size target, Size source, uint1 carry = 0) -> Size;

  //instruction.cpp
  auto instruction() -> void;
  template<typename Size> auto instructionRegister(Register input) -> void;
  template<typename Size> auto instructionSourceMemory(Memory input) -> void;
  auto instructionTargetMemory(Memory input) -> void;

  //instructions.cpp
  template<typename Size, typename Target, typename Source> auto instructionAdd(Target target, Source source) -> void;
  template<typename Size, typename Target, typename Source> auto instructionAddCarry(Target target, Source source) -> void;
  auto instructionComplementCarry() -> void;
  auto instructionNoOperation() -> void;
  auto instructionSoftwareInterrupt(Immediate interrupt) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  struct DataRegister {
    union {
      struct { Long order_lsb1(l0); } l;
      struct { Word order_lsb2(w0, w1); } w;
      struct { Byte order_lsb4(b0, b1, b2, b3); } b;
    };
  };

  struct FlagRegister {
    uint1 c;  //carry
    uint1 n;  //negative
    uint1 v;  //overflow / parity
    uint1 h;  //half carry
    uint1 z;  //zero
    uint1 s;  //sign
  };

  struct StatusRegister {
    uint2 rfp;      //register file pointer
    uint2 rfpp;     //register file pointer - 1
    uint3 iff = 7;  //interrupt mask flip-flop
    FlagRegister f;
    FlagRegister fp;
  };

  struct Registers {
    DataRegister  xwa[4];
    DataRegister  xbc[4];
    DataRegister  xde[4];
    DataRegister  xhl[4];
    DataRegister  xix;
    DataRegister  xiy;
    DataRegister  xiz;
    DataRegister  xsp;
    DataRegister   pc;
    StatusRegister sr;
  } r;

  enum : uint {
    RA0, RW0, QA0, QW0, RC0, RB0, QC0, QB0, RE0, RD0, QE0, QD0, RL0, RH0, QL0, QH0,
    RA1, RW1, QA1, QW1, RC1, RB1, QC1, QB1, RE1, RD1, QE1, QD1, RL1, RH1, QL1, QH1,
    RA2, RW2, QA2, QW2, RC2, RB2, QC2, QB2, RE2, RD2, QE2, QD2, RL2, RH2, QL2, QH2,
    RA3, RW3, QA3, QW3, RC3, RB3, QC3, QB3, RE3, RD3, QE3, QD3, RL3, RH3, QL3, QH3, SPC = 0xcf,  //AP = 0xd0
     AP,  WP, QAP, QWP,  CP,  BP, QCP, QBP,  EP,  DP, QEP, QDP,  LP,  HP, QLP, QHP,
     A,   W,  QA,  QW,   C,   B,  QC,  QB,   E,   D,  QE,  QD,   L,   H,  QL,  QH,
    IXL, IXH, QIXL,QIXH,IYL, IYH, QIYL,QIYH,IZL, IZH, QIZL,QIZH,SPL, SPH, QSPL,QSPH,
  };

  enum : uint {
    RWA0 = 0x00, QWA0 = 0x02, RBC0 = 0x04, QBC0 = 0x06, RDE0 = 0x08, QDE0 = 0x0a, RHL0 = 0x0c, QHL0 = 0x0e,
    RWA1 = 0x10, QWA1 = 0x12, RBC1 = 0x14, QBC1 = 0x16, RDE1 = 0x18, QDE1 = 0x1a, RHL1 = 0x1c, QHL1 = 0x1e,
    RWA2 = 0x20, QWA2 = 0x22, RBC2 = 0x24, QBC2 = 0x26, RDE2 = 0x28, QDE2 = 0x2a, RHL2 = 0x2c, QHL2 = 0x2e,
    RWA3 = 0x30, QWA3 = 0x32, RBC3 = 0x34, QBC3 = 0x36, RDE3 = 0x38, QDE3 = 0x3a, RHL3 = 0x3c, QHL3 = 0x3e,
     WAP = 0xd0, QWAP = 0xd2,  BCP = 0xd4, QBCP = 0xd6,  DEP = 0xd8, QDEP = 0xda,  HLP = 0xdc, QHLP = 0xde,
     WA  = 0xe0,  QWA = 0xe2,   BC = 0xe4,  QBC = 0xe6,   DE = 0xe8,  QDE = 0xea,   HL = 0xec,  QHL = 0xee,
     IX  = 0xf0,  QIX = 0xf2,   IY = 0xf4,  QIY = 0xf6,   IZ = 0xf8,  QIZ = 0xfa,   SP = 0xfc,  QSP = 0xfe,
  };

  enum : uint {
    XWA0 = 0x00, XBC0 = 0x04, XDE0 = 0x08, XHL0 = 0x0c,
    XWA1 = 0x10, XBC1 = 0x14, XDE1 = 0x18, XHL1 = 0x1c,
    XWA2 = 0x20, XBC2 = 0x24, XDE2 = 0x28, XHL2 = 0x2c,
    XWA3 = 0x30, XBC3 = 0x34, XDE3 = 0x38, XHL3 = 0x3c,
    XWAP = 0xd0, XBCP = 0xd4, XDEP = 0xd8, XHLP = 0xdc,
    XWA  = 0xe0,  XBC = 0xe4,  XDE = 0xe8,  XHL = 0xec,
    XIX  = 0xf0,  XIY = 0xf4,  XIZ = 0xf8,  XSP = 0xfc,
  };

  const uint1 undefined = 0;
};

}
