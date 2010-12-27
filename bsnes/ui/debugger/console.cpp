Console console;

void Console::create() {
  Window::create(0, 0, 256, 256, "Console");
  application.addWindow(this, "Debugger.Console", "192,192");

  unsigned x = 5, y = 5;
  output.create(*this, x, y, 580, 328); x += 580 + 5;
  output.setFont(application.monospaceFont);
  output.setEditable(false);

  traceToConsole.create(*this, x, y, 120, Style::CheckBoxHeight, "Trace to console"); y += Style::CheckBoxHeight;
  traceToConsole.setChecked(true);
  traceToFile.create(*this, x, y, 120, Style::CheckBoxHeight, "Trace to file"); y += Style::CheckBoxHeight;
  traceCPU.create(*this, x, y, 120, Style::CheckBoxHeight, "Trace S-CPU"); y += Style::CheckBoxHeight;
  traceCPU.setChecked(true);
  traceSMP.create(*this, x, y, 120, Style::CheckBoxHeight, "Trace S-SMP"); y += Style::CheckBoxHeight;

  clearConsole.create(*this, x, 338 - Style::ButtonHeight - 5, 120, Style::ButtonHeight, "Clear console");

  setGeometry(0, 0, 710, 338);

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

void Console::write(const string &text) {
  if(traceToConsole.checked()) {
    if(buffer != "") buffer.append("\n");
    buffer.append(text);
    output.setText(buffer);
    output.setCursorPosition(~0);
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
  if(traceToConsole.checked() == false) return;

  unsigned n = SNES::debugger.breakpoint_hit;
  write({ "Breakpoint ", n + 1, " hit." });

  if(SNES::debugger.breakpoint[n].source == SNES::Debugger::Breakpoint::Source::CPUBus) {
    eventTraceCPU();
  } else if(SNES::debugger.breakpoint[n].source == SNES::Debugger::Breakpoint::Source::APURAM) {
    eventTraceSMP();
  }
}

void Console::eventTraceCPU() {
  if(traceCPU.checked() == false) return;

  char text[256];
  SNES::cpu.disassemble_opcode(text, SNES::cpu.regs.pc);
  write(text);
}

void Console::eventTraceSMP() {
  if(traceSMP.checked() == false) return;

  char text[256];
  SNES::smp.disassemble_opcode(text, SNES::smp.regs.pc);
  write(text);
}
