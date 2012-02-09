struct BreakpointEntry : HorizontalLayout {
  CheckBox enable;
  LineEdit addr;
  LineEdit data;
  ComboBox type;
  ComboBox source;

  BreakpointEntry();
};

struct BreakpointEditor : Window {
  VerticalLayout layout;
  BreakpointEntry breakpoint[5];

  BreakpointEditor();
};

extern BreakpointEditor *breakpointEditor;
