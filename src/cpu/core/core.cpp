#include <../base.hpp>

#include "opcode_algorithms.cpp"
#include "opcode_functions.cpp"
#include "opcode_tables.cpp"
#include "disasm/disasm.cpp"

//immediate, 2-cycle opcodes with I/O cycle will become bus read
//when an IRQ is to be triggered immediately after opcode completion.
//this affects the following opcodes:
//  clc, cld, cli, clv, sec, sed, sei,
//  tax, tay, txa, txy, tya, tyx,
//  tcd, tcs, tdc, tsc, tsx, txs,
//  inc, inx, iny, dec, dex, dey,
//  asl, lsr, rol, ror, nop, xce.
alwaysinline void CPUcore::op_io_irq() {
  if(interrupt_pending()) {
    //modify I/O cycle to bus read cycle, do not increment PC
    op_read(regs.pc.d);
  } else {
    op_io();
  }
}

alwaysinline void CPUcore::op_io_cond2() {
  if(regs.d.l != 0x00) {
    op_io();
  }
}

alwaysinline void CPUcore::op_io_cond4(uint16 x, uint16 y) {
  if(!regs.p.x || (x & 0xff00) != (y & 0xff00)) {
    op_io();
  }
}

alwaysinline void CPUcore::op_io_cond6(uint16 addr) {
  if(regs.e && (regs.pc.w & 0xff00) != (addr & 0xff00)) {
    op_io();
  }
}

CPUcore::CPUcore() {
  initialize_opcode_table();
}
