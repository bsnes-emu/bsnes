MainWindow *mainWindow = 0;

MainWindow::MainWindow() {
  setTitle(application->title);
  setGeometry({ 256, 256, 512, 480 });
  setBackgroundColor({ 0, 0, 0 });

  cartridgeMenu.setText("Cartridge");
    cartridgeLoadSNES.setText("Load SNES Cartridge ...");
    cartridgeLoadNES.setText("Load NES Cartridge ...");
    cartridgeLoadGameBoy.setText("Load Game Boy Cartridge ...");

  nesMenu.setText("NES");
    nesPower.setText("Power Cycle");
    nesReset.setText("Reset");
    nesCartridgeUnload.setText("Unload Cartridge");

  snesMenu.setText("SNES");
    snesPower.setText("Power Cycle");
    snesReset.setText("Reset");
    snesCartridgeUnload.setText("Unload Cartridge");

  gameBoyMenu.setText("Game Boy");
    gameBoyPower.setText("Power Cycle");
    gameBoyCartridgeUnload.setText("Unload Cartridge");

  settingsMenu.setText("Settings");
    settingsSynchronizeVideo.setText("Synchronize Video");
    settingsSynchronizeVideo.setChecked();
    settingsSynchronizeAudio.setText("Synchronize Audio");
    settingsSynchronizeAudio.setChecked();
    settingsMuteAudio.setText("Mute Audio");

  toolsMenu.setText("Tools");
    toolsShrinkWindow.setText("Shrink Window");
    toolsTest.setText("Test");

  helpMenu.setText("Help");
    helpAbout.setText("About ...");

  append(cartridgeMenu);
    cartridgeMenu.append(cartridgeLoadNES);
    cartridgeMenu.append(cartridgeLoadSNES);
    cartridgeMenu.append(cartridgeLoadGameBoy);

  append(nesMenu);
    nesMenu.append(nesPower);
    nesMenu.append(nesReset);
    nesMenu.append(nesSeparator);
    nesMenu.append(nesCartridgeUnload);

  append(snesMenu);
    snesMenu.append(snesPower);
    snesMenu.append(snesReset);
    snesMenu.append(snesSeparator);
    snesMenu.append(snesCartridgeUnload);

  append(gameBoyMenu);
    gameBoyMenu.append(gameBoyPower);
    gameBoyMenu.append(gameBoySeparator);
    gameBoyMenu.append(gameBoyCartridgeUnload);

  append(settingsMenu);
    settingsMenu.append(settingsSynchronizeVideo);
    settingsMenu.append(settingsSynchronizeAudio);
    settingsMenu.append(settingsMuteAudio);

  append(toolsMenu);
    toolsMenu.append(toolsShrinkWindow);
    toolsMenu.append(toolsTest);

  append(helpMenu);
    helpMenu.append(helpAbout);

  setMenuFont(application->normalFont);
  setMenuVisible();

  setStatusText("No cartridge loaded");
  setStatusFont(application->boldFont);
  setStatusVisible();

  layout.append(viewport, { 0, 0, 512, 480 });
  append(layout);

  onClose = [&] { application->quit = true; };
  onSize = [&] { utility->resizeMainWindow(); };

  cartridgeLoadNES.onTick = [&] {
    fileBrowser->open("Load NES Cartridge", { "*.nes" }, [](string filename) {
      interface->loadCartridgeNES(filename);
    });
  };

  cartridgeLoadSNES.onTick = [&] {
    fileBrowser->open("Load SNES Cartridge", { "*.sfc" }, [](string filename) {
      interface->loadCartridgeSNES(filename);
    });
  };

  cartridgeLoadGameBoy.onTick = [&] {
    fileBrowser->open("Load Game Boy Cartridge", { "*.gb", "*.gbc" }, [](string filename) {
      interface->loadCartridgeGameBoy(filename);
    });
  };

  nesPower.onTick = { &Interface::power, interface };
  nesReset.onTick = { &Interface::reset, interface };
  nesCartridgeUnload.onTick = { &Interface::unloadCartridgeNES, interface };

  snesPower.onTick = { &Interface::power, interface };
  snesReset.onTick = { &Interface::reset, interface };
  snesCartridgeUnload.onTick = { &Interface::unloadCartridgeSNES, interface };

  gameBoyPower.onTick = { &Interface::power, interface };
  gameBoyCartridgeUnload.onTick = { &Interface::unloadCartridgeGameBoy, interface };

  settingsSynchronizeVideo.onTick = [&] {
    video.set(Video::Synchronize, settingsSynchronizeVideo.checked());
  };

  settingsSynchronizeAudio.onTick = [&] {
    audio.set(Audio::Synchronize, settingsSynchronizeAudio.checked());
  };

  settingsMuteAudio.onTick = [&] {
    dspaudio.setVolume(settingsMuteAudio.checked() ? 0.0 : 1.0);
  };

  toolsShrinkWindow.onTick = [&] { utility->resizeMainWindow(true); };

  toolsTest.onTick = [&] {
    NES::cpu.trace = toolsTest.checked();
  };

  helpAbout.onTick = [&] {
    MessageWindow::information(*this, {
      application->title, "\n\n",
      "Author: byuu\n",
      "Website: http://byuu.org/"
    });
  };
}
