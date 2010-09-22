struct MainWindow : Window {
  Menu system;
  MenuItem systemLoadCartridge;
  MenuSeparator systemSeparator;
  MenuItem systemQuit;
  Menu settings;
  MenuCheckItem settingsSynchronizeVideo;
  MenuCheckItem settingsSynchronizeAudio;
  MenuCheckItem settingsMuteAudio;
  MenuSeparator settingsSeparator;
  MenuItem settingsVideo;
  MenuItem settingsAdvanced;
  Menu tools;
  MenuItem toolsCheatEditor;
  Menu help;
  Viewport viewport;

  void create();
};

extern MainWindow mainWindow;
