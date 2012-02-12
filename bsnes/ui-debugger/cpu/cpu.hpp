struct CPUDebugger : Window {
  uint24 opcodePC;

  VerticalLayout layout;
    HorizontalLayout controlLayout;
      Button stepInto;
      Button stepOver;
      Button stepOut;
      Button skipOver;
      Widget spacer;
      CheckBox autoRefresh;
      Button update;
    TextEdit disassembly;
    Label registers;

  unsigned opcodeLength(uint24 addr) const;
  void updateDisassembly();
  CPUDebugger();
};

extern CPUDebugger *cpuDebugger;
