BreakpointEditor breakpointEditor;

void BreakpointEditor::create() {
  Window::create(0, 0, 256, 256, "Breakpoint Editor");
  application.addWindow(this, "Debugger.BreakpointEditor", "192,192");

  runToBreakpoint.setText("Run to breakpoint");
  for(unsigned n = 0; n < Breakpoints; n++) {
    enableBox[n].setText({ n + 1 });
    typeBox[n].addItem("Exec");
    typeBox[n].addItem("Read");
    typeBox[n].addItem("Write");
    sourceBox[n].addItem("CPU");
    sourceBox[n].addItem("APU");
    sourceBox[n].addItem("VRAM");
    sourceBox[n].addItem("OAM");
    sourceBox[n].addItem("CGRAM");
    enableBox[n].onTick = [n]() { breakpointEditor.toggleBreakpoint(n); };
  }

  unsigned x = 5, y = 5;
  layout.append(runToBreakpoint, x, y, 295, Style::CheckBoxHeight); y += Style::CheckBoxHeight + 5;
  for(unsigned n = 0; n < Breakpoints; n++) {
    layout.append(enableBox[n], x, y, 35, Style::EditBoxHeight);
    layout.append(addressBox[n], x + 35, y, 60, Style::EditBoxHeight);
    layout.append(valueBox[n], x + 100, y, 30, Style::EditBoxHeight);
    layout.append(typeBox[n], x + 135, y, 80, Style::EditBoxHeight);
    layout.append(sourceBox[n], x + 220, y, 80, Style::EditBoxHeight); y += Style::EditBoxHeight + 5;
  }
  setGeometry(0, 0, 310, y);
  setLayout(layout);

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
