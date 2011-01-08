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
  toolsSaveState.create(tools, "Save State");
  toolsSaveState1.create(toolsSaveState, "Slot 1");
  toolsSaveState2.create(toolsSaveState, "Slot 2");
  toolsSaveState3.create(toolsSaveState, "Slot 3");
  toolsSaveState4.create(toolsSaveState, "Slot 4");
  toolsSaveState5.create(toolsSaveState, "Slot 5");
  toolsLoadState.create(tools, "Load State");
  toolsLoadState1.create(toolsLoadState, "Slot 1");
  toolsLoadState2.create(toolsLoadState, "Slot 2");
  toolsLoadState3.create(toolsLoadState, "Slot 3");
  toolsLoadState4.create(toolsLoadState, "Slot 4");
  toolsLoadState5.create(toolsLoadState, "Slot 5");
  toolsSeparator1.create(tools);
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

  toolsSaveState1.onTick = []() { utility.saveState(1); };
  toolsSaveState2.onTick = []() { utility.saveState(2); };
  toolsSaveState3.onTick = []() { utility.saveState(3); };
  toolsSaveState4.onTick = []() { utility.saveState(4); };
  toolsSaveState5.onTick = []() { utility.saveState(5); };

  toolsLoadState1.onTick = []() { utility.loadState(1); };
  toolsLoadState2.onTick = []() { utility.loadState(2); };
  toolsLoadState3.onTick = []() { utility.loadState(3); };
  toolsLoadState4.onTick = []() { utility.loadState(4); };
  toolsLoadState5.onTick = []() { utility.loadState(5); };

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
