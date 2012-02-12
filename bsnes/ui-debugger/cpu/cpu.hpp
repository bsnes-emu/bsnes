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
    Label registers;

  uint24 mirror(uint24 addr);
  uint8_t read(uint24 addr);
  void write(uint24 addr, uint8 data);

  unsigned opcodeLength(uint24 addr);
  void updateDisassembly();
  CPUDebugger();
};

extern CPUDebugger *cpuDebugger;
