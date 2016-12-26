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
}

auto Z80::irq(bool maskable, uint16 pc, uint8 extbus) -> bool {
  if(maskable && !r.iff1) return false;

  push(r.pc);

  switch(r.im) {

  case 0: {
    //external data bus ($ff = RST $38)
    r.pc = extbus;
    break;
  }

  case 1: {
    //constant address
    r.pc = pc;
    break;
  }

  case 2: {
    //vector table with external data bus
    uint16 addr = (r.ir.byte.hi << 8) | extbus;
    r.pc  = read(addr + 0) << 0;
    r.pc |= read(addr + 1) << 8;
    break;
  }

  }

  r.iff1 = 0;
  if(maskable) r.iff2 = 0;
  return true;
}

auto Z80::parity(uint8 value) const -> bool {
  value ^= value >> 4;
  value ^= value >> 2;
  value ^= value >> 1;
  return !(value & 1);
}

}
