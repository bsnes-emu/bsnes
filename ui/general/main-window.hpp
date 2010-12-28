struct MainWindow : Window {
  Menu system;
  MenuItem systemLoadCartridge;
  MenuSeparator systemSeparator1;
  MenuItem systemPower;
  MenuItem systemReset;

  Menu settings;

  Menu tools;

  Menu help;
  MenuItem helpAbout;

  Viewport viewport;

  void create();
};

extern MainWindow mainWindow;
