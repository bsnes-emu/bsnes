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
    toolsStateSave.setText("Save State");
      toolsStateSave1.setText("Slot 1");
      toolsStateSave2.setText("Slot 2");
      toolsStateSave3.setText("Slot 3");
      toolsStateSave4.setText("Slot 4");
      toolsStateSave5.setText("Slot 5");
    toolsStateLoad.setText("Load State");
      toolsStateLoad1.setText("Slot 1");
      toolsStateLoad2.setText("Slot 2");
      toolsStateLoad3.setText("Slot 3");
      toolsStateLoad4.setText("Slot 4");
      toolsStateLoad5.setText("Slot 5");
    toolsShrinkWindow.setText("Shrink Window");
    toolsCheatEditor.setText("Cheat Editor ...");
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
    toolsMenu.append(toolsStateSave);
      toolsStateSave.append(toolsStateSave1);
      toolsStateSave.append(toolsStateSave2);
      toolsStateSave.append(toolsStateSave3);
      toolsStateSave.append(toolsStateSave4);
      toolsStateSave.append(toolsStateSave5);
    toolsMenu.append(toolsStateLoad);
      toolsStateLoad.append(toolsStateLoad1);
      toolsStateLoad.append(toolsStateLoad2);
      toolsStateLoad.append(toolsStateLoad3);
      toolsStateLoad.append(toolsStateLoad4);
      toolsStateLoad.append(toolsStateLoad5);
    toolsMenu.append(toolsSeparator);
    toolsMenu.append(toolsShrinkWindow);
    toolsMenu.append(toolsCheatEditor);
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
      interface->loadCartridge(filename);
    });
  };

  cartridgeLoadSNES.onTick = [&] {
    fileBrowser->open("Load SNES Cartridge", { "*.sfc" }, [](string filename) {
      interface->loadCartridge(filename);
    });
  };

  cartridgeLoadGameBoy.onTick = [&] {
    fileBrowser->open("Load Game Boy Cartridge", { "*.gb", "*.gbc" }, [](string filename) {
      interface->loadCartridge(filename);
    });
  };

  nesPower.onTick = { &Interface::power, interface };
  nesReset.onTick = { &Interface::reset, interface };
  nesCartridgeUnload.onTick = { &Interface::unloadCartridge, interface };

  snesPower.onTick = { &Interface::power, interface };
  snesReset.onTick = { &Interface::reset, interface };
  snesCartridgeUnload.onTick = { &Interface::unloadCartridge, interface };

  gameBoyPower.onTick = { &Interface::power, interface };
  gameBoyCartridgeUnload.onTick = { &Interface::unloadCartridge, interface };

  settingsSynchronizeVideo.onTick = [&] {
    video.set(Video::Synchronize, settingsSynchronizeVideo.checked());
  };

  settingsSynchronizeAudio.onTick = [&] {
    audio.set(Audio::Synchronize, settingsSynchronizeAudio.checked());
  };

  settingsMuteAudio.onTick = [&] {
    dspaudio.setVolume(settingsMuteAudio.checked() ? 0.0 : 1.0);
  };

  toolsStateSave1.onTick = [&] { interface->saveState({ interface->baseName, "-1.bst" }); };
  toolsStateSave2.onTick = [&] { interface->saveState({ interface->baseName, "-2.bst" }); };
  toolsStateSave3.onTick = [&] { interface->saveState({ interface->baseName, "-3.bst" }); };
  toolsStateSave4.onTick = [&] { interface->saveState({ interface->baseName, "-4.bst" }); };
  toolsStateSave5.onTick = [&] { interface->saveState({ interface->baseName, "-5.bst" }); };

  toolsStateLoad1.onTick = [&] { interface->loadState({ interface->baseName, "-1.bst" }); };
  toolsStateLoad2.onTick = [&] { interface->loadState({ interface->baseName, "-2.bst" }); };
  toolsStateLoad3.onTick = [&] { interface->loadState({ interface->baseName, "-3.bst" }); };
  toolsStateLoad4.onTick = [&] { interface->loadState({ interface->baseName, "-4.bst" }); };
  toolsStateLoad5.onTick = [&] { interface->loadState({ interface->baseName, "-5.bst" }); };

  toolsShrinkWindow.onTick = [&] { utility->resizeMainWindow(true); };

  toolsCheatEditor.onTick = [&] { cheatEditor->setVisible(); };

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

  synchronize();
}

void MainWindow::synchronize() {
  if(interface->loaded()) {
    toolsStateSave.setEnabled(true);
    toolsStateLoad.setEnabled(true);
  } else {
    toolsStateSave.setEnabled(false);
    toolsStateLoad.setEnabled(false);
  }
}
