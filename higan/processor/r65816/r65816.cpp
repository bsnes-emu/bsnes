#include <processor/processor.hpp>
#include "r65816.hpp"

namespace Processor {

#include "algorithms.cpp"
#include "disassembler.cpp"
#include "serialization.cpp"

#define L lastCycle();
#define call(op) (this->*op)()

#include "opcode_read.cpp"
#include "opcode_write.cpp"
#include "opcode_rmw.cpp"
#include "opcode_pc.cpp"
#include "opcode_misc.cpp"
#include "switch.cpp"

#undef L
#undef call

//immediate, 2-cycle opcodes with I/O cycle will become bus read
//when an IRQ is to be triggered immediately after opcode completion.
//this affects the following opcodes:
//  clc, cld, cli, clv, sec, sed, sei,
//  tax, tay, txa, txy, tya, tyx,
//  tcd, tcs, tdc, tsc, tsx, txs,
//  inc, inx, iny, dec, dex, dey,
//  asl, lsr, rol, ror, nop, xce.
auto R65816::ioIRQ() -> void {
  if(interruptPending()) {
    //modify I/O cycle to bus read cycle, do not increment PC
    read(regs.pc.d);
  } else {
    io();
  }
}

auto R65816::io2() -> void {
  if(regs.d.l != 0x00) {
    io();
  }
}

auto R65816::io4(uint16 x, uint16 y) -> void {
  if(!regs.p.x || (x & 0xff00) != (y & 0xff00)) {
    io();
  }
}

auto R65816::io6(uint16 addr) -> void {
  if(regs.e && (regs.pc.w & 0xff00) != (addr & 0xff00)) {
    io();
  }
}

auto R65816::interrupt() -> void {
  read(regs.pc.d);
  io();
  if(!regs.e) writestack(regs.pc.b);
  writestack(regs.pc.h);
  writestack(regs.pc.l);
  writestack(regs.e ? (regs.p & ~0x10) : regs.p);
  rd.l = read(regs.vector + 0);
  regs.pc.b = 0x00;
  regs.p.i  = 1;
  regs.p.d  = 0;
  rd.h = read(regs.vector + 1);
  regs.pc.w = rd.w;
}

}
