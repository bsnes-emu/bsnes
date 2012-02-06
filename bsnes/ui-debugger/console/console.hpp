struct ConsoleWindow : Window {
  Menu menuEmulation;
    Item menuEmulationReloadCartridge;
    Item menuEmulationPowerCycle;
    Item menuEmulationReset;
    Separator menuEmulationSeparator;
    CheckItem menuEmulationSynchronizeAudio;
    CheckItem menuEmulationMuteAudio;

  Menu menuWindows;
    Item menuWindowsVideo;

  VerticalLayout layout;
    HorizontalLayout commandLayout;
      Button runButton;
      Button stepButton;
    TextEdit console;

  void print(const string &text);

  ConsoleWindow();
};

extern ConsoleWindow *consoleWindow;
