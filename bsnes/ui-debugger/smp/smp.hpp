struct SMPDebugger : Window {
  uint16 opcodePC;

  VerticalLayout layout;
    HorizontalLayout controlLayout;
      Button stepInto;
      Widget spacer;
      CheckBox autoUpdate;
      Button update;
    TextEdit disassembly;
    Button registers;

  uint8 read(uint16 addr);
  void write(uint16 addr, uint8 data);

  unsigned opcodeLength(uint16 addr);
  void updateDisassembly();
  SMPDebugger();
};

struct SMPRegisterEditor : Window {
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
    HorizontalLayout secondaryLayout;
      CheckBox flagN;
      CheckBox flagV;
      CheckBox flagP;
      CheckBox flagB;
      CheckBox flagH;
      CheckBox flagI;
      CheckBox flagZ;
      CheckBox flagC;
    HorizontalLayout tertiaryLayout;
      Widget spacer;
      Button update;

  void loadRegisters();
  void saveRegisters();
  SMPRegisterEditor();
};

extern SMPDebugger *smpDebugger;
extern SMPRegisterEditor *smpRegisterEditor;
