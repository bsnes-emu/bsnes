struct MainWindow : Window {
  Font statusFont;
  Menu system;
  MenuItem systemLoadCartridge;
  MenuSeparator systemSeparator;
  MenuItem systemQuit;
  Menu settings;
  MenuItem settingsVideo;
  Menu tools;
  Menu help;
  Viewport viewport;

  void create();
};

extern MainWindow mainWindow;
