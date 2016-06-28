#include <processor/processor.hpp>
#include "r65816.hpp"

namespace Processor {

#include "algorithms.cpp"
#include "disassembler.cpp"
#include "serialization.cpp"

#define E if(r.e)
#define N if(!r.e)
#define L lastCycle();
#define call(op) (this->*op)()

#include "instructions-read.cpp"
#include "instructions-write.cpp"
#include "instructions-rmw.cpp"
#include "instructions-pc.cpp"
#include "instructions-misc.cpp"
#include "switch.cpp"

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
    read(r.pc.d);
  } else {
    io();
  }
}

auto R65816::io2() -> void {
  if(r.d.l != 0x00) {
    io();
  }
}

auto R65816::io4(uint16 x, uint16 y) -> void {
  if(!r.p.x || (x & 0xff00) != (y & 0xff00)) {
    io();
  }
}

auto R65816::io6(uint16 addr) -> void {
  if(r.e && (r.pc.w & 0xff00) != (addr & 0xff00)) {
    io();
  }
}

auto R65816::interrupt() -> void {
  read(r.pc.d);
  io();
N writeSP(r.pc.b);
  writeSP(r.pc.h);
  writeSP(r.pc.l);
  writeSP(r.e ? (r.p & ~0x10) : r.p);
  r.pc.l = read(r.vector + 0);
  r.p.i = 1;
  r.p.d = 0;
  r.pc.h = read(r.vector + 1);
  r.pc.b = 0x00;
}

#undef E
#undef N
#undef L
#undef call

}
