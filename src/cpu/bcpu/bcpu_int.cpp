/*
[IRQ cycles]
  [1] pbr,pc ; opcode
  [2] pbr,pc ; io
  [3] 0,s    ; pbr
  [4] 0,s-1  ; pch
  [5] 0,s-2  ; pcl
  [6] 0,s-3  ; p
  [7] 0,va   ; aavl
  [8] 0,va+1 ; aavh
*/
void bCPU::irq(uint16 addr) {
//WDC documentation is incorrect, first cycle
//is a memory read fetch from PBR:PC
  add_cycles(mem_bus->speed(regs.pc.d));
  add_cycles(6);
  stack_write(regs.pc.b);
  stack_write(regs.pc.h);
  stack_write(regs.pc.l);
  stack_write(regs.p);
  rd.l = op_read(OPMODE_ADDR, addr);
  rd.h = op_read(OPMODE_ADDR, addr + 1);

  regs.pc.b = 0x00;
  regs.pc.w = rd.w;
  regs.p.i  = 1;
  regs.p.d  = 0;

//let debugger know the new IRQ opcode address
  snes->notify(SNES::CPU_EXEC_OPCODE_END);
}

bool bCPU::nmi_test() {
  if(time.nmi_transition == 0)return false;
  time.nmi_transition = 0;

  if(status.cpu_state == CPUSTATE_WAI) {
    status.cpu_state = CPUSTATE_RUN;
  }

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

  if(status.cpu_state == CPUSTATE_WAI) {
    status.cpu_state = CPUSTATE_RUN;
  }

  if(regs.p.i)return false;
  return true;
}
