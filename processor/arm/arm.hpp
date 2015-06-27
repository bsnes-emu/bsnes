#ifndef PROCESSOR_ARM_HPP
#define PROCESSOR_ARM_HPP

namespace Processor {

//Supported Models:
//* ARMv3 (ST018)
//* ARMv4 (ARM7TDMI)

struct ARM {
  enum : unsigned { Byte = 8, Half = 16, Word = 32 };
  enum : bool { Nonsequential = 0, Sequential = 1 };

  #include "registers.hpp"
  #include "instructions-arm.hpp"
  #include "instructions-thumb.hpp"
  #include "disassembler.hpp"

  virtual auto step(unsigned clocks) -> void = 0;
  virtual auto bus_idle(uint32 addr) -> void = 0;
  virtual auto bus_read(uint32 addr, uint32 size, bool mode) -> uint32 = 0;
  virtual auto bus_load(uint32 addr, uint32 size, bool mode) -> uint32 = 0;
  virtual auto bus_write(uint32 addr, uint32 size, bool mode, uint32 word) -> void = 0;
  virtual auto bus_store(uint32 addr, uint32 size, bool mode, uint32 word) -> void = 0;

  //arm.cpp
  auto power() -> void;
  auto exec() -> void;
  auto idle() -> void;
  auto read(uint32 addr, uint32 size, bool mode) -> uint32;
  auto load(uint32 addr, uint32 size, bool mode) -> uint32;
  auto write(uint32 addr, uint32 size, bool mode, uint32 word) -> void;
  auto store(uint32 addr, uint32 size, bool mode, uint32 word) -> void;
  auto vector(uint32 addr, Processor::Mode mode) -> void;

  //algorithms.cpp
  auto condition(uint4 condition) -> bool;
  auto bit(uint32 result) -> uint32;
  auto add(uint32 source, uint32 modify, bool carry) -> uint32;
  auto sub(uint32 source, uint32 modify, bool carry) -> uint32;
  auto mul(uint32 product, uint32 multiplicand, uint32 multiplier) -> uint32;
  auto lsl(uint32 source, uint8 shift) -> uint32;
  auto lsr(uint32 source, uint8 shift) -> uint32;
  auto asr(uint32 source, uint8 shift) -> uint32;
  auto ror(uint32 source, uint8 shift) -> uint32;
  auto rrx(uint32 source) -> uint32;

  //step.cpp
  auto arm_step() -> void;
  auto thumb_step() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  bool trace{false};
  uintmax_t instructions{0};
};

}

#endif
