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
    Separator settingsSeparator;
    Item settingsConfiguration;

  Menu toolsMenu;
    Menu toolsStateSave;
      Item toolsStateSave1;
      Item toolsStateSave2;
      Item toolsStateSave3;
      Item toolsStateSave4;
      Item toolsStateSave5;
    Menu toolsStateLoad;
      Item toolsStateLoad1;
      Item toolsStateLoad2;
      Item toolsStateLoad3;
      Item toolsStateLoad4;
      Item toolsStateLoad5;
    Separator toolsSeparator;
    Item toolsShrinkWindow;
    Item toolsCheatEditor;
    Item toolsStateManager;
    CheckItem toolsTest;

  Menu helpMenu;
    Item helpAbout;

  void synchronize();
  MainWindow();
};

extern MainWindow *mainWindow;
