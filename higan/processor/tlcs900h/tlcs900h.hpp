//Toshiba TLCS900/H

/* open questions:
 *
 * what happens when a prohibited instruction operand size is used? (eg adc.l (memory),#immediate)
 * what happens when %11 is used for pre-decrement and post-increment addressing?
 * what happens when using 8-bit register indexing and d0 is set (Word) or d1/d0 is set (Long)?
 * what value is read back from a non-existent 8-bit register ID? (eg 0x40-0xcf)
 * many instructions are undefined, some are marked as dummy instructions ... what do each do?
 */

#pragma once

namespace Processor {

struct TLCS900H {
  virtual auto read(uint32 address) -> uint8 { return 0; }
  virtual auto write(uint32 address, uint8 data) -> void {};

  TLCS900H();

  struct FlagRegister   { using type =  uint8; enum : uint { bits =  8 }; uint1 id; };
  struct StatusRegister { using type = uint16; enum : uint { bits = 16 };           };
  struct ProgramCounter { using type = uint32; enum : uint { bits = 32 };           };

  template<typename T> struct Register  { using type = T; enum : uint { bits = 8 * sizeof(T) }; uint8 id;   };
  template<typename T> struct Memory    { using type = T; enum : uint { bits = 8 * sizeof(T) }; T address;  };
  template<typename T> struct Immediate { using type = T; enum : uint { bits = 8 * sizeof(T) }; T constant; };

  template<typename T> auto load(Immediate<T> immediate) const -> T { return immediate.constant; }

  //tlcs900h.cpp
  auto power() -> void;

  //registers.cpp
  template<typename T> auto map(Register<T>) -> maybe<T&>;
  template<typename T> auto load(Register<T>) -> T;
  template<typename T> auto store(Register<T>, uint32) -> void;
  auto load(FlagRegister) -> uint8;
  auto store(FlagRegister, uint8) -> void;
  auto load(StatusRegister) -> uint16;
  auto store(StatusRegister, uint16) -> void;
  auto load(ProgramCounter) -> uint32;
  auto store(ProgramCounter, uint32) -> void;

  //memory.cpp
  template<typename T = uint8> auto fetch() -> T;
  template<typename T> auto push(T) -> void;
  template<typename T> auto pop(T) -> void;
  template<typename T> auto load(Memory<T>) -> T;
  template<typename T> auto store(Memory<T>, uint32) -> void;

  //conditions.cpp
  auto condition(uint4 code) -> bool;

  //algorithms.cpp
  template<typename T> auto parity(T) const -> bool;
  template<typename T> auto algorithmAdd(T target, T source, uint1 carry = 0) -> T;
  template<typename T> auto algorithmAnd(T target, T source) -> T;
  template<typename T> auto algorithmOr(T target, T source) -> T;
  template<typename T> auto algorithmSubtract(T target, T source, uint1 carry = 0) -> T;
  template<typename T> auto algorithmXor(T target, T source) -> T;

  //instruction.cpp
  template<typename T> auto registers(uint3) const -> Register<T>;
  auto instruction() -> void;
  template<typename Register> auto instructionRegister(Register) -> void;
  template<typename Memory> auto instructionSourceMemory(Memory) -> void;
  auto instructionTargetMemory(uint32 address) -> void;

