struct SMPDebugger : TopLevelWindow {
  EditBox output;
  Button stepInto;
  Button stepOver;
  Button proceed;

  void create();
  void synchronize();
  void refreshDisassembly();
  void eventStepInto();
  void eventStepOver();

  uint8_t read(uint16_t addr);
};

extern SMPDebugger smpDebugger;
