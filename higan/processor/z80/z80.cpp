#include <processor/processor.hpp>
#include "z80.hpp"

namespace Processor {

#include "memory.cpp"
#include "instruction.cpp"
#include "instructions.cpp"
#include "disassembler.cpp"

auto Z80::power() -> void {
}

auto Z80::reset() -> void {
  r.af = 0x0000;
  r.bc = 0x0000;
  r.de = 0x0000;
  r.hl = 0x0000;
  r.ir = 0x0000;
  r.ix = 0x0000;
  r.iy = 0x0000;
  r.sp = 0x0000;
  r.pc = 0x0000;

  r.halt = 0;
  r.iff1 = 0;
  r.iff2 = 0;
  r.im = 0;

  instructionsExecuted = 0;
}

auto Z80::parity(uint8 value) const -> bool {
  value ^= value >> 4;
  value ^= value >> 2;
  value ^= value >> 1;
  return !(value & 1);
}

}
