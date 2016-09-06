#include <processor/processor.hpp>
#include "z80.hpp"

namespace Processor {

#include "disassembler.cpp"
#include "registers.cpp"
#include "memory.cpp"
#include "instruction.cpp"
#include "instructions.cpp"

auto Z80::power() -> void {
}

auto Z80::reset() -> void {
  memory::fill(&r, sizeof(Registers));
  r.hlp = &r.hl;
  instructionsExecuted = 0;
}

auto Z80::parity(uint8 value) const -> bool {
  value ^= value >> 4;
  value ^= value >> 2;
  value ^= value >> 1;
  return !(value & 1);
}

}
