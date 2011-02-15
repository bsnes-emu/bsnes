struct Console : TopLevelWindow {
  HorizontalLayout layout;
  TextEdit output;
  VerticalLayout controlLayout;
  CheckBox traceToConsole;
  CheckBox traceToFile;
  CheckBox traceCPU;
  CheckBox traceSMP;
  Label spacer;
  Button clearConsole;

  string buffer;
  file logfile;

  void create();
  void write(const string &text, bool echo = false);
  void tracerEnable(bool state);
  void eventBreakpoint();
  void eventTraceCPU();
  void eventTraceSMP();
};

extern Console console;

