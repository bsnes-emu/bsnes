#include <processor/processor.hpp>
#include "huc6280.hpp"

namespace Processor {

#define L lastCycle();
#include "memory.cpp"
#include "instruction.cpp"
#include "instructions.cpp"
#include "disassembler.cpp"
#undef L

auto HuC6280::power() -> void {
  r.a = 0x00;
  r.x = 0x00;
  r.y = 0x00;
  r.s = 0x00;
  r.pc = 0x0000;
  r.p = 0x00;
  r.mdr = 0x00;
}

}
