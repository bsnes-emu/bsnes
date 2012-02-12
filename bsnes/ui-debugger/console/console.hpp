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
    Separator menuWindowsSeparator1;
    Item menuWindowsCPUDebugger;
    Item menuWindowsSMPDebugger;
    Separator menuWindowsSeparator2;
    Item menuWindowsMemoryEditor;
    Item menuWindowsBreakpointEditor;
    Item menuWindowsVRAMViewer;

  Menu menuState;
    Item menuStateSave1;
    Item menuStateSave2;
    Item menuStateSave3;
    Item menuStateSave4;
    Item menuStateSave5;
    Separator menuStateSeparator;
    Item menuStateLoad1;
    Item menuStateLoad2;
    Item menuStateLoad3;
    Item menuStateLoad4;
    Item menuStateLoad5;

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
    Label title;
    Label version;
    Label website;

  AboutWindow();
};

extern ConsoleWindow *consoleWindow;
extern AboutWindow *aboutWindow;
