#include <processor/processor.hpp>
#include "r65816.hpp"

namespace Processor {

#include "algorithms.cpp"
#include "disassembler.cpp"
#include "serialization.cpp"

#define L last_cycle();
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
auto R65816::op_io_irq() -> void {
  if(interrupt_pending()) {
    //modify I/O cycle to bus read cycle, do not increment PC
    op_read(regs.pc.d);
  } else {
    op_io();
  }
}

auto R65816::op_io_cond2() -> void {
  if(regs.d.l != 0x00) {
    op_io();
  }
}

auto R65816::op_io_cond4(uint16 x, uint16 y) -> void {
  if(!regs.p.x || (x & 0xff00) != (y & 0xff00)) {
    op_io();
  }
}

auto R65816::op_io_cond6(uint16 addr) -> void {
  if(regs.e && (regs.pc.w & 0xff00) != (addr & 0xff00)) {
    op_io();
  }
}

auto R65816::op_irq() -> void {
  op_read(regs.pc.d);
  op_io();
  if(!regs.e) op_writestack(regs.pc.b);
  op_writestack(regs.pc.h);
  op_writestack(regs.pc.l);
  op_writestack(regs.e ? (regs.p & ~0x10) : regs.p);
  rd.l = op_read(regs.vector + 0);
  regs.pc.b = 0x00;
  regs.p.i  = 1;
  regs.p.d  = 0;
  rd.h = op_read(regs.vector + 1);
  regs.pc.w = rd.w;
}

}
