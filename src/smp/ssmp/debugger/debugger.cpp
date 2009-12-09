#ifdef SSMP_CPP

void sSMPDebug::op_step() {
  bool break_event = false;

  if(debugger.step_smp) {
    debugger.break_event = Debugger::SMPStep;
    scheduler.exit();
  } else {
    debugger.breakpoint_test(Debugger::Breakpoint::APURAM, Debugger::Breakpoint::Exec, regs.pc, 0x00);
  }

  usage[regs.pc] |= UsageExec;
  opcode_pc = regs.pc;

  if(step_event) step_event();
  sSMP::op_step();
  scheduler.sync_smpcpu();
}

uint8 sSMPDebug::op_read(uint16 addr) {
  uint8 data = sSMP::op_read(addr);
  usage[addr] |= UsageRead;
  debugger.breakpoint_test(Debugger::Breakpoint::APURAM, Debugger::Breakpoint::Read, addr, data);
  return data;
}

void sSMPDebug::op_write(uint16 addr, uint8 data) {
  sSMP::op_write(addr, data);
  usage[addr] |= UsageWrite;
  usage[addr] &= ~UsageExec;
  debugger.breakpoint_test(Debugger::Breakpoint::APURAM, Debugger::Breakpoint::Write, addr, data);
}

sSMPDebug::sSMPDebug() {
  usage = new uint8[1 << 16]();
  opcode_pc = 0xffc0;
}

sSMPDebug::~sSMPDebug() {
  delete[] usage;
}

#endif
