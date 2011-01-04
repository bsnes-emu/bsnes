MainWindow mainWindow;

void MainWindow::create() {
  Window::create(128, 128, 160 * 2, 144 * 2, { GameBoy::Info::Name, " v", GameBoy::Info::Version });
  setDefaultFont(application.proportionalFont);
  setFont(application.proportionalFontBold);

  system.create(*this, "System");
  systemLoadCartridge.create(system, "Load Cartridge ...");
  systemSeparator1.create(system);
  systemPower.create(system, "Power Cycle");

  settings.create(*this, "Settings");
  settingsVideoSync.create(settings, "Synchronize Video");
  settingsVideoSync.setChecked(true);

  tools.create(*this, "Tools");
  toolsTraceCPU.create(tools, "Trace CPU");

  help.create(*this, "Help");
  helpAbout.create(help, "About ...");

  viewport.create(*this, 0, 0, 160 * 2, 144 * 2);

  setMenuVisible(true);
  setStatusVisible(true);

  onClose = []() {
    application.quit = true;
    return false;
  };

  systemLoadCartridge.onTick = []() {
    string filename = OS::fileOpen(mainWindow, "Game Boy cartridges\t*.gb,*.gbc", "/media/sdb1/root/gameboy_images/");
    if(filename != "") utility.loadCartridge(filename);
  };

  systemPower.onTick = []() {
    if(GameBoy::cartridge.loaded()) GameBoy::system.power();
  };

  settingsVideoSync.onTick = []() {
    video.set(Video::Synchronize, mainWindow.settingsVideoSync.checked());
  };

  toolsTraceCPU.onTick = []() {
    GameBoy::cpu.trace = mainWindow.toolsTraceCPU.checked();
  };

  helpAbout.onTick = []() {
    MessageWindow::information(mainWindow, {
      "bgameboy\n\n",
      "Version: ", GameBoy::Info::Version, "\n",
      "Author: byuu\n",
      "Homepage: http://byuu.org/"
    });
  };
}
