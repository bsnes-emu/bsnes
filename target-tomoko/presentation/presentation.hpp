struct Presentation : Window {
  Presentation();
  auto resizeViewport() -> void;
  auto drawSplashScreen() -> void;

  MenuBar menuBar{this};
    Menu libraryMenu{&menuBar};
      vector<MenuItem*> loadBootableMedia;
    Menu systemMenu{&menuBar};
      MenuItem powerSystem{&systemMenu};
      MenuItem resetSystem{&systemMenu};
      MenuSeparator systemMenuSeparator{&systemMenu};
      MenuItem unloadSystem{&systemMenu};
    Menu settingsMenu{&menuBar};
      Menu videoScaleMenu{&settingsMenu};
        MenuRadioItem videoScaleSmall{&videoScaleMenu};
        MenuRadioItem videoScaleNormal{&videoScaleMenu};
        MenuRadioItem videoScaleLarge{&videoScaleMenu};
        MenuSeparator videoScaleSeparator{&videoScaleMenu};
        MenuCheckItem aspectCorrection{&videoScaleMenu};
      MenuSeparator settingsMenuSeparator1{&settingsMenu};
      MenuCheckItem synchronizeVideo{&settingsMenu};
      MenuCheckItem synchronizeAudio{&settingsMenu};
      MenuCheckItem muteAudio{&settingsMenu};
      MenuSeparator settingsMenuSeparator2{&settingsMenu};
      MenuItem showConfiguration{&settingsMenu};
    Menu toolsMenu{&menuBar};

  VerticalLayout layout{this};
    Viewport viewport{&layout, Size{~0, ~0}};

  StatusBar statusBar{this};
};

extern Presentation* presentation;
