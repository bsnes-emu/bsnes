MainWindow mainWindow;

void MainWindow::create() {
  statusFont.create("Sans", 8, Font::Style::Bold);
  Window::create(120, 120, 595, 448, string(SNES::Info::Name, " v", SNES::Info::Version));
  setDefaultFont(application.font);
  setFont(statusFont);
  setBackgroundColor(0, 0, 0);

  system.create(*this, "System");
  systemLoadCartridge.create(system, "Load Cartridge ...");
  systemSeparator.create(system);
  systemQuit.create(system, "Quit");
  setMenuVisible(true);
  settings.create(*this, "Settings");
  settingsVideo.create(settings, "Video Settings ...");
  tools.create(*this, "Tools");
  help.create(*this, "Help");

  viewport.create(*this, 0, 0, 595, 448);
  setStatusVisible(true);

  systemLoadCartridge.onTick = []() {
    utility.loadCartridgeNormal();
  };

  systemQuit.onTick = []() {
    application.quit = true;
  };

  settingsVideo.onTick = []() {
    videoSettingsWindow.setVisible();
  };

  onClose = []() {
    application.quit = true;
    return false;
  };
}
