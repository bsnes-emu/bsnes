MainWindow mainWindow;

void MainWindow::create() {
  application.windows.append(this);
  Window::create(128, 128, 595, 448, string(SNES::Info::Name, " v", SNES::Info::Version));
  setDefaultFont(application.proportionalFont);
  setFont(application.proportionalFontBold);
  setBackgroundColor(0, 0, 0);

  system.create(*this, "System");
  systemLoadCartridge.create(system, "Load Cartridge ...");
  systemSeparator.create(system);
  systemQuit.create(system, "Quit");

  settings.create(*this, "Settings");
  settingsSynchronizeVideo.create(settings, "Synchronize Video");
  settingsSynchronizeVideo.setChecked(config.video.synchronize);
  settingsSynchronizeAudio.create(settings, "Synchronize Audio");
  settingsSynchronizeAudio.setChecked(config.audio.synchronize);
  settingsMuteAudio.create(settings, "Mute Audio");
  settingsMuteAudio.setChecked(config.audio.mute);
  settingsSeparator.create(settings);
  settingsVideo.create(settings, "Video Settings ...");
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

  help.create(*this, "Help");
  helpAbout.create(help, "About ...");

  viewport.create(*this, 0, 0, 595, 448);
  utility.setStatus("");
  setMenuVisible(true);
  setStatusVisible(true);

  systemLoadCartridge.onTick = []() {
    utility.loadCartridgeNormal();
  };

  systemQuit.onTick = []() { application.quit = true; };

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
}
