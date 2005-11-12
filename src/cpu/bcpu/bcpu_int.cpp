/*
[IRQ cycles]
  [0] pbr,pc ; opcode
  [1] pbr,pc ; io
  [2] 0,s    ; pbr
  [3] 0,s-1  ; pch
  [4] 0,s-2  ; pcl
  [5] 0,s-3  ; p
  [6] 0,va   ; aavl
  [7] 0,va+1 ; aavh
*/
void bCPU::irq_run() {
//WDC documentation is incorrect, first cycle
//is a memory read fetch from PBR:PC
  switch(status.cycle_pos++) {
  case 0: add_cycles(r_mem->speed(regs.pc.d));   break;
  case 1: add_cycles(6);                         break;
  case 2: stack_write(regs.pc.b);                break;
  case 3: stack_write(regs.pc.h);                break;
  case 4: stack_write(regs.pc.l);                break;
  case 5: stack_write(regs.p);                   break;
  case 6: rd.l = op_read(OPMODE_ADDR, aa.w);     break;
  case 7: rd.h = op_read(OPMODE_ADDR, aa.w + 1);
    regs.pc.b = 0x00;
    regs.pc.w = rd.w;
    regs.p.i  = 1;
    regs.p.d  = 0;
  #ifdef DEBUGGER
  //let debugger know the new IRQ opcode address
    snes->notify(SNES::CPU_EXEC_OPCODE_END);
  #endif
    status.cycle_pos = 0;
    run_state.irq = false;
    break;
  }
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
  if(regs.p.i)return false;
  return true;
}
