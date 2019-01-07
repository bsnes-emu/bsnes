//Toshiba TLCS900/H

/* open questions:
 *
 * what happens when a prohibited instruction operand size is used? (eg adc.l (memory),#immediate)
 * what happens when %11 is used for pre-decrement and post-increment addressing?
 * what happens when using 8-bit register indexing and d0 is set (Word) or d1/d0 is set (Long)?
 */

#pragma once

namespace Processor {

struct TLCS900H {
  using Byte =  uint8;
  using Word = uint16;
  using Long = uint32;

  struct Register  { Byte value; };
  struct Memory    { Long value; };
  struct Immediate { Long value; };

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

  virtual auto read(uint32 address) -> uint8 = 0;
  virtual auto write(uint32 address, uint8 data) -> void = 0;

  template<typename Size> auto read(Immediate immediate) -> Size { return immediate.value; }

  //tlcs900h.cpp
  auto power() -> void;

  //registers.cpp
  template<typename Size> auto map(Register register) -> maybe<Size&>;
  template<typename Size> auto read(Register) -> Size;
  template<typename Size> auto write(Register, Size data) -> void;
  template<typename Size> auto read(StatusRegister) -> Size;
  template<typename Size> auto write(StatusRegister, Size) -> void;
  template<typename Size> auto read(FlagRegister) -> Size;
  template<typename Size> auto write(FlagRegister, Size) -> void;

  //memory.cpp
  template<typename Size = Byte> auto fetch() -> Size;
  template<typename Size> auto push(Size) -> void;
  template<typename Size> auto pop() -> Size;
  template<typename Size> auto read(Memory memory) -> Size;
  template<typename Size> auto write(Memory memory, Size data) -> void;

  //conditions.cpp
  auto condition(uint4 code) -> bool;

  //algorithms.cpp
  template<typename Size> auto parity(Size) const -> bool;
  template<typename Size> auto algorithmAdd(Size target, Size source, uint1 carry = 0) -> Size;
  template<typename Size> auto algorithmAnd(Size target, Size source) -> Size;
  template<typename Size> auto algorithmOr(Size target, Size source) -> Size;
  template<typename Size> auto algorithmSubtract(Size target, Size source, uint1 carry = 0) -> Size;
  template<typename Size> auto algorithmXor(Size target, Size source) -> Size;

  //instruction.cpp
  template<typename Size> auto registerLookup(uint3 code) -> Register;

  auto instruction() -> void;
  template<typename Size> auto instructionRegister(Register) -> void;
  template<typename Size> auto instructionSourceMemory(Memory) -> void;
  auto instructionTargetMemory(Memory) -> void;

  //instructions.cpp
  template<typename Size, typename Target, typename Source> auto instructionAdd(Target target, Source source) -> void;
  template<typename Size, typename Target, typename Source> auto instructionAddCarry(Target target, Source source) -> void;
  template<typename Size, typename Target, typename Source> auto instructionAnd(Target target, Source source) -> void;
  template<typename Size, typename Target, typename Source> auto instructionCompare(Target target, Source source) -> void;
  auto instructionComplementCarry() -> void;
  template<typename Size, typename Source> auto instructionJump(Source) -> void;
  template<typename Size, typename Source> auto instructionJump(uint4 code, Source) -> void;
  template<typename Size> auto instructionJumpRelative(uint4 code, Size displacement) -> void;
  template<typename Size, typename Target, typename Source> auto instructionLoad(Target target, Source source) -> void;
  auto instructionNoOperation() -> void;
  template<typename Size, typename Target, typename Source> auto instructionOr(Target target, Source source) -> void;
  template<typename Size, typename Target> auto instructionPop(Target target) -> void;
  template<typename Size, typename Source> auto instructionPush(Source source) -> void;
  auto instructionSoftwareInterrupt(uint3 interrupt) -> void;
  template<typename Size, typename Target, typename Source> auto instructionSubtract(Target target, Source source) -> void;
  template<typename Size, typename Target, typename Source> auto instructionSubtractCarry(Target target, Source source) -> void;
  template<typename Size, typename Target, typename Source> auto instructionXor(Target target, Source source) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

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

  static inline const Register A{0xe0};
  static inline const Register W{0xe1};
  static inline const Register C{0xe4};
  static inline const Register B{0xe5};
  static inline const Register E{0xe8};
  static inline const Register D{0xe9};
  static inline const Register L{0xec};
  static inline const Register H{0xed};

  static inline const Register WA{0xe0};
  static inline const Register BC{0xe4};
  static inline const Register DE{0xe8};
  static inline const Register HL{0xec};
  static inline const Register IX{0xf0};
  static inline const Register IY{0xf4};
  static inline const Register IZ{0xf8};
  static inline const Register SP{0xfc};

  static inline const Register XWA{0xe0};
  static inline const Register XBC{0xe4};
  static inline const Register XDE{0xe8};
  static inline const Register XHL{0xec};
  static inline const Register XIX{0xf0};
  static inline const Register XIY{0xf4};
  static inline const Register XIZ{0xf8};
  static inline const Register XSP{0xfc};

  static inline const uint1 Undefined = 0;
};

}
