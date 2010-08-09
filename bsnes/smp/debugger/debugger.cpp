#ifdef SMP_CPP

bool SMPDebugger::property(unsigned id, string &name, string &value) {
  unsigned n = 0;

  //$00f0
  if(id == n++) { name = "$00f0"; value = ""; return true; }
  if(id == n++) { name = "Clock Speed"; value = clock_speed(); return true; }
  if(id == n++) { name = "Timers Enable"; value = timers_enable(); return true; }
  if(id == n++) { name = "RAM Disable"; value = ram_disable(); return true; }
  if(id == n++) { name = "RAM Writable"; value = ram_writable(); return true; }
  if(id == n++) { name = "Timers Disable"; value = timers_disable(); return true; }

  //$00f1
  if(id == n++) { name = "$00f1"; value = ""; return true; }
  if(id == n++) { name = "IPLROM Enable"; value = iplrom_enable(); return true; }

  //$00f2
  if(id == n++) { name = "$00f2"; value = ""; return true; }
  if(id == n++) { name = "DSP Address"; value = string("0x", strhex<2>(dsp_address())); return true; }

  return false;
}

void SMPDebugger::op_step() {
  bool break_event = false;

  usage[regs.pc] |= UsageExec;
  opcode_pc = regs.pc;

  if(debugger.step_smp) {
    debugger.break_event = Debugger::BreakEvent::SMPStep;
    scheduler.exit(Scheduler::ExitReason::DebuggerEvent);
  } else {
    debugger.breakpoint_test(Debugger::Breakpoint::Source::APURAM, Debugger::Breakpoint::Mode::Exec, regs.pc, 0x00);
  }

  if(step_event) step_event();
  SMP::op_step();
  synchronize_cpu();
}

uint8 SMPDebugger::op_read(uint16 addr) {
  uint8 data = SMP::op_read(addr);
  usage[addr] |= UsageRead;
  debugger.breakpoint_test(Debugger::Breakpoint::Source::APURAM, Debugger::Breakpoint::Mode::Read, addr, data);
  return data;
}

void SMPDebugger::op_write(uint16 addr, uint8 data) {
  SMP::op_write(addr, data);
  usage[addr] |= UsageWrite;
  usage[addr] &= ~UsageExec;
  debugger.breakpoint_test(Debugger::Breakpoint::Source::APURAM, Debugger::Breakpoint::Mode::Write, addr, data);
}

SMPDebugger::SMPDebugger() {
  usage = new uint8[1 << 16]();
  opcode_pc = 0xffc0;
}

SMPDebugger::~SMPDebugger() {
  delete[] usage;
}

//===========
//SMPDebugger
//===========

//$00f0
unsigned SMPDebugger::clock_speed() { return status.clock_speed; }
bool SMPDebugger::timers_enable() { return status.timers_enabled; }
bool SMPDebugger::ram_disable() { return status.ram_disabled; }
bool SMPDebugger::ram_writable() { return status.ram_writable; }
bool SMPDebugger::timers_disable() { return status.timers_disabled; }

//$00f1
bool SMPDebugger::iplrom_enable() { return status.iplrom_enabled; }

//$00f2
unsigned SMPDebugger::dsp_address() { return status.dsp_addr; }

#endif
