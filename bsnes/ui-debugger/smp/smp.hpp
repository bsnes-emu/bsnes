struct SMPDebugger : Window {
  uint16 opcodePC;

  VerticalLayout layout;
    HorizontalLayout controlLayout;
      Button stepInto;
      Widget spacer;
      CheckBox autoUpdate;
      Button update;
    TextEdit disassembly;
    Label registers;

  uint8 read(uint16 addr);
  void write(uint16 addr, uint8 data);

  unsigned opcodeLength(uint16 addr);
  void updateDisassembly();
  SMPDebugger();
};

extern SMPDebugger *smpDebugger;
