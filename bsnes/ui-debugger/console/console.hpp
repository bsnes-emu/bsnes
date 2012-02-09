struct ConsoleWindow : Window {
  Menu menuEmulation;
    Item menuEmulationReloadCartridge;
    Item menuEmulationPowerCycle;
    Item menuEmulationReset;
    Separator menuEmulationSeparator;
    CheckItem menuEmulationSynchronizeAudio;
    CheckItem menuEmulationMuteAudio;

  Menu menuDebug;
    CheckItem menuDebugCPU;
    CheckItem menuDebugSMP;

  Menu menuTracer;
    CheckItem menuTracerEnable;
    CheckItem menuTracerMask;
    Item menuTracerMaskReset;

  Menu menuWindows;
    Item menuWindowsVideoWindow;
    Item menuWindowsMemoryEditor;
    Item menuWindowsBreakpointEditor;

  Menu menuHelp;
    Item menuHelpAbout;

  VerticalLayout layout;
    HorizontalLayout commandLayout;
      Button runButton;
      Button stepButton;
      Widget spacer;
      Button clearButton;
    TextEdit console;

  void print(const string &text);

  ConsoleWindow();
};

struct AboutWindow : Window {
  VerticalLayout layout;
    Canvas canvas;
    HorizontalLayout titleLayout;
      Widget titleL;
      Label title;
      Widget titleR;
    HorizontalLayout versionLayout;
      Widget versionL;
      Label version;
      Widget versionR;

  void show();
  AboutWindow();
};

extern ConsoleWindow *consoleWindow;
extern AboutWindow *aboutWindow;
