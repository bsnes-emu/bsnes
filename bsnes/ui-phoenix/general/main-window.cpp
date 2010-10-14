MainWindow mainWindow;

void MainWindow::create() {
  Window::create(0, 0, 595, 448, { SNES::Info::Name, " v", SNES::Info::Version });
  application.addWindow(this, "MainWindow", "128,128");
  setFont(application.proportionalFontBold);
  setBackgroundColor(0, 0, 0);

  system.create(*this, "System");
  systemLoadCartridge.create(system, "Load Cartridge ...");
  systemLoadCartridgeSpecial.create(system, "Load Special");
  systemLoadCartridgeBsxSlotted.create(systemLoadCartridgeSpecial, "Load BS-X Slotted Cartridge ...");
  systemLoadCartridgeBsx.create(systemLoadCartridgeSpecial, "Load BS-X Cartridge ...");
  systemLoadCartridgeSufamiTurbo.create(systemLoadCartridgeSpecial, "Load Sufami Turbo Cartridge ...");
  systemLoadCartridgeSuperGameBoy.create(systemLoadCartridgeSpecial, "Load Super Game Boy Cartridge ...");
  systemLoadCartridgeSuperGameBoy.setEnabled(SNES::supergameboy.opened());
  systemSeparator1.create(system);
  systemPower.create(system, "Power Cycle");
  systemReset.create(system, "Reset");
  systemSeparator2.create(system);
  systemPort1.create(system, "Controller Port 1");
  systemPort1None.create(systemPort1, "None");
  systemPort1Gamepad.create(systemPort1None, "Gamepad");
  systemPort1Multitap.create(systemPort1None, "Multitap");
  systemPort1Mouse.create(systemPort1None, "Mouse");
  if(config.controller.port1 == 0) systemPort1None.setChecked();
  if(config.controller.port1 == 1) systemPort1Gamepad.setChecked();
  if(config.controller.port1 == 2) systemPort1Multitap.setChecked();
  if(config.controller.port1 == 3) systemPort1Mouse.setChecked();
  systemPort2.create(system, "Controller Port 2");
  systemPort2None.create(systemPort2, "None");
  systemPort2Gamepad.create(systemPort2None, "Gamepad");
  systemPort2Multitap.create(systemPort2None, "Multitap");
  systemPort2Mouse.create(systemPort2None, "Mouse");
  systemPort2SuperScope.create(systemPort2None, "Super Scope");
  systemPort2Justifier.create(systemPort2None, "Justifier");
  systemPort2Justifiers.create(systemPort2None, "Justifiers");
  if(config.controller.port2 == 0) systemPort2None.setChecked();
  if(config.controller.port2 == 1) systemPort2Gamepad.setChecked();
  if(config.controller.port2 == 2) systemPort2Multitap.setChecked();
  if(config.controller.port2 == 3) systemPort2Mouse.setChecked();
  if(config.controller.port2 == 4) systemPort2SuperScope.setChecked();
  if(config.controller.port2 == 5) systemPort2Justifier.setChecked();
  if(config.controller.port2 == 6) systemPort2Justifiers.setChecked();

  settings.create(*this, "Settings");
  settingsVideoMode.create(settings, "Video Mode");
  settingsVideoMode1x.create(settingsVideoMode, "Scale 1x");
  settingsVideoMode2x.create(settingsVideoMode1x, "Scale 2x");
  settingsVideoMode3x.create(settingsVideoMode1x, "Scale 3x");
  settingsVideoMode4x.create(settingsVideoMode1x, "Scale 4x");
  settingsVideoMode5x.create(settingsVideoMode1x, "Scale 5x");
  if(config.video.scale == 1) settingsVideoMode1x.setChecked();
  if(config.video.scale == 2) settingsVideoMode2x.setChecked();
  if(config.video.scale == 3) settingsVideoMode3x.setChecked();
  if(config.video.scale == 4) settingsVideoMode4x.setChecked();
  if(config.video.scale == 5) settingsVideoMode5x.setChecked();
  settingsVideoModeSeparator1.create(settingsVideoMode);
  settingsVideoModeAspectRatioCorrection.create(settingsVideoMode, "Correct Aspect Ratio");
  settingsVideoModeAspectRatioCorrection.setChecked(config.video.aspectRatioCorrection);
  settingsVideoModeSeparator2.create(settingsVideoMode);
  settingsVideoModeNTSC.create(settingsVideoMode, "NTSC");
  settingsVideoModePAL.create(settingsVideoModeNTSC, "PAL");
  if(config.video.region == 0) settingsVideoModeNTSC.setChecked();
  if(config.video.region == 1) settingsVideoModePAL.setChecked();
  settingsSmoothVideo.create(settings, "Smooth Video");
  settingsSmoothVideo.setChecked(config.video.smooth);
  settingsSeparator1.create(settings);
  settingsSynchronizeVideo.create(settings, "Synchronize Video");
  settingsSynchronizeVideo.setChecked(config.video.synchronize);
  settingsSynchronizeAudio.create(settings, "Synchronize Audio");
  settingsSynchronizeAudio.setChecked(config.audio.synchronize);
  settingsMuteAudio.create(settings, "Mute Audio");
  settingsMuteAudio.setChecked(config.audio.mute);
  settingsSeparator2.create(settings);
  settingsVideo.create(settings, "Video Settings ...");
  settingsAudio.create(settings, "Audio Settings ...");
  settingsInput.create(settings, "Input Settings ...");
  settingsAdvanced.create(settings, "Advanced Settings ...");

  tools.create(*this, "Tools");
  toolsStateSave.create(tools, "Save State");
  toolsStateSave1.create(toolsStateSave, "Slot 1");
  toolsStateSave2.create(toolsStateSave, "Slot 2");
  toolsStateSave3.create(toolsStateSave, "Slot 3");
  toolsStateSave4.create(toolsStateSave, "Slot 4");
  toolsStateSave5.create(toolsStateSave, "Slot 5");
  toolsStateLoad.create(tools, "Load State");
  toolsStateLoad1.create(toolsStateLoad, "Slot 1");
  toolsStateLoad2.create(toolsStateLoad, "Slot 2");
  toolsStateLoad3.create(toolsStateLoad, "Slot 3");
  toolsStateLoad4.create(toolsStateLoad, "Slot 4");
  toolsStateLoad5.create(toolsStateLoad, "Slot 5");
  toolsSeparator1.create(tools);
  toolsCaptureMouse.create(tools, "Capture Mouse");
  toolsSeparator2.create(tools);
  toolsCheatEditor.create(tools, "Cheat Editor ...");
  toolsStateManager.create(tools, "State Manager ...");

  help.create(*this, "Help");
  helpAbout.create(help, "About ...");

  viewport.create(*this, 0, 0, 595, 448);
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

  toolsCaptureMouse.onTick = []() { input.acquire(); };

  toolsCheatEditor.onTick = []() { cheatEditor.setVisible(); };
  toolsStateManager.onTick = []() { stateManager.setVisible(); };

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
    return false;
  };

  synchronize();
}

void MainWindow::synchronize() {
  if(SNES::cartridge.loaded() == false) {
    systemPower.setEnabled(false);
    systemReset.setEnabled(false);
    toolsStateSave.setEnabled(false);
    toolsStateLoad.setEnabled(false);
    toolsCaptureMouse.setEnabled(false);
  } else {
    systemPower.setEnabled(true);
    systemReset.setEnabled(true);
    toolsStateSave.setEnabled(true);
    toolsStateLoad.setEnabled(true);
    toolsCaptureMouse.setEnabled(true);
  }
}
