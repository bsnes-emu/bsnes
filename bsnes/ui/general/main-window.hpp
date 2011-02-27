struct MainWindow : TopLevelWindow {
  Menu system;
  Item systemLoadCartridge;
  Menu systemLoadCartridgeSpecial;
  Item systemLoadCartridgeBsxSlotted;
  Item systemLoadCartridgeBsx;
  Item systemLoadCartridgeSufamiTurbo;
  Item systemLoadCartridgeSuperGameBoy;
  Separator systemSeparator1;
  Item systemPower;
  Item systemReset;
  Separator systemSeparator2;
  Menu systemPort1;
  RadioItem systemPort1None;
  RadioItem systemPort1Gamepad;
  RadioItem systemPort1Multitap;
  RadioItem systemPort1Mouse;
  Menu systemPort2;
  RadioItem systemPort2None;
  RadioItem systemPort2Gamepad;
  RadioItem systemPort2Multitap;
  RadioItem systemPort2Mouse;
  RadioItem systemPort2SuperScope;
  RadioItem systemPort2Justifier;
  RadioItem systemPort2Justifiers;

  Menu settings;
  Menu settingsVideoMode;
  RadioItem settingsVideoMode1x;
  RadioItem settingsVideoMode2x;
  RadioItem settingsVideoMode3x;
  RadioItem settingsVideoMode4x;
  RadioItem settingsVideoMode5x;
  Separator settingsVideoModeSeparator1;
  CheckItem settingsVideoModeAspectRatioCorrection;
  Separator settingsVideoModeSeparator2;
  RadioItem settingsVideoModeNTSC;
  RadioItem settingsVideoModePAL;
  CheckItem settingsSmoothVideo;
  Separator settingsSeparator1;
  CheckItem settingsSynchronizeVideo;
  CheckItem settingsSynchronizeAudio;
  CheckItem settingsMuteAudio;
  Separator settingsSeparator2;
  Item settingsVideo;
  Item settingsAudio;
  Item settingsInput;
  Item settingsAdvanced;

  Menu tools;
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
  Item toolsCheatEditor;
  Item toolsStateManager;
  #if defined(DEBUGGER)
  Separator toolsSeparator2;
  Item toolsDebugger;
  #endif

  Menu help;
  Item helpAbout;

  FixedLayout layout;
  Viewport viewport;

  void create();
  void synchronize();
};

extern MainWindow mainWindow;
