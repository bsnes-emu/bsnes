struct CPUdebugger : TopLevelWindow {
  EditBox output;
  Button stepInto;

  void create();
  void eventStepInto();
};

extern CPUdebugger cpuDebugger;
