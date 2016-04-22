#include <processor/processor.hpp>
#include "hg51b.hpp"

namespace Processor {

#include "registers.cpp"
#include "instructions.cpp"
#include "serialization.cpp"

auto HG51B::exec(uint24 addr) -> void {
  if(regs.halt) return;
  addr = addr + regs.pc * 2;
  opcode  = read(addr++) << 0;
  opcode |= read(addr++) << 8;
  regs.pc = (regs.pc & 0xffff00) | ((regs.pc + 1) & 0x0000ff);
  instruction();
}

auto HG51B::power() -> void {
  regs.halt = true;

  regs.n = 0;
  regs.z = 0;
  regs.c = 0;
}

}
