void Debugger::cpu_op_exec(uint24 addr) {
  if(flags.debugCPU == false) return;

  char text[512];
  if(fpTracer.open() || flags.step) {
    SNES::cpu.disassemble_opcode(text, addr);
  }

  if(fpTracer.open()) {
    fpTracer.print(text, "\n");
  }

  if(flags.step) {
    flags.step = false;
    print(text, "\n");
    suspend();
    consoleWindow->stepButton.setFocused();
    SNES::scheduler.debug();
  }
}

void Debugger::cpu_op_read(uint24 addr) {
}

void Debugger::cpu_op_write(uint24 addr, uint8 data) {
}
