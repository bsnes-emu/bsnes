MainWindow mainWindow;

void MainWindow::create() {
  setTitle({ SNES::Info::Name, " v", SNES::Info::Version });
  setResizable(false);
  setGeometry({ 0, 0, 595, 448 });
  application.addWindow(this, "MainWindow", "128,128");
  setMenuFont(application.proportionalFont);
  setStatusFont(application.proportionalFontBold);
  setBackgroundColor(0, 0, 0);

  system.setText("System");

  systemLoadCartridge.setText("Load Cartridge ...");
  system.append(systemLoadCartridge);

  systemLoadCartridgeSpecial.setText("Load Special");
  system.append(systemLoadCartridgeSpecial);

  systemLoadCartridgeBsxSlotted.setText("Load BS-X Slotted Cartridge ...");
  systemLoadCartridgeSpecial.append(systemLoadCartridgeBsxSlotted);

  systemLoadCartridgeBsx.setText("Load BS-X Cartridge ...");
  systemLoadCartridgeSpecial.append(systemLoadCartridgeBsx);

  systemLoadCartridgeSufamiTurbo.setText("Load Sufami Turbo Cartridge ...");
  systemLoadCartridgeSpecial.append(systemLoadCartridgeSufamiTurbo);

  systemLoadCartridgeSuperGameBoy.setText("Load Super Game Boy Cartridge ...");
  systemLoadCartridgeSpecial.append(systemLoadCartridgeSuperGameBoy);

  system.append(systemSeparator1);

  systemPower.setText("Power Cycle");
  system.append(systemPower);

  systemReset.setText("Reset");
  system.append(systemReset);

  system.append(systemSeparator2);

  systemPort1.setText("Controller Port 1");
  system.append(systemPort1);

  systemPort1None.setText("None");
  systemPort1.append(systemPort1None);

  systemPort1Gamepad.setText("Gamepad");
  systemPort1.append(systemPort1Gamepad);

  systemPort1Multitap.setText("Multitap");
  systemPort1.append(systemPort1Multitap);

  systemPort1Mouse.setText("Mouse");
  systemPort1.append(systemPort1Mouse);

  RadioItem::group(
    systemPort1None, systemPort1Gamepad, systemPort1Multitap, systemPort1Mouse
  );

  systemPort2.setText("Controller Port 2");
  system.append(systemPort2);

  systemPort2None.setText("None");
  systemPort2.append(systemPort2None);

  systemPort2Gamepad.setText("Gamepad");
  systemPort2.append(systemPort2Gamepad);

  systemPort2Multitap.setText("Multitap");
  systemPort2.append(systemPort2Multitap);

  systemPort2Mouse.setText("Mouse");
  systemPort2.append(systemPort2Mouse);

  systemPort2SuperScope.setText("Super Scope");
  systemPort2.append(systemPort2SuperScope);

  systemPort2Justifier.setText("Justifier");
  systemPort2.append(systemPort2Justifier);

  systemPort2Justifiers.setText("Justifiers");
  systemPort2.append(systemPort2Justifiers);

  RadioItem::group(
    systemPort2None, systemPort2Gamepad, systemPort2Multitap, systemPort2Mouse,
    systemPort2SuperScope, systemPort2Justifier, systemPort2Justifiers
  );

  append(system);

  settings.setText("Settings");

  settingsVideoMode.setText("Video Mode");
  settings.append(settingsVideoMode);

  settingsVideoMode1x.setText("Scale 1x");
  settingsVideoMode.append(settingsVideoMode1x);

  settingsVideoMode2x.setText("Scale 2x");
  settingsVideoMode.append(settingsVideoMode2x);

  settingsVideoMode3x.setText("Scale 3x");
  settingsVideoMode.append(settingsVideoMode3x);

  settingsVideoMode4x.setText("Scale 4x");
  settingsVideoMode.append(settingsVideoMode4x);

  settingsVideoMode5x.setText("Scale 5x");
  settingsVideoMode.append(settingsVideoMode5x);

  RadioItem::group(
    settingsVideoMode1x, settingsVideoMode2x, settingsVideoMode3x, settingsVideoMode4x, settingsVideoMode5x
  );

  settingsVideoMode.append(settingsVideoModeSeparator1);

  settingsVideoModeAspectRatioCorrection.setText("Correct Aspect Ratio");
  settingsVideoMode.append(settingsVideoModeAspectRatioCorrection);

  settingsVideoMode.append(settingsVideoModeSeparator2);

  settingsVideoModeNTSC.setText("NTSC");
  settingsVideoMode.append(settingsVideoModeNTSC);

  settingsVideoModePAL.setText("PAL");
  settingsVideoMode.append(settingsVideoModePAL);

  settingsVideoFilter.setText("Video Filter");
  settings.append(settingsVideoFilter);

  settingsVideoFilterNone.setText("None");
  settingsVideoFilter.append(settingsVideoFilterNone);

  settingsVideoFilter.append(settingsVideoFilterSeparator);

  settingsVideoShader.setText("Video Shader");
  settings.append(settingsVideoShader);

  settingsVideoShaderNone.setText("None");
  settingsVideoShader.append(settingsVideoShaderNone);

  settingsVideoShader.append(settingsVideoShaderSeparator);

  setupFiltersAndShaders();

  RadioItem::group(
    settingsVideoModeNTSC, settingsVideoModePAL
  );

  settingsSmoothVideo.setText("Smooth Video");
  settings.append(settingsSmoothVideo);

  settings.append(settingsSeparator1);

  settingsSynchronizeVideo.setText("Synchronize Video");
  settings.append(settingsSynchronizeVideo);

  settingsSynchronizeAudio.setText("Synchronize Audio");
  settings.append(settingsSynchronizeAudio);

  settingsMuteAudio.setText("Mute Audio");
  settings.append(settingsMuteAudio);

  settings.append(settingsSeparator2);

  settingsVideo.setText("Video Settings ...");
  settings.append(settingsVideo);

  settingsAudio.setText("Audio Settings ...");
  settings.append(settingsAudio);

  settingsInput.setText("Input Settings ...");
  settings.append(settingsInput);

  settingsAdvanced.setText("Advanced Settings ...");
  settings.append(settingsAdvanced);

  append(settings);

  tools.setText("Tools");

  toolsStateSave.setText("Save State");
  tools.append(toolsStateSave);

  toolsStateSave1.setText("Slot 1");
  toolsStateSave.append(toolsStateSave1);

  toolsStateSave2.setText("Slot 2");
  toolsStateSave.append(toolsStateSave2);

  toolsStateSave3.setText("Slot 3");
  toolsStateSave.append(toolsStateSave3);

  toolsStateSave4.setText("Slot 4");
  toolsStateSave.append(toolsStateSave4);

  toolsStateSave5.setText("Slot 5");
  toolsStateSave.append(toolsStateSave5);

  toolsStateLoad.setText("Load State");
  tools.append(toolsStateLoad);

  toolsStateLoad1.setText("Slot 1");
  toolsStateLoad.append(toolsStateLoad1);

  toolsStateLoad2.setText("Slot 2");
  toolsStateLoad.append(toolsStateLoad2);

  toolsStateLoad3.setText("Slot 3");
  toolsStateLoad.append(toolsStateLoad3);

  toolsStateLoad4.setText("Slot 4");
  toolsStateLoad.append(toolsStateLoad4);

  toolsStateLoad5.setText("Slot 5");
  toolsStateLoad.append(toolsStateLoad5);

  tools.append(toolsSeparator1);

  toolsCheatEditor.setText("Cheat Editor ...");
  tools.append(toolsCheatEditor);

  toolsStateManager.setText("State Manager ...");
  tools.append(toolsStateManager);

  #if defined(DEBUGGER)
  tools.append(toolsSeparator2);

  toolsDebugger.setText("Debugger ...");
  tools.append(toolsDebugger);
  #endif

  append(tools);

  help.setText("Help");

  helpAbout.setText("About ...");
  help.append(helpAbout);

  append(help);

  if(config.controller.port1 == 0) systemPort1None.setChecked();
  if(config.controller.port1 == 1) systemPort1Gamepad.setChecked();
  if(config.controller.port1 == 2) systemPort1Multitap.setChecked();
  if(config.controller.port1 == 3) systemPort1Mouse.setChecked();
  if(config.controller.port2 == 0) systemPort2None.setChecked();
  if(config.controller.port2 == 1) systemPort2Gamepad.setChecked();
  if(config.controller.port2 == 2) systemPort2Multitap.setChecked();
  if(config.controller.port2 == 3) systemPort2Mouse.setChecked();
  if(config.controller.port2 == 4) systemPort2SuperScope.setChecked();
  if(config.controller.port2 == 5) systemPort2Justifier.setChecked();
  if(config.controller.port2 == 6) systemPort2Justifiers.setChecked();

  if(config.video.scale == 1) settingsVideoMode1x.setChecked();
  if(config.video.scale == 2) settingsVideoMode2x.setChecked();
  if(config.video.scale == 3) settingsVideoMode3x.setChecked();
  if(config.video.scale == 4) settingsVideoMode4x.setChecked();
  if(config.video.scale == 5) settingsVideoMode5x.setChecked();
  settingsVideoModeAspectRatioCorrection.setChecked(config.video.aspectRatioCorrection);
  if(config.video.region == 0) settingsVideoModeNTSC.setChecked();
  if(config.video.region == 1) settingsVideoModePAL.setChecked();
  settingsSmoothVideo.setChecked(config.video.smooth);
  settingsSynchronizeVideo.setChecked(config.video.synchronize);
  settingsSynchronizeAudio.setChecked(config.audio.synchronize);
  settingsMuteAudio.setChecked(config.audio.mute);

  layout.append(viewport, { 0, 0, 595, 448 });
  append(layout);

  utility.setStatus("");
  setMenuVisible(true);
  setStatusVisible(true);

  systemLoadCartridge.onTick = []() {
    fileBrowser.fileOpen(FileBrowser::Mode::Cartridge, [](string filename) {
      cartridge.loadNormal(filename);
    });
  };

  systemLoadCartridgeBsxSlotted.onTick = []() { singleSlotLoader.loadCartridgeBsxSlotted(); };
  systemLoadCartridgeBsx.onTick = []() { singleSlotLoader.loadCartridgeBsx(); };
  systemLoadCartridgeSufamiTurbo.onTick = []() { doubleSlotLoader.loadCartridgeSufamiTurbo(); };
  systemLoadCartridgeSuperGameBoy.onTick = []() { singleSlotLoader.loadCartridgeSuperGameBoy(); };

  systemPower.onTick = []() {
    SNES::system.power();
    utility.showMessage("System was power cycled");
  };

  systemReset.onTick = []() {
    SNES::system.reset();
    utility.showMessage("System was reset");
  };

  systemPort1None.onTick       = []() { config.controller.port1 = 0; utility.setControllers(); };
  systemPort1Gamepad.onTick    = []() { config.controller.port1 = 1; utility.setControllers(); };
  systemPort1Multitap.onTick   = []() { config.controller.port1 = 2; utility.setControllers(); };
  systemPort1Mouse.onTick      = []() { config.controller.port1 = 3; utility.setControllers(); };

  systemPort2None.onTick       = []() { config.controller.port2 = 0; utility.setControllers(); };
  systemPort2Gamepad.onTick    = []() { config.controller.port2 = 1; utility.setControllers(); };
  systemPort2Multitap.onTick   = []() { config.controller.port2 = 2; utility.setControllers(); };
  systemPort2Mouse.onTick      = []() { config.controller.port2 = 3; utility.setControllers(); };
  systemPort2SuperScope.onTick = []() { config.controller.port2 = 4; utility.setControllers(); };
  systemPort2Justifier.onTick  = []() { config.controller.port2 = 5; utility.setControllers(); };
  systemPort2Justifiers.onTick = []() { config.controller.port2 = 6; utility.setControllers(); };

  settingsVideoMode1x.onTick = []() { utility.setScale(1); };
  settingsVideoMode2x.onTick = []() { utility.setScale(2); };
  settingsVideoMode3x.onTick = []() { utility.setScale(3); };
  settingsVideoMode4x.onTick = []() { utility.setScale(4); };
  settingsVideoMode5x.onTick = []() { utility.setScale(5); };

  settingsVideoModeAspectRatioCorrection.onTick = []() {
    config.video.aspectRatioCorrection = mainWindow.settingsVideoModeAspectRatioCorrection.checked();
    utility.setScale();
  };

  settingsVideoModeNTSC.onTick = []() { config.video.region = 0; utility.setScale(); };
  settingsVideoModePAL.onTick  = []() { config.video.region = 1; utility.setScale(); };

  settingsVideoFilterNone.onTick = []() {
    config.video.filter = "";
    utility.setFilter();
  };

  settingsVideoShaderNone.onTick = []() {
    config.video.shader = "";
    utility.setShader();
  };

  settingsSmoothVideo.onTick = []() {
    config.video.smooth = mainWindow.settingsSmoothVideo.checked();
    video.set(Video::Filter, (unsigned)config.video.smooth);
  };

  settingsSynchronizeVideo.onTick = []() {
    config.video.synchronize = mainWindow.settingsSynchronizeVideo.checked();
    video.set(Video::Synchronize, config.video.synchronize);
  };

  settingsSynchronizeAudio.onTick = []() {
    config.audio.synchronize = mainWindow.settingsSynchronizeAudio.checked();
    audio.set(Audio::Synchronize, config.audio.synchronize);
  };

  settingsMuteAudio.onTick = []() { config.audio.mute = mainWindow.settingsMuteAudio.checked(); };

  settingsVideo.onTick = []() { videoSettings.setVisible(); };
  settingsAudio.onTick = []() { audioSettings.setVisible(); };
  settingsInput.onTick = []() { inputSettings.setVisible(); };
  settingsAdvanced.onTick = []() { advancedSettings.setVisible(); };

  toolsStateSave1.onTick = []() { utility.saveState(1); };
  toolsStateSave2.onTick = []() { utility.saveState(2); };
  toolsStateSave3.onTick = []() { utility.saveState(3); };
  toolsStateSave4.onTick = []() { utility.saveState(4); };
  toolsStateSave5.onTick = []() { utility.saveState(5); };

  toolsStateLoad1.onTick = []() { utility.loadState(1); };
  toolsStateLoad2.onTick = []() { utility.loadState(2); };
  toolsStateLoad3.onTick = []() { utility.loadState(3); };
  toolsStateLoad4.onTick = []() { utility.loadState(4); };
  toolsStateLoad5.onTick = []() { utility.loadState(5); };

  toolsCheatEditor.onTick = []() { cheatEditor.setVisible(); };
  toolsStateManager.onTick = []() { stateManager.setVisible(); };

  #if defined(DEBUGGER)
  toolsDebugger.onTick = []() { debugger.setVisible(); };
  #endif

  helpAbout.onTick = []() {
    MessageWindow::information(mainWindow, {
      "bsnes\n\n",
      "Version: ", SNES::Info::Version, "\n",
      "Profile: ", SNES::Info::Profile, "\n",
      "Author: byuu\n",
      "Homepage: http://byuu.org/"
    });
  };

  onClose = []() {
    application.quit = true;
  };

  synchronize();
}

