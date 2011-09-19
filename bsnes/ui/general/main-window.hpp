struct MainWindow : Window {
  FixedLayout layout;
  Viewport viewport;

  Menu cartridgeMenu;
    Item cartridgeLoadSNES;
    Item cartridgeLoadNES;
    Item cartridgeLoadGameBoy;
    Separator cartridgeSeparator;
    Item cartridgeLoadSatellaviewSlotted;
    Item cartridgeLoadSatellaview;
    Item cartridgeLoadSufamiTurbo;
    Item cartridgeLoadSuperGameBoy;

  Menu nesMenu;
    Item nesPower;
    Item nesReset;
    Separator nesSeparator1;
    Menu nesPort1;
      RadioItem nesPort1Device[2];
    Menu nesPort2;
      RadioItem nesPort2Device[2];
    Separator nesSeparator2;
    Item nesCartridgeUnload;

  Menu snesMenu;
    Item snesPower;
    Item snesReset;
    Separator snesSeparator1;
    Menu snesPort1;
      RadioItem snesPort1Device[4];
    Menu snesPort2;
      RadioItem snesPort2Device[8];
    Separator snesSeparator2;
    Item snesCartridgeUnload;

  Menu gameBoyMenu;
    Item gameBoyPower;
    Separator gameBoySeparator;
    Item gameBoyCartridgeUnload;

  Menu settingsMenu;
    Menu settingsVideoShaders;
      RadioItem *settingsVideoShader;
    Separator settingsSeparator1;
    CheckItem settingsSynchronizeVideo;
    CheckItem settingsSynchronizeAudio;
    CheckItem settingsSmoothVideo;
    CheckItem settingsMuteAudio;
    Separator settingsSeparator2;
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
    Separator toolsSeparator1;
    Item toolsCaptureMouse;
    Item toolsShrinkWindow;
    Separator toolsSeparator2;
    Item toolsCheatEditor;
    Item toolsStateManager;
    Separator toolsSeparator3;
    CheckItem toolsTest;

  void synchronize();
  MainWindow();

private:
  unsigned videoShaderCount;
  lstring videoShaderName;

  void setupVideoShaders();
};

extern MainWindow *mainWindow;
