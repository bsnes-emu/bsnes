void bCPU::irq_run() {
  mem_read(regs.pc.d);
  cpu_io();
  if(!regs.e)op_writestack(regs.pc.b);
  op_writestack(regs.pc.h);
  op_writestack(regs.pc.l);
  op_writestack(regs.e ? (regs.p & ~0x10) : regs.p);
  rd.l = op_readaddr(aa.w + 0);
  regs.pc.b = 0x00;
  regs.p.i  = 1;
  regs.p.d  = 0;
  rd.h = op_readaddr(aa.w + 1);
  regs.pc.w = rd.w;
#ifdef DEBUGGER
//let debugger know the new IRQ opcode address
  snes->notify(SNES::CPU_EXEC_OPCODE_END);
#endif
}

bool bCPU::nmi_test() {
  if(time.nmi_transition == 0)return false;
  time.nmi_transition = 0;

  run_state.wai = false;
  return true;
}

bool bCPU::irq_test() {
  if(time.irq_transition == 1)goto _true;

  if(time.irq_read == 0) {
    if(time.irq_line == 1 && (irq_trigger_pos_match(0) || irq_trigger_pos_match(2))) {
      return false;
    }
    goto _true;
  }

  if(time.irq_line == 0) {
    time.irq_line = 1;
    goto _true;
  }

  return false;

_true:
  time.irq_transition = 0;

  run_state.wai = false;
  return (regs.p.i) ? false : true;
}
