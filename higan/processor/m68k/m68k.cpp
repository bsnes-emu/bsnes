#include <processor/processor.hpp>
#include "m68k.hpp"

namespace Processor {

enum : uint { Byte, Word, Long };
enum : bool { Reverse = 1 };

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

  for(auto& da : r.da) da = 0;
  r.sp = 0;
  r.pc = 0;
  r.sr = 0x2000;
}

auto M68K::supervisor() -> bool {
  if(r.s) return true;
  //todo: raise TRAP exception
  return false;
}

}
