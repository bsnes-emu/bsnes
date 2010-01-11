#ifdef SSMP_CPP

void sSMPDebugger::op_step() {
  bool break_event = false;

  usage[regs.pc] |= UsageExec;
  opcode_pc = regs.pc;

  if(debugger.step_smp) {
    debugger.break_event = Debugger::SMPStep;
    scheduler.exit(Scheduler::DebuggerEvent);
  } else {
    debugger.breakpoint_test(Debugger::Breakpoint::APURAM, Debugger::Breakpoint::Exec, regs.pc, 0x00);
  }

  if(step_event) step_event();
  sSMP::op_step();
  scheduler.sync_smpcpu();
}

uint8 sSMPDebugger::op_read(uint16 addr) {
  uint8 data = sSMP::op_read(addr);
  usage[addr] |= UsageRead;
  debugger.breakpoint_test(Debugger::Breakpoint::APURAM, Debugger::Breakpoint::Read, addr, data);
  return data;
}

void sSMPDebugger::op_write(uint16 addr, uint8 data) {
  sSMP::op_write(addr, data);
  usage[addr] |= UsageWrite;
  usage[addr] &= ~UsageExec;
  debugger.breakpoint_test(Debugger::Breakpoint::APURAM, Debugger::Breakpoint::Write, addr, data);
}

sSMPDebugger::sSMPDebugger() {
  usage = new uint8[1 << 16]();
  opcode_pc = 0xffc0;
}

sSMPDebugger::~sSMPDebugger() {
  delete[] usage;
}

//===========
//SMPDebugger
//===========

//$00f0
unsigned sSMPDebugger::clock_speed() { return status.clock_speed; }
bool sSMPDebugger::mmio_disable() { return status.mmio_disabled; }
bool sSMPDebugger::ram_writable() { return status.ram_writable; }

//$00f1
bool sSMPDebugger::iplrom_enable() { return status.iplrom_enabled; }

//$00f2
unsigned sSMPDebugger::dsp_address() { return status.dsp_addr; }

#endif
