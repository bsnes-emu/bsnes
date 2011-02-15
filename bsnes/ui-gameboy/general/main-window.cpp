MainWindow mainWindow;

void MainWindow::create() {
  setTitle({ GameBoy::Info::Name, " v", GameBoy::Info::Version });
  setResizable(false);
  setMenuFont(application.proportionalFont);
  setStatusFont(application.proportionalFontBold);

  system.setText("System");
  append(system);

  systemLoadCartridge.setText("Load Cartridge ...");
  system.append(systemLoadCartridge);

  system.append(systemSeparator1);

  systemPower.setText("Power Cycle");
  system.append(systemPower);

  settings.setText("Settings");
  append(settings);

  settingsVideoSync.setText("Synchronize Video");
  settingsVideoSync.setChecked(false);
  settings.append(settingsVideoSync);

  settingsAudioSync.setText("Synchronize Audio");
  settingsAudioSync.setChecked(true);
  settings.append(settingsAudioSync);

  tools.setText("Tools");
  append(tools);

  toolsSaveState.setText("Save State");
  tools.append(toolsSaveState);

  toolsSaveState1.setText("Slot 1");
  toolsSaveState.append(toolsSaveState1);

  toolsSaveState2.setText("Slot 2");
  toolsSaveState.append(toolsSaveState2);

  toolsSaveState3.setText("Slot 3");
  toolsSaveState.append(toolsSaveState3);

  toolsSaveState4.setText("Slot 4");
  toolsSaveState.append(toolsSaveState4);

  toolsSaveState5.setText("Slot 5");
  toolsSaveState.append(toolsSaveState5);

  toolsLoadState.setText("Load State");
  tools.append(toolsLoadState);

  toolsLoadState1.setText("Slot 1");
  toolsLoadState.append(toolsLoadState1);

  toolsLoadState2.setText("Slot 2");
  toolsLoadState.append(toolsLoadState2);

  toolsLoadState3.setText("Slot 3");
  toolsLoadState.append(toolsLoadState3);

  toolsLoadState4.setText("Slot 4");
  toolsLoadState.append(toolsLoadState4);

  toolsLoadState5.setText("Slot 5");
  toolsLoadState.append(toolsLoadState5);

  tools.append(toolsSeparator1);

  toolsTraceCPU.setText("Trace CPU");
  tools.append(toolsTraceCPU);

  help.setText("Help");
  append(help);

  helpAbout.setText("About ...");
  help.append(helpAbout);

  layout.append(viewport, { 0, 0, 160 * 2, 144 * 2 });
  setLayout(layout);
  setGeometry({ 128, 128, 160 * 2, 144 * 2 });

  setMenuVisible(true);
  setStatusVisible(true);

  onClose = []() {
    application.quit = true;
  };

  systemLoadCartridge.onTick = []() {
    string filename = OS::fileLoad(mainWindow, "/media/sdb1/root/gameboy_images/", "Game Boy cartridges (*.gb,*.gbc)");
    if(filename != "") utility.loadCartridge(filename);
  };

  systemPower.onTick = []() {
    if(GameBoy::cartridge.loaded()) GameBoy::system.power();
  };

  settingsVideoSync.onTick = []() {
    video.set(Video::Synchronize, mainWindow.settingsVideoSync.checked());
  };

  settingsAudioSync.onTick = []() {
    audio.set(Audio::Synchronize, mainWindow.settingsAudioSync.checked());
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
