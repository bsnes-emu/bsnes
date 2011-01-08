struct MainWindow : Window {
  Menu system;
  MenuItem systemLoadCartridge;
  MenuSeparator systemSeparator1;
  MenuItem systemPower;

  Menu settings;
  MenuCheckItem settingsVideoSync;

  Menu tools;
  Menu toolsSaveState;
  MenuItem toolsSaveState1;
  MenuItem toolsSaveState2;
  MenuItem toolsSaveState3;
  MenuItem toolsSaveState4;
  MenuItem toolsSaveState5;
  Menu toolsLoadState;
  MenuItem toolsLoadState1;
  MenuItem toolsLoadState2;
  MenuItem toolsLoadState3;
  MenuItem toolsLoadState4;
  MenuItem toolsLoadState5;
  MenuSeparator toolsSeparator1;
  MenuCheckItem toolsTraceCPU;

  Menu help;
  MenuItem helpAbout;

  Viewport viewport;

  void create();
};

extern MainWindow mainWindow;
