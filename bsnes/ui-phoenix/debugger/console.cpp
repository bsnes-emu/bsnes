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
  traceToDisk.create(*this, x, y, 120, Style::CheckBoxHeight, "Trace to disk"); y += Style::CheckBoxHeight;
  traceToDisk.setEnabled(false);
  traceCPU.create(*this, x, y, 120, Style::CheckBoxHeight, "Trace S-CPU"); y += Style::CheckBoxHeight;
  traceCPU.setChecked(true);
  traceSMP.create(*this, x, y, 120, Style::CheckBoxHeight, "Trace S-SMP"); y += Style::CheckBoxHeight;

  setGeometry(0, 0, 775, 338);

  onClose = []() {
    debugger.showConsole.setChecked(false);
    return true;
  };
}

void Console::eventBreakpoint() {
  if(traceToConsole.checked() == false) return;

  unsigned n = SNES::debugger.breakpoint_hit;
  string text = { "Breakpoint ", n + 1, " hit." };
  buffer.append(string(buffer == "" ? "" : "\n", text));
  output.setText(buffer);
  output.setCursorPosition(~0);

  if(SNES::debugger.breakpoint[n].source == SNES::Debugger::Breakpoint::Source::CPUBus) {
    eventTraceCPU();
  } else if(SNES::debugger.breakpoint[n].source == SNES::Debugger::Breakpoint::Source::APURAM) {
    eventTraceSMP();
  }
}

void Console::eventTraceCPU() {
  if(traceCPU.checked() == false) return;
  if(traceToConsole.checked() == false) return;

  char text[256];
  SNES::cpu.disassemble_opcode(text, SNES::cpu.regs.pc);
  buffer.append(string(buffer == "" ? "" : "\n", text));
  output.setText(buffer);
  output.setCursorPosition(~0);
}

void Console::eventTraceSMP() {
  if(traceSMP.checked() == false) return;
  if(traceToConsole.checked() == false) return;

  char text[256];
  SNES::smp.disassemble_opcode(text, SNES::smp.regs.pc);
  buffer.append(string(buffer == "" ? "" : "\n", text));
  output.setText(buffer);
  output.setCursorPosition(~0);
}
