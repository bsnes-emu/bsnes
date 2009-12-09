#ifdef SCPU_CPP

void sCPUDebug::op_step() {
  bool break_event = false;

  if(debugger.step_cpu) {
    debugger.break_event = Debugger::CPUStep;
    scheduler.exit();
  } else {
    debugger.breakpoint_test(Debugger::Breakpoint::CPUBus, Debugger::Breakpoint::Exec, regs.pc, 0x00);
  }

  usage[regs.pc] &= ~(UsageFlagM | UsageFlagX);
  usage[regs.pc] |= UsageExec | (regs.p.m << 1) | (regs.p.x << 0);
  opcode_pc = regs.pc;

  if(step_event) step_event();
  sCPU::op_step();
  scheduler.sync_cpusmp();
}

uint8 sCPUDebug::op_read(uint32 addr) {
  uint8 data = sCPU::op_read(addr);
  usage[addr] |= UsageRead;
  debugger.breakpoint_test(Debugger::Breakpoint::CPUBus, Debugger::Breakpoint::Read, addr, data);
  return data;
}

void sCPUDebug::op_write(uint32 addr, uint8 data) {
  sCPU::op_write(addr, data);
  usage[addr] |= UsageWrite;
  usage[addr] &= ~UsageExec;
  debugger.breakpoint_test(Debugger::Breakpoint::CPUBus, Debugger::Breakpoint::Write, addr, data);
}

sCPUDebug::sCPUDebug() {
  usage = new uint8[1 << 24]();
  opcode_pc = 0x8000;
}

sCPUDebug::~sCPUDebug() {
  delete[] usage;
}

#endif
