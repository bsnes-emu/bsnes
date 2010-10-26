struct Console : TopLevelWindow {
  EditBox output;
  CheckBox traceToConsole;
  CheckBox traceToDisk;
  CheckBox traceCPU;

  string buffer;

  void create();
  void eventTraceCPU();
};

extern Console console;

