struct Presentation : Window {
  Presentation();

  MenuBar menuBar{this};
    Menu libraryMenu{&menuBar};
      vector<MenuItem*> loadBootableMedia;
    Menu superFamicomMenu{&menuBar};
    Menu settingsMenu{&menuBar};
    Menu toolsMenu{&menuBar};

  VerticalLayout layout{this};
    Viewport viewport{&layout, Size{~0, ~0}};

  StatusBar statusBar{this};
};

extern Presentation* presentation;
