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

//

auto M68K::Registers::d(uint3 reg) -> uint32& {
  switch(reg) {
  case 0: return d0;
  case 1: return d1;
  case 2: return d2;
  case 3: return d3;
  case 4: return d4;
  case 5: return d5;
  case 6: return d6;
  case 7: return d7;
  }
  unreachable;
}

auto M68K::Registers::a(uint3 reg) -> uint32& {
  switch(reg) {
  case 0: return a0;
  case 1: return a1;
  case 2: return a2;
  case 3: return a3;
  case 4: return a4;
  case 5: return a5;
  case 6: return a6;
  case 7: return ssp;
  }
  unreachable;
}

}
