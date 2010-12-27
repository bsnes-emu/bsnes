CPUDebugger cpuDebugger;

void CPUDebugger::create() {
  Window::create(0, 0, 256, 256, "CPU Debugger");
  application.addWindow(this, "Debugger.CPUdebugger", "192,192");

  unsigned x = 5, y = 5;
  output.create(*this, x, y, 400, 200); x += 400 + 5;
  output.setFont(application.monospaceFont);
  output.setEditable(false);

  stepInto.create(*this, x, y, 80, Style::ButtonHeight, "Step Into"); y += Style::ButtonHeight;
  stepOver.create(*this, x, y, 80, Style::ButtonHeight, "Step Over"); y += Style::ButtonHeight;
  proceed.create(*this, x, y, 80, Style::ButtonHeight, "Proceed"); y += Style::ButtonHeight;
  proceed.setEnabled(false);

  setGeometry(0, 0, 490, 205);

  onClose = []() {
    debugger.showCPUDebugger.setChecked(false);
    return true;
  };

  stepInto.onTick = []() {
    debugger.debugMode = Debugger::DebugMode::StepIntoCPU;
  };

  stepOver.onTick = { &CPUDebugger::eventStepOver, this };
}

void CPUDebugger::synchronize() {
  stepInto.setEnabled(SNES::cartridge.loaded() && debugger.enableDebugger.checked());
  stepOver.setEnabled(stepInto.enabled() && SNES::cpu.opcode_edge);
}

void CPUDebugger::refreshDisassembly() {
  unsigned addr = SNES::cpu.regs.pc;
  uint8_t *usage = SNES::cpu.usage;

  signed offset[15];
  foreach(n, offset) n = -1;

  offset[7] = addr;

  //reverse disassembly
  for(signed n = 6; n >= 0; n--) {
    signed base = offset[n + 1];
    if(base == -1) break;

    for(unsigned r = 1; r <= 4; r++) {
      if(usage[(base - r) & 0xffffff] & 0x20) {
        offset[n] = base - r;
        break;
      }
    }
  }

  //forward disassembly
  for(signed n = 8; n <= 14; n++) {
    signed base = offset[n - 1];
    if(base == -1) break;

    for(unsigned r = 1; r <= 4; r++) {
      if(usage[(base + r) & 0xffffff] & 0x20) {
        offset[n] = base + r;
        break;
      }
    }
  }

  string buffer;
  for(unsigned n = 0; n < 15; n++) {
    buffer.append(n == 7 ? "> " : "  ");
    if(offset[n] == -1) {
      buffer.append("...\n");
    } else {
      unsigned addr = offset[n];
      buffer.append(hex<6>(addr));
      buffer.append(" ");
      string text = SNESCPU::disassemble(
        addr, usage[addr] & 2, usage[addr] & 1,
        read(addr + 0), read(addr + 1), read(addr + 2), read(addr + 3)
      );

      buffer.append(text);
      buffer.append("\n");
    }
  }
  buffer.rtrim<1>("\n");
  output.setText(buffer);
}

void CPUDebugger::eventStepInto() {
  refreshDisassembly();
}

void CPUDebugger::eventStepOver() {
  uint8_t opcode = read(SNES::cpu.regs.pc);
  unsigned length = SNESCPU::getOpcodeLength(SNES::cpu.regs.p.m, SNES::cpu.regs.p.x, opcode);
  SNES::cpu.regs.pc += length;
  refreshDisassembly();
  console.eventTraceCPU();
}

uint8_t CPUDebugger::read(unsigned addr) {
  return SNES::debugger.read(SNES::Debugger::MemorySource::CPUBus, addr);
}
