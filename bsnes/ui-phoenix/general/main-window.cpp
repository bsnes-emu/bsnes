MainWindow mainWindow;

void MainWindow::create() {
  application.windows.append(this);
  Window::create(128, 128, 595, 448, string(SNES::Info::Name, " v", SNES::Info::Version));
  setDefaultFont(application.proportionalFont);
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
  systemPort1.setEnabled(false);
  systemPort2.create(system, "Controller Port 2");
  systemPort2.setEnabled(false);

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
  toolsSeparator.create(tools);
  toolsCheatEditor.create(tools, "Cheat Editor ...");
  toolsStateManager.create(tools, "State Manager ...");

  help.create(*this, "Help");
  helpAbout.create(help, "About ...");

  viewport.create(*this, 0, 0, 595, 448);
  utility.setStatus("");
  setMenuVisible(true);
  setStatusVisible(true);

  systemLoadCartridge.onTick = []() { utility.loadCartridgeNormal(); };
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

  toolsCheatEditor.onTick = []() { cheatEditor.setVisible(); };
  toolsStateManager.onTick = []() { stateManager.setVisible(); };

  helpAbout.onTick = []() {
    MessageWindow::information(mainWindow, string(
      "bsnes\n\n",
      "Version: ", SNES::Info::Version, "\n",
      "Profile: ", SNES::Info::Profile, "\n",
      "Author: byuu\n",
      "Homepage: http://byuu.org/"
    ));
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
  } else {
    systemPower.setEnabled(true);
    systemReset.setEnabled(true);
    toolsStateSave.setEnabled(true);
    toolsStateLoad.setEnabled(true);
  }
}
