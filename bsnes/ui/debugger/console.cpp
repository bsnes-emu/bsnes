Console console;

void Console::create() {
  setTitle("Console");
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

  layout.setMargin(5);
  layout.append(output, 0, 0, 5);
  controlLayout.append(traceToConsole, 120, Style::CheckBoxHeight);
  controlLayout.append(traceToFile, 120, Style::CheckBoxHeight);
  controlLayout.append(traceCPU, 120, Style::CheckBoxHeight);
  controlLayout.append(traceSMP, 120, Style::CheckBoxHeight);
  controlLayout.append(spacer, 120, 0, Style::CheckBoxHeight);
  controlLayout.append(clearConsole, 120, Style::ButtonHeight);
  layout.append(controlLayout, 120, 0);

  setGeometry({ 0, 0, layout.minimumWidth() + 580, 350 });
  append(layout);

  onClose = []() {
    debugger.showConsole.setChecked(false);
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
    OS::processEvents();
  }
  if(traceToFile.checked() && logfile.open()) {
    logfile.print(string(text, "\n"));
  }
}

void Console::tracerEnable(bool state) {
  if(state == true) {
    logfile.open(path.load(SNES::Cartridge::Slot::Cartridge, "log"), file::mode::write);
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
