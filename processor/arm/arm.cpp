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
  crash = false;
  r(15).modify = [&] {
    pipeline.reload = true;
  };

  trace = false;
  instructions = 0;
}

auto ARM::exec() -> void {
  cpsr().t ? thumb_step() : arm_step();
}

auto ARM::idle() -> void {
  bus_idle(r(15));
}

auto ARM::read(uint32 addr, uint32 size) -> uint32 {
  uint32 word = bus_read(addr, size);
  sequential() = true;
  return word;
}

auto ARM::load(uint32 addr, uint32 size) -> uint32 {
  sequential() = false;
  uint32 word = read(addr, size);

  if(size == Half) { word &= 0xffff; word |= word << 16; }
  if(size == Byte) { word &= 0xff; word |= word << 8; word |= word << 16; }

  word = ror(word, 8 * (addr & 3));
  idle();

  if(size == Half) word &= 0xffff;
  if(size == Byte) word &= 0xff;
  return word;
}

auto ARM::write(uint32 addr, uint32 size, uint32 word) -> void {
  bus_write(addr, size, word);
  sequential() = true;
}

auto ARM::store(uint32 addr, uint32 size, uint32 word) -> void {
  if(size == Half) { word &= 0xffff; word |= word << 16; }
  if(size == Byte) { word &= 0xff; word |= word << 8; word |= word << 16; }

  sequential() = false;
  write(addr, size, word);
  sequential() = false;
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
