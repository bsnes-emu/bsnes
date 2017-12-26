#include <processor/processor.hpp>
#include "z80.hpp"

namespace Processor {

#include "disassembler.cpp"
#include "registers.cpp"
#include "memory.cpp"
#include "instruction.cpp"
#include "algorithms.cpp"
#include "instructions.cpp"
#include "serialization.cpp"

auto Z80::power() -> void {
  r = {};
  prefix = Prefix::hl;
}

auto Z80::irq(bool maskable, uint16 pc, uint8 extbus) -> bool {
  if(maskable && !IFF1) return false;
  R.bits(0,6)++;

  push(PC);

  switch(maskable ? IM : (uint2)1) {

  case 0: {
    //external data bus ($ff = RST $38)
    PC = extbus;
    break;
  }

  case 1: {
    //constant address
    PC = pc;
    break;
  }

  case 2: {
    //vector table with external data bus
    uint16 addr = I << 8 | extbus;
    PC  = read(addr + 0) << 0;
    PC |= read(addr + 1) << 8;
    break;
  }

  }

  IFF1 = 0;
  if(maskable) IFF2 = 0;
  return true;
}

auto Z80::parity(uint8 value) const -> bool {
  value ^= value >> 4;
  value ^= value >> 2;
  value ^= value >> 1;
  return !(value & 1);
}

}
