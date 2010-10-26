CPUdebugger cpuDebugger;

void CPUdebugger::create() {
  Window::create(0, 0, 256, 256, "CPU Debugger");
  application.addWindow(this, "Debugger.CPUdebugger", "192,192");

  unsigned x = 5, y = 5;
  output.create(*this, x, y, 400, 200); x += 400 + 5;
  output.setFont(application.monospaceFont);
  output.setEditable(false);

  stepInto.create(*this, x, y, 80, Style::ButtonHeight, "Step Into");

  setGeometry(0, 0, 490, 205);

  stepInto.onTick = []() {
    SNES::debugger.step_cpu = true;
    debugger.debugMode = Debugger::DebugMode::StepIntoCPU;
  };
}

void CPUdebugger::eventStepInto() {
  SNES::debugger.step_cpu = false;
  char text[256];
  SNES::cpu.disassemble_opcode(text, SNES::cpu.regs.pc);
  text[21] = 0;
  output.setText({
    "  ...\n",
    "  ...\n",
    "  ...\n",
    "  ...\n",
    "  ...\n",
    "  ...\n",
    "  ...\n",
    "> ", text, "\n",
    "  ...\n",
    "  ...\n",
    "  ...\n",
    "  ...\n",
    "  ...\n",
    "  ...\n",
    "  ..."
  });
}
