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
  interrupt(Processor::Mode::SVC, 0x00);
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
  return sleep();
}

auto ARM::read(uint mode, uint32 addr) -> uint32 {
  return get(mode, addr);
}

auto ARM::load(uint mode, uint32 addr) -> uint32 {
  pipeline.nonsequential = true;
  uint32 word = get(Load | mode, addr);

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
  return set(mode, addr, word);
}

auto ARM::store(uint mode, uint32 addr, uint32 word) -> void {
  pipeline.nonsequential = true;

  if(mode & Half) { word &= 0xffff; word |= word << 16; }
  if(mode & Byte) { word &= 0xff; word |= word << 8; word |= word << 16; }

  return set(Store | mode, addr, word);
}

auto ARM::interrupt(Processor::Mode mode, uint32 addr) -> void {
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
