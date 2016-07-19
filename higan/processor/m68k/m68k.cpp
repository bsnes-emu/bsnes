#include <processor/processor.hpp>
#include "m68k.hpp"

namespace Processor {

enum : uint { Byte, Word, Long };

#include "registers.cpp"
#include "memory.cpp"
#include "ea.cpp"
#include "instruction.cpp"
#include "instructions.cpp"
#include "disassembler.cpp"

auto M68K::power() -> void {
}

auto M68K::reset() -> void {
  instructionsExecuted = 0;

  for(uint rn : range(15)) write<Long>(Register{rn}, 0);
  r.ssp = 0;
  r.usp = 0;
  r.pc = 0;
  r.sr = 0x2000;
}

}
