struct MainWindow : Window {
  Menu system;
  MenuItem systemQuit;
  Menu settings;
  Menu tools;
  Menu help;

  Viewport viewport;

  void create();
};

extern MainWindow mainWindow;
