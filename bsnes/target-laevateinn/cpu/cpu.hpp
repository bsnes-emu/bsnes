struct CPUDebugger : Window {
  uint24 opcodePC;

  VerticalLayout layout;
    HorizontalLayout controlLayout;
      Button stepInto;
      Button stepNMI;
      Button stepIRQ;
      Widget spacer;
      CheckBox autoUpdate;
      Button update;
    TextEdit disassembly;
    Button registers;

  uint24 mirror(uint24 addr);
  uint8_t read(uint24 addr);
  void write(uint24 addr, uint8 data);

  unsigned opcodeLength(uint24 addr);
  void updateDisassembly();
  CPUDebugger();
};

struct CPURegisterEditor : Window {
  VerticalLayout layout;
    HorizontalLayout primaryLayout;
      Label regALabel;
      LineEdit regAValue;
      Label regXLabel;
      LineEdit regXValue;
      Label regYLabel;
      LineEdit regYValue;
      Label regSLabel;
      LineEdit regSValue;
      Label regDLabel;
      LineEdit regDValue;
      Label regDBLabel;
      LineEdit regDBValue;
    HorizontalLayout secondaryLayout;
      CheckBox flagN;
      CheckBox flagV;
      CheckBox flagM;
      CheckBox flagX;
      CheckBox flagD;
      CheckBox flagI;
      CheckBox flagZ;
      CheckBox flagC;
    HorizontalLayout tertiaryLayout;
      CheckBox flagE;
      Widget spacer;
      Button update;

  void loadRegisters();
  void saveRegisters();
  CPURegisterEditor();
};

extern CPUDebugger *cpuDebugger;
extern CPURegisterEditor *cpuRegisterEditor;
