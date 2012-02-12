//S-CPU
//=====

void Debugger::cpu_op_exec(uint24 addr) {
  cpuUsage.data[addr] |= Usage::Exec;
  cpuDebugger->opcodePC = addr;
  bool breakpointHit = breakpointEditor->testExecCPU(addr);

  if(fpTracer.open() || (debug.cpu && flags.step) || flags.cpu.stepInto || breakpointHit) {
    char text[512];
    SNES::cpu.disassemble_opcode(text, addr);

    if(fpTracer.open()) fpTracer.print(text, "\n");
    if((debug.cpu && flags.step) || flags.cpu.stepInto || breakpointHit) {
      print(text, "\n");
      if(debug.cpu && flags.step) {
        consoleWindow->stepButton.setFocused();
      }
      if(flags.cpu.stepInto) {
        cpuDebugger->stepInto.setFocused();
        cpuDebugger->updateDisassembly();
      }
      suspend();
      SNES::scheduler.debug();
    }
  }
}

void Debugger::cpu_op_read(uint24 addr) {
  cpuUsage.data[addr] |= Usage::Read;
  bool breakpointHit = breakpointEditor->testReadCPU(addr);

  if(breakpointHit) {
    char text[512];
    SNES::cpu.disassemble_opcode(text, cpuDebugger->opcodePC);
    print(text, "\n");

    suspend();
    SNES::scheduler.debug();
  }
}

void Debugger::cpu_op_write(uint24 addr, uint8 data) {
  cpuUsage.data[addr] |= Usage::Write;
  bool breakpointHit = breakpointEditor->testWriteCPU(addr, data);

  if(breakpointHit) {
    char text[512];
    SNES::cpu.disassemble_opcode(text, cpuDebugger->opcodePC);
    print(text, "\n");

    suspend();
    SNES::scheduler.debug();
  }
}

//S-SMP
//=====

void Debugger::smp_op_exec(uint16 addr) {
  apuUsage.data[addr] |= Usage::Exec;
}

void Debugger::smp_op_read(uint16 addr) {
  apuUsage.data[addr] |= Usage::Read;
}

void Debugger::smp_op_write(uint16 addr, uint8 data) {
  apuUsage.data[addr] |= Usage::Write;
}
