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

  for(uint n : range(8)) r.d(n) = 0;
  for(uint n : range(7)) r.a(n) = 0;
  r.ssp = 0;
  r.usp = 0;
  r.pc = 0;
  r.sr = 0x2000;
}

}
