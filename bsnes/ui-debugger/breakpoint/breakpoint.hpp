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
  BreakpointEntry breakpointEntry[8];

  struct Breakpoint {
    enum : unsigned { Read, Write, Exec };
    enum : unsigned { CPU, APU, VRAM, OAM, CGRAM };
    unsigned id;
    bool compare;
    unsigned addr;
    unsigned data;
    unsigned type;
    unsigned source;
  };
  vector<Breakpoint> breakpoint;
  vector<Breakpoint> breakpointReadCPU;
  vector<Breakpoint> breakpointWriteCPU;
  vector<Breakpoint> breakpointExecCPU;
  void synchronize();

  bool testReadCPU(uint24 addr);
  bool testWriteCPU(uint24 addr, uint8 data);
  bool testExecCPU(uint24 addr);

  BreakpointEditor();
};

extern BreakpointEditor *breakpointEditor;
