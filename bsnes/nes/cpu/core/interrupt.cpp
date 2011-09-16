void CPU::interrupt() {
  op_readpc();
  op_readpc();
  op_writesp(regs.pc >> 8);
  op_writesp(regs.pc >> 0);
  op_writesp(regs.p | 0x20);
  uint16 vector = 0xfffe;  //IRQ
  if(status.nmi_pending) {
    status.nmi_pending = false;
    vector = 0xfffa;
  }
  abs.l = op_read(vector++);
  regs.p.i = 1;
  regs.p.d = 0;
L abs.h = op_read(vector++);
  regs.pc = abs.w;
}

void CPU::interrupt_test() {
  status.interrupt_pending = ((status.irq_line | status.irq_apu_line) & ~regs.p.i) | status.nmi_pending;
}

void CPU::set_nmi_line(bool line) {
  //edge-sensitive (0->1)
  if(!status.nmi_line && line) status.nmi_pending = true;
  status.nmi_line = line;
}

void CPU::set_irq_line(bool line) {
  //level-sensitive
  status.irq_line = line;
}

void CPU::set_irq_apu_line(bool line) {
  //level-sensitive
  status.irq_apu_line = line;
}
