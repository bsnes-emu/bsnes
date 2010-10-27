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

  setGeometry(0, 0, 775, 338);
}

void Console::eventTraceCPU() {
  if(traceCPU.checked() == false) return;
  if(traceToConsole.checked() == false) return;

  char text[256];
  SNES::cpu.disassemble_opcode(text, SNES::cpu.regs.pc);
  buffer.append(string(buffer == "" ? "" : "\n", text));
  output.setText(buffer);
}
