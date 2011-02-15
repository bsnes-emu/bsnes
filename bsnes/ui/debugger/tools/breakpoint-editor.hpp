struct BreakpointEditor : TopLevelWindow {
  enum : unsigned { Breakpoints = SNES::Debugger::Breakpoints };
  VerticalLayout layout;
  CheckBox runToBreakpoint;
  HorizontalLayout breakpointLayout[Breakpoints];
  CheckBox enableBox[Breakpoints];
  LineEdit addressBox[Breakpoints];
  LineEdit valueBox[Breakpoints];
  ComboBox typeBox[Breakpoints];
  ComboBox sourceBox[Breakpoints];

  void create();
  void toggleBreakpoint(unsigned breakpoint);
  void eventBreakpoint();
};

extern BreakpointEditor breakpointEditor;
