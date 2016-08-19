#include <processor/processor.hpp>
#include "z80.hpp"

namespace Processor {

#include "instruction.cpp"
#include "instructions.cpp"

auto Z80::power() -> void {
}

auto Z80::reset() -> void {
  r.af = 0x0000;
  r.bc = 0x0000;
  r.de = 0x0000;
  r.hl = 0x0000;
  r.ix = 0x0000;
  r.iy = 0x0000;
  r.sp = 0x0000;
  r.pc = 0x0000;
  r.i  = 0x00;
  r.r  = 0x00;
}

}