  //instructions.cpp
  template<typename Target, typename Source> auto instructionAdd(Target target, Source source) -> void;
  template<typename Target, typename Source> auto instructionAddCarry(Target target, Source source) -> void;
  template<typename Target, typename Source> auto instructionAnd(Target target, Source source) -> void;
  template<typename Source> auto instructionCall(uint4 code, Source) -> void;
  template<typename Target, typename Source> auto instructionCompare(Target target, Source source) -> void;
  auto instructionComplementCarry() -> void;
  template<typename Target, typename Source> auto instructionExchange(Target target, Source source) -> void;
  auto instructionHalt() -> void;
  template<typename Source> auto instructionJump(uint4 code, Source) -> void;
  template<typename Source> auto instructionJumpRelative(uint4 code, Source) -> void;
  template<typename Target, typename Source> auto instructionLoad(Target target, Source source) -> void;
  auto instructionNoOperation() -> void;
  template<typename Target, typename Source> auto instructionOr(Target target, Source source) -> void;
  template<typename Target> auto instructionPop(Target target) -> void;
  template<typename Source> auto instructionPush(Source source) -> void;
  auto instructionReturnInterrupt() -> void;
  template<typename Target> auto instructionSetConditionCode(uint4 code, Target) -> void;
  auto instructionSetInterruptFlags(uint3 flags) -> void;
  auto instructionSoftwareInterrupt(uint3 interrupt) -> void;
  template<typename Target, typename Source> auto instructionSubtract(Target target, Source source) -> void;
  template<typename Target, typename Source> auto instructionSubtractCarry(Target target, Source source) -> void;
  template<typename Target, typename Source> auto instructionXor(Target target, Source source) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  union DataRegister {
    DataRegister() {}
    struct { uint32 order_lsb1(l0); } l;
    struct { uint16 order_lsb2(w0, w1); } w;
    struct {  uint8 order_lsb4(b0, b1, b2, b3); } b;
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

    uint1 c, cp;  //carry
    uint1 n, np;  //negative
    uint1 v, vp;  //overflow or parity
    uint1 h, hp;  //half carry
    uint1 z, zp;  //zero
    uint1 s, sp;  //sign
    uint2 rfp, rfpp = 3;  //register file pointer
    uint3 iff = 7;  //interrupt mask flip-flop

    uint1 halted;
  } r;

  auto carry() const -> bool { return r.c; }
  auto negative() const -> bool { return r.n; }
  auto overflow() const -> bool { return r.v; }
  auto parity() const -> bool { return r.v; }
  auto halfCarry() const -> bool { return r.h; }
  auto zero() const -> bool { return r.z; }
  auto sign() const -> bool { return r.s; }

  auto setCarry(bool value) -> void { r.c = value; }
  auto setNegative(bool value) -> void { r.n = value; }
  auto setOverflow(bool value) -> void { r.v = value; }
  auto setParity(bool value) -> void { r.v = value; }
  auto setHalfCarry(bool value) -> void { r.h = value; }
  auto setZero(bool value) -> void { r.z = value; }
  auto setSign(bool value) -> void { r.s = value; }

  auto rfp() const -> uint2 { return r.rfp; }
  auto setRFP(uint2 value) -> void { r.rfp = value; }

  auto iff() const -> uint3 { return r.iff; }
  auto setIFF(uint3 value) -> void { r.iff = value; }

  auto halted() const -> bool { return r.halted; }
  auto setHalted(bool value) -> void { r.halted = value; }

  static inline const Register< uint8> A{0xe0};
  static inline const Register< uint8> W{0xe1};
  static inline const Register< uint8> C{0xe4};
  static inline const Register< uint8> B{0xe5};
  static inline const Register< uint8> E{0xe8};
  static inline const Register< uint8> D{0xe9};
  static inline const Register< uint8> L{0xec};
  static inline const Register< uint8> H{0xed};

  static inline const Register<uint16> WA{0xe0};
  static inline const Register<uint16> BC{0xe4};
  static inline const Register<uint16> DE{0xe8};
  static inline const Register<uint16> HL{0xec};
  static inline const Register<uint16> IX{0xf0};
  static inline const Register<uint16> IY{0xf4};
  static inline const Register<uint16> IZ{0xf8};
  static inline const Register<uint16> SP{0xfc};

  static inline const Register<uint32> XWA{0xe0};
  static inline const Register<uint32> XBC{0xe4};
  static inline const Register<uint32> XDE{0xe8};
  static inline const Register<uint32> XHL{0xec};
  static inline const Register<uint32> XIX{0xf0};
  static inline const Register<uint32> XIY{0xf4};
  static inline const Register<uint32> XIZ{0xf8};
  static inline const Register<uint32> XSP{0xfc};

  static inline const FlagRegister F {0};
  static inline const FlagRegister FP{1};

  static inline const StatusRegister SR{};
  static inline const ProgramCounter PC{};

  static inline const uint4 False{0x00};
  static inline const uint4 True {0x08};

  static inline const uint1 Undefined = 0;
};

}
