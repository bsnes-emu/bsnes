#include "opfn.cpp"
//#include "op_read.cpp"
//#include "op_write.cpp"
//#include "op_rmw.cpp"
//#include "op_pc.cpp"
//#include "op_misc.cpp"

void sCPU::main() {
  for(;;) {
    status.in_opcode = true;

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

//  (this->*optbl[op_readpc()])();
    switch(op_readpc()) {
      #include "op_read.cpp"
      #include "op_write.cpp"
      #include "op_rmw.cpp"
      #include "op_pc.cpp"
      #include "op_misc.cpp"
    }

    status.in_opcode = false;
    opcode_edge();
  }
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

#ifdef DEBUGGER
  status.in_opcode = false;
  snes->notify(SNES::CPU_EXEC_OPCODE_END);
  status.in_opcode = true;
#endif
}

//

void sCPU::op_io_cond2() {
  if(regs.d.l != 0x00)op_io();
}

void sCPU::op_io_cond4(uint16 x, uint16 y) {
  if(!regs.p.x && (x & 0xff00) != (y & 0xff00))op_io();
}

void sCPU::op_io_cond6(uint16 addr) {
  if(regs.e && (regs.pc.w & 0xff00) != (addr & 0xff00))op_io();
}
