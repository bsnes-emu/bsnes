#ifdef SCPU_CPP

void sCPUdebug::op_step() {
  bool break_event = false;

  if(debugger.step_cpu) {
    debugger.break_event = Debugger::CPUStep;
    break_event = true;
  }

  for(unsigned i = 0; i < Debugger::Breakpoints; i++) {
    if(debugger.breakpoint[i].enabled == false) continue;
    if(debugger.breakpoint[i].addr != regs.pc) continue;
    if(debugger.breakpoint[i].mode != Debugger::Breakpoint::Exec) continue;
    if(debugger.breakpoint[i].source != Debugger::Breakpoint::CPUBus) continue;

    debugger.breakpoint[i].counter++;
    debugger.breakpoint_hit = i;
    debugger.break_event = Debugger::BreakpointHit;
    break_event = true;
    break;
  }

  if(break_event) scheduler.exit();

  if(debugger.trace_cpu) {
    char t[256];
    disassemble_opcode(t);
    debugger.tracefile.print(string() << t << "\n");
  }

  sCPU::op_step();
  scheduler.sync_cpusmp();
}

uint8 sCPUdebug::op_read(uint32 addr) {
  uint8 data = sCPU::op_read(addr);

  for(unsigned i = 0; i < Debugger::Breakpoints; i++) {
    if(debugger.breakpoint[i].enabled == false) continue;
    if(debugger.breakpoint[i].addr != addr) continue;
    if(debugger.breakpoint[i].data != -1 && debugger.breakpoint[i].data != data) continue;
    if(debugger.breakpoint[i].mode != Debugger::Breakpoint::Read) continue;
    if(debugger.breakpoint[i].source != Debugger::Breakpoint::CPUBus) continue;

    debugger.breakpoint[i].counter++;
    debugger.breakpoint_hit = i;
    debugger.break_event = Debugger::BreakpointHit;
    scheduler.exit();
    break;
  }
}

void sCPUdebug::op_write(uint32 addr, uint8 data) {
  sCPU::op_write(addr, data);

  for(unsigned i = 0; i < Debugger::Breakpoints; i++) {
    if(debugger.breakpoint[i].enabled == false) continue;
    if(debugger.breakpoint[i].addr != addr) continue;
    if(debugger.breakpoint[i].data != -1 && debugger.breakpoint[i].data != data) continue;
    if(debugger.breakpoint[i].mode != Debugger::Breakpoint::Write) continue;
    if(debugger.breakpoint[i].source != Debugger::Breakpoint::CPUBus) continue;

    debugger.breakpoint[i].counter++;
    debugger.breakpoint_hit = i;
    debugger.break_event = Debugger::BreakpointHit;
    scheduler.exit();
    break;
  }
}

#endif
