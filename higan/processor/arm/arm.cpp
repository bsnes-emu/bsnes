#include <processor/processor.hpp>
#include "arm.hpp"

namespace Processor {

#include "registers.cpp"
#include "algorithms.cpp"
#include "instructions-arm.cpp"
#include "instructions-thumb.cpp"
#include "step.cpp"
#include "disassembler.cpp"
#include "serialization.cpp"

auto ARM::power() -> void {
  processor.power();
  vector(0x00000000, Processor::Mode::SVC);
  pipeline.reload = true;
  pipeline.nonsequential = true;
  crash = false;
  r(15).modify = [&] {
    pipeline.reload = true;
  };

  trace = false;
  instructions = 0;
}

auto ARM::exec() -> void {
  cpsr().t ? stepTHUMB() : stepARM();
}

auto ARM::idle() -> void {
  pipeline.nonsequential = true;
  return _idle();
}

auto ARM::read(uint mode, uint32 addr) -> uint32 {
  return _read(mode, addr);
}

auto ARM::load(uint mode, uint32 addr) -> uint32 {
  pipeline.nonsequential = true;
  uint32 word = _read(Load | mode, addr);

  if(mode & Half) {
    addr &= 1;
    word = mode & Signed ? (int16_t)word : (uint16_t)word;
  }

  if(mode & Byte) {
    addr &= 0;
    word = mode & Signed ? (int8_t)word : (uint8_t)word;
  }

  if(mode & Signed) {
    word = asr(word, 8 * (addr & 3));
  } else {
    word = ror(word, 8 * (addr & 3));
  }

  idle();
  return word;
}

auto ARM::write(uint mode, uint32 addr, uint32 word) -> void {
  pipeline.nonsequential = true;
  return _write(mode, addr, word);
}

auto ARM::store(uint mode, uint32 addr, uint32 word) -> void {
  pipeline.nonsequential = true;

  if(mode & Half) { word &= 0xffff; word |= word << 16; }
  if(mode & Byte) { word &= 0xff; word |= word << 8; word |= word << 16; }

  return _write(Store | mode, addr, word);
}

auto ARM::vector(uint32 addr, Processor::Mode mode) -> void {
  auto psr = cpsr();
  processor.setMode(mode);
  spsr() = psr;
  cpsr().i = 1;
  cpsr().f |= mode == Processor::Mode::FIQ;
  cpsr().t = 0;
  r(14) = pipeline.decode.address;
  r(15) = addr;
}

}
