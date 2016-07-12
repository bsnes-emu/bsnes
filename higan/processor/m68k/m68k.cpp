#include <processor/processor.hpp>
#include "m68k.hpp"

namespace Processor {

#include "memory.cpp"
#include "ea.cpp"
#include "instruction.cpp"
#include "instructions.cpp"
#include "disassembler.cpp"

auto M68K::power() -> void {
}

auto M68K::reset() -> void {
  instructionsExecuted = 0;

  r.d0 = r.d1 = r.d2 = r.d3 = r.d4 = r.d5 = r.d6 = r.d7 = 0;
  r.a0 = r.a1 = r.a2 = r.a3 = r.a4 = r.a5 = r.a6 = r.usp = r.ssp = 0;
  r.pc = 0;
  r.ccr = 0;
}

}
