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
  MenuItem settingsInput;
  MenuItem settingsAdvanced;
  Menu tools;
  Menu toolsStateSave;
  MenuItem toolsStateSave1;
  MenuItem toolsStateSave2;
  MenuItem toolsStateSave3;
  MenuItem toolsStateSave4;
  MenuItem toolsStateSave5;
  Menu toolsStateLoad;
  MenuItem toolsStateLoad1;
  MenuItem toolsStateLoad2;
  MenuItem toolsStateLoad3;
  MenuItem toolsStateLoad4;
  MenuItem toolsStateLoad5;
  MenuSeparator toolsSeparator;
  MenuItem toolsCheatEditor;
  Menu help;
  MenuItem helpAbout;
  Viewport viewport;

  void create();
};

extern MainWindow mainWindow;
