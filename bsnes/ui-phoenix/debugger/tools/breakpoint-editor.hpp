struct BreakpointEditor : TopLevelWindow {
  enum : unsigned { Breakpoints = SNES::Debugger::Breakpoints };
  CheckBox runToBreakpoint;
  CheckBox enableBox[Breakpoints];
  TextBox addressBox[Breakpoints];
  TextBox valueBox[Breakpoints];
  ComboBox typeBox[Breakpoints];
  ComboBox sourceBox[Breakpoints];

  void create();
  void toggleBreakpoint(unsigned breakpoint);
  void eventBreakpoint();
};

extern BreakpointEditor breakpointEditor;
