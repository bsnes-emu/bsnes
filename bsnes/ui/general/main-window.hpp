struct MainWindow : Window {
  FixedLayout layout;
  Viewport viewport;

  Menu cartridgeMenu;
    Item cartridgeLoadSNES;
    Item cartridgeLoadNES;
    Item cartridgeLoadGameBoy;

  Menu nesMenu;
    Item nesPower;
    Item nesReset;
    Separator nesSeparator;
    Item nesCartridgeUnload;

  Menu snesMenu;
    Item snesPower;
    Item snesReset;
    Separator snesSeparator;
    Item snesCartridgeUnload;

  Menu gameBoyMenu;
    Item gameBoyPower;
    Separator gameBoySeparator;
    Item gameBoyCartridgeUnload;

  Menu settingsMenu;
    CheckItem settingsSynchronizeVideo;
    CheckItem settingsSynchronizeAudio;
    CheckItem settingsMuteAudio;

  Menu helpMenu;
    Item helpAbout;

  MainWindow();
};

extern MainWindow *mainWindow;
