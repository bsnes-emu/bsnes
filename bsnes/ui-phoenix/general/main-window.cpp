MainWindow mainWindow;

void MainWindow::create() {
  application.windows.append(this);
  Window::create(120, 120, 595, 448, string(SNES::Info::Name, " v", SNES::Info::Version));
  setDefaultFont(application.proportionalFont);
  setFont(application.proportionalFontBold);
  setBackgroundColor(0, 0, 0);

  system.create(*this, "System");
  systemLoadCartridge.create(system, "Load Cartridge ...");
  systemSeparator.create(system);
  systemQuit.create(system, "Quit");
  setMenuVisible(true);
  settings.create(*this, "Settings");
  settingsSynchronizeVideo.create(settings, "Synchronize Video");
  settingsSynchronizeVideo.setChecked(config.video.synchronize);
  settingsSynchronizeAudio.create(settings, "Synchronize Audio");
  settingsSynchronizeAudio.setChecked(config.audio.synchronize);
  settingsMuteAudio.create(settings, "Mute Audio");
  settingsMuteAudio.setChecked(config.audio.mute);
  settingsSeparator.create(settings);
  settingsVideo.create(settings, "Video Settings ...");
  settingsAdvanced.create(settings, "Advanced Settings ...");
  tools.create(*this, "Tools");
  help.create(*this, "Help");

  viewport.create(*this, 0, 0, 595, 448);
  utility.setStatus("");
  setStatusVisible(true);

  systemLoadCartridge.onTick = []() {
    utility.loadCartridgeNormal();
  };

  systemQuit.onTick = []() {
    application.quit = true;
  };

  settingsSynchronizeVideo.onTick = []() {
    config.video.synchronize = mainWindow.settingsSynchronizeVideo.checked();
    video.set(Video::Synchronize, config.video.synchronize);
  };

  settingsSynchronizeAudio.onTick = []() {
    config.audio.synchronize = mainWindow.settingsSynchronizeAudio.checked();
    audio.set(Audio::Synchronize, config.audio.synchronize);
  };

  settingsMuteAudio.onTick = []() {
    config.audio.mute = mainWindow.settingsMuteAudio.checked();
  };

  settingsVideo.onTick = []() {
    videoSettingsWindow.setVisible();
  };

  settingsAdvanced.onTick = []() {
    advancedSettingsWindow.setVisible();
  };

  onClose = []() {
    application.quit = true;
    return false;
  };
}
