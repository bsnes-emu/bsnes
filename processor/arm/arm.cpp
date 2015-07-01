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
  cpsr().t ? thumb_step() : arm_step();
}

auto ARM::idle() -> void {
  pipeline.nonsequential = true;
  return bus_idle();
}

auto ARM::read(unsigned mode, uint32 addr) -> uint32 {
  return bus_read(mode, addr);
}

auto ARM::load(unsigned mode, uint32 addr) -> uint32 {
  pipeline.nonsequential = true;
  uint32 word = bus_read(Load | mode, addr);

  if(mode & Half) { word &= 0xffff; word |= word << 16; }
  if(mode & Byte) { word &= 0xff; word |= word << 8; word |= word << 16; }

  word = ror(word, 8 * (addr & 3));
  idle();

  if(mode & Half) word &= 0xffff;
  if(mode & Byte) word &= 0xff;
  return word;
}

auto ARM::write(unsigned mode, uint32 addr, uint32 word) -> void {
  pipeline.nonsequential = true;
  return bus_write(mode, addr, word);
}

auto ARM::store(unsigned mode, uint32 addr, uint32 word) -> void {
  pipeline.nonsequential = true;

  if(mode & Half) { word &= 0xffff; word |= word << 16; }
  if(mode & Byte) { word &= 0xff; word |= word << 8; word |= word << 16; }

  return bus_write(Store | mode, addr, word);
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
