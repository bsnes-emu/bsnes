#ifdef SSMP_CPP

void sSMPdebug::op_step() {
  bool break_event = false;

  if(debugger.step_smp) {
    debugger.break_event = Debugger::SMPStep;
    break_event = true;
  }

  for(unsigned i = 0; i < Debugger::Breakpoints; i++) {
    if(debugger.breakpoint[i].enabled == false) continue;
    if(debugger.breakpoint[i].addr != regs.pc) continue;
    if(debugger.breakpoint[i].mode != Debugger::Breakpoint::Exec) continue;
    if(debugger.breakpoint[i].source != Debugger::Breakpoint::APURAM) continue;

    debugger.breakpoint[i].counter++;
    debugger.breakpoint_hit = i;
    debugger.break_event = Debugger::BreakpointHit;
    break_event = true;
    break;
  }

  if(break_event) scheduler.exit();

  if(debugger.trace_smp) {
    char t[256];
    disassemble_opcode(t);
    debugger.tracefile.print(string() << t << "\n");
  }

  sSMP::op_step();
  scheduler.sync_smpcpu();
}

#endif
