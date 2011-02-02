BreakpointEditor breakpointEditor;

void BreakpointEditor::create() {
  Window::create(0, 0, 256, 256, "Breakpoint Editor");
  application.addWindow(this, "Debugger.BreakpointEditor", "192,192");

  unsigned x = 5, y = 5;

  runToBreakpoint.create(*this, x, y, 295, Style::CheckBoxHeight, "Run to breakpoint");
  y += Style::CheckBoxHeight + 5;

  for(unsigned n = 0; n < Breakpoints; n++) {
    enableBox[n].create(*this, x, y, 35, Style::EditBoxHeight, { n + 1 });
    addressBox[n].create(*this, x + 35, y, 60, Style::EditBoxHeight);
    valueBox[n].create(*this, x + 100, y, 30, Style::EditBoxHeight);
    typeBox[n].create(*this, x + 135, y, 80, Style::EditBoxHeight, "Exec\nRead\nWrite");
    sourceBox[n].create(*this, x + 220, y, 80, Style::EditBoxHeight, "CPU\nAPU\nVRAM\nOAM\nCGRAM");
    y += Style::EditBoxHeight + 5;

    enableBox[n].onTick = [n]() { breakpointEditor.toggleBreakpoint(n); };
  }

  setGeometry(0, 0, 310, y);

  runToBreakpoint.onTick = []() {
    if(breakpointEditor.runToBreakpoint.checked()) {
      debugger.debugMode = Debugger::DebugMode::WaitForBreakpoint;
    } else {
      debugger.debugMode = Debugger::DebugMode::None;
    }
    debugger.synchronize();
  };
}

void BreakpointEditor::toggleBreakpoint(unsigned n) {
  bool enabled = enableBox[n].checked();
  if(enabled == false) {
    SNES::debugger.breakpoint[n].enabled = false;
  } else {
    SNES::debugger.breakpoint[n].enabled = true;
    SNES::debugger.breakpoint[n].addr = hex(addressBox[n].text());
    SNES::debugger.breakpoint[n].data = hex(valueBox[n].text());
    if(valueBox[n].text() == "") SNES::debugger.breakpoint[n].data = -1;  //break on any value
    SNES::debugger.breakpoint[n].mode = (SNES::Debugger::Breakpoint::Mode)typeBox[n].selection();
    SNES::debugger.breakpoint[n].source = (SNES::Debugger::Breakpoint::Source)sourceBox[n].selection();
    SNES::debugger.breakpoint[n].counter = 0;
  }

  //do not allow values to be edited while breakpoint is enabled
  addressBox[n].setEnabled(!enabled);
  valueBox[n].setEnabled(!enabled);
  typeBox[n].setEnabled(!enabled);
  sourceBox[n].setEnabled(!enabled);
}

void BreakpointEditor::eventBreakpoint() {
  runToBreakpoint.setChecked(false);
}
