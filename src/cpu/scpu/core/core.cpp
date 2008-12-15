#ifdef SCPU_CPP

#include "opfn.cpp"

void sCPU::enter() {
  initialize:
  //initial latch values for $213c/$213d
  //[x]0035 : [y]0000 (53.0 -> 212) [lda $2137]
  //[x]0038 : [y]0000 (56.5 -> 226) [nop : lda $2137]
  add_clocks(186);

  loop:
  if(event.irq) {
    event.irq = false;
    if(status.nmi_pending == true) {
      status.nmi_pending = false;
      event.irq_vector = (regs.e == false) ? 0xffea : 0xfffa;
    } else if(status.irq_pending == true) {
      status.irq_pending = false;
      event.irq_vector = (regs.e == false) ? 0xffee : 0xfffe;
    }
    op_irq();
  }

  tracer.trace_cpuop(); //traces CPU opcode (only if tracer is enabled)

  status.in_opcode = true;
  switch(op_readpc()) {
    #include "op_read.cpp"
    #include "op_write.cpp"
    #include "op_rmw.cpp"
    #include "op_pc.cpp"
    #include "op_misc.cpp"
  }
  status.in_opcode = false;

  goto loop;
}

void sCPU::op_irq() {
  op_read(regs.pc.d);
  op_io();
  if(!regs.e)op_writestack(regs.pc.b);
  op_writestack(regs.pc.h);
  op_writestack(regs.pc.l);
  op_writestack(regs.e ? (regs.p & ~0x10) : regs.p);
  rd.l = op_read(event.irq_vector + 0);
  regs.pc.b = 0x00;
  regs.p.i  = 1;
  regs.p.d  = 0;
  rd.h = op_read(event.irq_vector + 1);
  regs.pc.w = rd.w;
}

//immediate, 2-cycle opcodes with I/O cycle will become bus read
//when an IRQ is to be triggered immediately after opcode completion
//this affects the following opcodes:
//  clc, cld, cli, clv, sec, sed, sei,
//  tax, tay, txa, txy, tya, tyx,
//  tcd, tcs, tdc, tsc, tsx, tcs,
//  inc, inx, iny, dec, dex, dey,
//  asl, lsr, rol, ror, nop, xce.
alwaysinline void sCPU::op_io_irq() {
  if(event.irq) {
    //IRQ pending, modify I/O cycle to bus read cycle, do not increment PC
    op_read(regs.pc.d);
  } else {
    op_io();
  }
}

alwaysinline void sCPU::op_io_cond2() {
  if(regs.d.l != 0x00) {
    op_io();
  }
}

alwaysinline void sCPU::op_io_cond4(uint16 x, uint16 y) {
  if(!regs.p.x || (x & 0xff00) != (y & 0xff00)) {
    op_io();
  }
}

alwaysinline void sCPU::op_io_cond6(uint16 addr) {
  if(regs.e && (regs.pc.w & 0xff00) != (addr & 0xff00)) {
    op_io();
  }
}

#endif //ifdef SCPU_CPP
