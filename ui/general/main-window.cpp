MainWindow mainWindow;

void MainWindow::create() {
  Window::create(128, 128, 160 * 3, 144 * 3, { GameBoy::Info::Name, " v", GameBoy::Info::Version });

  system.create(*this, "System");
  systemLoadCartridge.create(system, "Load Cartridge ...");
  systemSeparator1.create(system);
  systemPower.create(system, "Power Cycle");
  systemPower.setEnabled(false);
  systemReset.create(system, "Reset");
  systemReset.setEnabled(false);

  settings.create(*this, "Settings");
  settings.setEnabled(false);

  tools.create(*this, "Tools");
  tools.setEnabled(false);

  help.create(*this, "Help");
  helpAbout.create(help, "About ...");

  viewport.create(*this, 0, 0, 160 * 3, 144 * 3);

  setMenuVisible(true);
  setStatusVisible(true);

  onClose = []() {
    application.quit = true;
    return false;
  };

  systemLoadCartridge.onTick = []() {
    string filename = OS::fileOpen(mainWindow, "Game Boy cartridges\t*.gb", "/media/sdb1/root/gameboy_images/");
    if(filename != "") utility.loadCartridge(filename);
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
