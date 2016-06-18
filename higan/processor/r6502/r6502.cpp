#include <processor/processor.hpp>
#include "r6502.hpp"

namespace Processor {

#define L lastCycle();
#define call(op) (this->*op)()

#include "memory.cpp"
#include "algorithms.cpp"
#include "instructions.cpp"
#include "switch.cpp"
#include "disassembler.cpp"
#include "serialization.cpp"

auto R6502::mdr() const -> uint8 {
  return regs.mdr;
}

auto R6502::power() -> void {
  regs.a = 0x00;
  regs.x = 0x00;
  regs.y = 0x00;
  regs.s = 0x00;
  regs.p = 0x04;
}

auto R6502::reset() -> void {
  regs.mdr = 0x00;
  regs.s -= 3;
  regs.p.i = 1;
}

auto R6502::interrupt() -> void {
  io();
  io();
  writeSP(regs.pc >> 8);
  writeSP(regs.pc >> 0);
  writeSP(regs.p | 0x20);
  uint16 vector = 0xfffe;  //IRQ
  nmi(vector);
  abs.l = read(vector++);
  regs.p.i = 1;
  regs.p.d = 0;
L abs.h = read(vector++);
  regs.pc = abs.w;
}

#undef L
#undef call

}
