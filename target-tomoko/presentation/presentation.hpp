struct Presentation : Window {
  Presentation();

  MenuBar menuBar{this};
    Menu libraryMenu{&menuBar};
      vector<MenuItem*> loadBootableMedia;
    Menu systemMenu{&menuBar};
      MenuItem powerSystem{&systemMenu};
      MenuItem resetSystem{&systemMenu};
      MenuSeparator systemMenuSeparator{&systemMenu};
      MenuItem unloadSystem{&systemMenu};
    Menu settingsMenu{&menuBar};
      MenuItem showConfiguration{&settingsMenu};
    Menu toolsMenu{&menuBar};

  VerticalLayout layout{this};
    Viewport viewport{&layout, Size{~0, ~0}};

  StatusBar statusBar{this};
};

extern Presentation* presentation;