void MainWindow::synchronize() {
  if(SNES::cartridge.loaded() == false) {
    systemPower.setEnabled(false);
    systemReset.setEnabled(false);
    toolsStateSave.setEnabled(false);
    toolsStateLoad.setEnabled(false);
  } else {
    systemPower.setEnabled(true);
    systemReset.setEnabled(true);
    toolsStateSave.setEnabled(true);
    toolsStateLoad.setEnabled(true);
  }
}

void MainWindow::setupFiltersAndShaders() {
  string folderPath;
  lstring files;

  folderPath = { path.base, "filters/" };
  files = directory::files(folderPath, "*.filter");
  if(files.size() == 0) {
    folderPath = { path.user, ".config/bsnes/filters/" };
    files = directory::files(folderPath, "*.filter");
  }
  foreach(filename, files) {
    settingsVideoFilterName.append({ folderPath, filename });
  }

  if(settingsVideoFilterName.size() == 0) {
    settingsVideoFilter.setVisible(false);
    config.video.filter = "";  //as the list (and thus the 'None' option) is invisible,
    utility.setFilter();       //erase any previously saved filter name
  } else {
    settingsVideoFilterItem = new Item[settingsVideoFilterName.size()];
    foreach(filename, settingsVideoFilterName, n) {
      settingsVideoFilterItem[n].onTick = [n]() {
        config.video.filter = mainWindow.settingsVideoFilterName[n];
        utility.setFilter();
      };
      settingsVideoFilterItem[n].setText(nall::basename(notdir(filename)));
      settingsVideoFilter.append(settingsVideoFilterItem[n]);
    }
  }

  folderPath = { path.base, "shaders/" };
  files = directory::files(folderPath, { "*.", config.video.driver, ".shader" });
  if(files.size() == 0) {
    folderPath = { path.user, ".config/bsnes/shaders/" };
    files = directory::files(folderPath, { "*.", config.video.driver, ".shader" });
  }
  foreach(filename, files) {
    settingsVideoShaderName.append({ folderPath, filename });
  }

  if(settingsVideoShaderName.size() == 0) {
    settingsVideoShader.setVisible(false);
    config.video.shader = "";
    utility.setShader();
  } else {
    settingsVideoShaderItem = new Item[settingsVideoShaderName.size()];
    foreach(filename, settingsVideoShaderName, n) {
      settingsVideoShaderItem[n].onTick = [n]() {
        config.video.shader = mainWindow.settingsVideoShaderName[n];
        utility.setShader();
      };
      settingsVideoShaderItem[n].setText(nall::basename(nall::basename(notdir(filename))));
      settingsVideoShader.append(settingsVideoShaderItem[n]);
    }
  }
}
