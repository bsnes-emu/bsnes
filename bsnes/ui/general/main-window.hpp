struct MainWindow : Window {
  FixedLayout layout;
  Viewport viewport;

  Menu cartridgeMenu;
    Item cartridgeLoadSNES;
    Item cartridgeLoadNES;
    Item cartridgeLoadGameBoy;
    Item cartridgeLoadGameBoyColor;
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
      RadioItem snesPort1Device[5];
    Menu snesPort2;
      RadioItem snesPort2Device[8];
    Separator snesSeparator2;
    Item snesCartridgeUnload;

  Menu gameBoyMenu;
    Item gameBoyPower;
    Separator gameBoySeparator;
    Item gameBoyCartridgeUnload;

  Menu settingsMenu;
    Menu settingsVideoFilter;
      RadioItem settingsVideoFilterNone;
      Separator settingsVideoFilterSeparator;
      RadioItem *settingsVideoFilterList;
    Menu settingsVideoShader;
      RadioItem settingsVideoShaderNone;
      RadioItem settingsVideoShaderBlur;
      Separator settingsVideoShaderSeparator;
      RadioItem *settingsVideoShaderList;
    Separator settingsSeparator1;
    CheckItem settingsSynchronizeVideo;
    CheckItem settingsSynchronizeAudio;
    Separator settingsSeparator2;
    CheckItem settingsCorrectAspectRatio;
    CheckItem settingsMaskOverscan;
    CheckItem settingsMuteAudio;
    Separator settingsSeparator3;
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

  void synchronize();
  MainWindow();

private:
  lstring videoFilterName;
  lstring videoShaderName;

  void setupVideoFilters();
  void setupVideoShaders();
};

extern MainWindow *mainWindow;
