Console console;

void Console::create() {
  Window::create(0, 0, 256, 256, "Console");
  application.addWindow(this, "Debugger.Console", "192,192");

  output.setFont(application.monospaceFont);
  output.setEditable(false);
  traceToConsole.setText("Trace to console");
  traceToFile.setText("Trace to file");
  traceCPU.setText("Trace CPU");
  traceSMP.setText("Trace SMP");
  traceToConsole.setChecked(true);
  traceCPU.setChecked(true);
  clearConsole.setText("Clear console");

  unsigned x = 5, y = 5;
  layout.append(output, x, y, 580, 338); x += 580 + 5;
  layout.append(traceToConsole, x, y, 120, Style::CheckBoxHeight); y += Style::CheckBoxHeight;
  layout.append(traceToFile, x, y, 120, Style::CheckBoxHeight); y += Style::CheckBoxHeight;
  layout.append(traceCPU, x, y, 120, Style::CheckBoxHeight); y += Style::CheckBoxHeight;
  layout.append(traceSMP, x, y, 120, Style::CheckBoxHeight); y += Style::CheckBoxHeight;
  layout.append(clearConsole, x, 348 - Style::ButtonHeight - 5, 120, Style::ButtonHeight);
  setGeometry(0, 0, 715, 348);
  setLayout(layout);

  onClose = []() {
    debugger.showConsole.setChecked(false);
    return true;
  };

  traceToFile.onTick = []() { console.tracerEnable(console.traceToFile.checked()); };

  clearConsole.onTick = []() {
    console.buffer = "";
    console.output.setText(console.buffer);
  };
}

void Console::write(const string &text, bool echo) {
  if(traceToConsole.checked() || echo) {
    if(buffer != "") buffer.append("\n");
    buffer.append(text);
    output.setText(buffer);
    output.setCursorPosition(~0);
    OS::run();
  }
  if(traceToFile.checked() && logfile.open()) {
    logfile.print(string(text, "\n"));
  }
}

void Console::tracerEnable(bool state) {
  if(state == true) {
    string filename = { cartridge.baseName, ".log" };
    logfile.open(filename, file::mode::write);
  } else {
    logfile.close();
  }
}

void Console::eventBreakpoint() {
  unsigned n = SNES::debugger.breakpoint_hit;
  write({ "Breakpoint ", n + 1, " hit." }, true);

  if(SNES::debugger.breakpoint[n].source == SNES::Debugger::Breakpoint::Source::CPUBus) {
    eventTraceCPU();
    cpuDebugger.refreshDisassembly();
  } else if(SNES::debugger.breakpoint[n].source == SNES::Debugger::Breakpoint::Source::APURAM) {
    eventTraceSMP();
    smpDebugger.refreshDisassembly();
  }
}

void Console::eventTraceCPU() {
  if(traceCPU.checked() == false) return;
  if(traceToConsole.checked() == false && traceToFile.checked() == false) return;

  char text[256];
  SNES::cpu.disassemble_opcode(text, SNES::cpu.regs.pc);
  write(text);
}

void Console::eventTraceSMP() {
  if(traceSMP.checked() == false) return;
  if(traceToConsole.checked() == false && traceToFile.checked() == false) return;

  char text[256];
  SNES::smp.disassemble_opcode(text, SNES::smp.regs.pc);
  write(text);
}
