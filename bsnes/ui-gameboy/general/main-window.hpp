struct MainWindow : Window {
  Menu system;
  Item systemLoadCartridge;
  Separator systemSeparator1;
  Item systemPower;

  Menu settings;
  CheckItem settingsVideoSync;
  CheckItem settingsAudioSync;

  Menu tools;
  Menu toolsSaveState;
  Item toolsSaveState1;
  Item toolsSaveState2;
  Item toolsSaveState3;
  Item toolsSaveState4;
  Item toolsSaveState5;
  Menu toolsLoadState;
  Item toolsLoadState1;
  Item toolsLoadState2;
  Item toolsLoadState3;
  Item toolsLoadState4;
  Item toolsLoadState5;
  Separator toolsSeparator1;
  CheckItem toolsTraceCPU;

  Menu help;
  Item helpAbout;

  FixedLayout layout;
  Viewport viewport;

  void create();
};

extern MainWindow mainWindow;
