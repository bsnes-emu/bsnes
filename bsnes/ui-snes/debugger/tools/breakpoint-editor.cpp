BreakpointEditor breakpointEditor;

void BreakpointEditor::create() {
  setTitle("Breakpoint Editor");
  application.addWindow(this, "Debugger.BreakpointEditor", "192,192");

  runToBreakpoint.setText("Run to breakpoint");
  for(unsigned n = 0; n < Breakpoints; n++) {
    enableBox[n].setText({ n + 1 });
    typeBox[n].append("Exec");
    typeBox[n].append("Read");
    typeBox[n].append("Write");
    sourceBox[n].append("CPU");
    sourceBox[n].append("APU");
    sourceBox[n].append("VRAM");
    sourceBox[n].append("OAM");
    sourceBox[n].append("CGRAM");
    enableBox[n].onTick = [n]() { breakpointEditor.toggleBreakpoint(n); };
  }

  layout.setMargin(5);
  layout.append(runToBreakpoint, ~0, 0, 5);
  for(unsigned n = 0; n < Breakpoints; n++) {
    breakpointLayout[n].append(enableBox[n],  35, 0   );
    breakpointLayout[n].append(addressBox[n], 60, 0, 5);
    breakpointLayout[n].append(valueBox[n],   30, 0, 5);
    breakpointLayout[n].append(typeBox[n],     0, 0, 5);
    breakpointLayout[n].append(sourceBox[n],   0, 0   );
    layout.append(breakpointLayout[n], ~0, 0, n < Breakpoints - 1 ? 5 : 0);
  }

  append(layout);
  setGeometry({ 0, 0, layout.minimumGeometry().width, layout.minimumGeometry().height });

  onClose = []() {
    debugger.showBreakpointEditor.setChecked(false);
  };

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
