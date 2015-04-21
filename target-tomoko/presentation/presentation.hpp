struct Presentation : Window {
  Presentation();
  auto resizeViewport() -> void;
  auto toggleFullScreen() -> void;
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
        MenuCheckItem maskOverscan{&videoScaleMenu};
      Menu videoFilterMenu{&settingsMenu};
        MenuRadioItem videoFilterNone{&videoFilterMenu};
        MenuRadioItem videoFilterBlur{&videoFilterMenu};
        MenuSeparator videoFilterSeparator{&videoFilterMenu};
        MenuCheckItem colorEmulation{&videoFilterMenu};
      MenuSeparator settingsMenuSeparator1{&settingsMenu};
      MenuCheckItem synchronizeVideo{&settingsMenu};
      MenuCheckItem synchronizeAudio{&settingsMenu};
      MenuCheckItem muteAudio{&settingsMenu};
      MenuCheckItem showStatusBar{&settingsMenu};
      MenuSeparator settingsMenuSeparator2{&settingsMenu};
      MenuItem showConfiguration{&settingsMenu};
    Menu toolsMenu{&menuBar};
      Menu saveStateMenu{&toolsMenu};
        MenuItem saveSlot1{&saveStateMenu};
        MenuItem saveSlot2{&saveStateMenu};
        MenuItem saveSlot3{&saveStateMenu};
        MenuItem saveSlot4{&saveStateMenu};
        MenuItem saveSlot5{&saveStateMenu};
      Menu loadStateMenu{&toolsMenu};
        MenuItem loadSlot1{&loadStateMenu};
        MenuItem loadSlot2{&loadStateMenu};
        MenuItem loadSlot3{&loadStateMenu};
        MenuItem loadSlot4{&loadStateMenu};
        MenuItem loadSlot5{&loadStateMenu};
      MenuSeparator toolsMenuSeparator{&toolsMenu};
      MenuItem cheatEditor{&toolsMenu};
      MenuItem stateManager{&toolsMenu};

  FixedLayout layout{this};
    Viewport viewport{&layout, Geometry{0, 0, 1, 1}};

  StatusBar statusBar{this};
};

extern Presentation* presentation;
