MainWindow *mainWindow = 0;

MainWindow::MainWindow() {
  setTitle(application->title);
  setGeometry({ 256, 256, 626, 480 });
  setBackgroundColor({ 0, 0, 0 });
  windowManager->append(this, "MainWindow");

  cartridgeMenu.setText("Cartridge");
    cartridgeLoadSNES.setText("Load SNES Cartridge ...");
    cartridgeLoadNES.setText("Load NES Cartridge ...");
    cartridgeLoadGameBoy.setText("Load Game Boy Cartridge ...");
    cartridgeLoadSatellaviewSlotted.setText("Load Satellaview-Slotted Cartridge ...");
    cartridgeLoadSatellaview.setText("Load Satellaview Cartridge ...");
    cartridgeLoadSufamiTurbo.setText("Load Sufami Turbo Cartridge ...");
    cartridgeLoadSuperGameBoy.setText("Load Super Game Boy Cartridge ...");

  nesMenu.setText("NES");
    nesPower.setText("Power Cycle");
    nesReset.setText("Reset");
    nesPort1.setText("Controller Port 1");
      nesPort1Device[0].setText("None");
      nesPort1Device[1].setText("Gamepad");
      RadioItem::group(nesPort1Device[0], nesPort1Device[1]);
      nesPort1Device[config->nes.controllerPort1Device].setChecked();
    nesPort2.setText("Controller Port 2");
      nesPort2Device[0].setText("None");
      nesPort2Device[1].setText("Gamepad");
      RadioItem::group(nesPort2Device[0], nesPort2Device[1]);
      nesPort2Device[config->nes.controllerPort2Device].setChecked();
    nesCartridgeUnload.setText("Unload Cartridge");

  snesMenu.setText("SNES");
    snesPower.setText("Power Cycle");
    snesReset.setText("Reset");
    snesPort1.setText("Controller Port 1");
      snesPort1Device[0].setText("None");
      snesPort1Device[1].setText("Gamepad");
      snesPort1Device[2].setText("Multitap");
      snesPort1Device[3].setText("Mouse");
      RadioItem::group(snesPort1Device[0], snesPort1Device[1], snesPort1Device[2], snesPort1Device[3]);
      snesPort1Device[config->snes.controllerPort1Device].setChecked();
    snesPort2.setText("Controller Port 2");
      snesPort2Device[0].setText("None");
      snesPort2Device[1].setText("Gamepad");
      snesPort2Device[2].setText("Multitap");
      snesPort2Device[3].setText("Mouse");
      snesPort2Device[4].setText("Super Scope");
      snesPort2Device[5].setText("Justifier");
      snesPort2Device[6].setText("Dual Justifiers");
      snesPort2Device[7].setText("Serial Cable");
      RadioItem::group(snesPort2Device[0], snesPort2Device[1], snesPort2Device[2], snesPort2Device[3],
                       snesPort2Device[4], snesPort2Device[5], snesPort2Device[6], snesPort2Device[7]);
      snesPort2Device[config->snes.controllerPort2Device].setChecked();
    snesCartridgeUnload.setText("Unload Cartridge");

  gameBoyMenu.setText("Game Boy");
    gameBoyPower.setText("Power Cycle");
    gameBoyCartridgeUnload.setText("Unload Cartridge");

  settingsMenu.setText("Settings");
    settingsVideoFilter.setText("Video Filter");
      settingsVideoFilterNone.setText("None");
      setupVideoFilters();
    settingsVideoShader.setText("Video Shader");
      settingsVideoShaderNone.setText("None");
      settingsVideoShaderBlur.setText("Blur");
      setupVideoShaders();
    settingsSynchronizeVideo.setText("Synchronize Video");
    settingsSynchronizeVideo.setChecked(config->video.synchronize);
    settingsSynchronizeAudio.setText("Synchronize Audio");
    settingsSynchronizeAudio.setChecked(config->audio.synchronize);
    settingsCorrectAspectRatio.setText("Correct Aspect Ratio");
    settingsCorrectAspectRatio.setChecked(config->video.correctAspectRatio);
    settingsMaskOverscan.setText("Mask Overscan");
    settingsMaskOverscan.setChecked(config->video.maskOverscan);
    settingsMuteAudio.setText("Mute Audio");
    settingsMuteAudio.setChecked(config->audio.mute);
    settingsConfiguration.setText("Configuration ...");

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
    toolsStateManager.setText("State Manager ...");

  append(cartridgeMenu);
    cartridgeMenu.append(cartridgeLoadNES);
    cartridgeMenu.append(cartridgeLoadSNES);
    cartridgeMenu.append(cartridgeLoadGameBoy);
    cartridgeMenu.append(cartridgeSeparator);
    cartridgeMenu.append(cartridgeLoadSatellaviewSlotted);
    cartridgeMenu.append(cartridgeLoadSatellaview);
    cartridgeMenu.append(cartridgeLoadSufamiTurbo);
    cartridgeMenu.append(cartridgeLoadSuperGameBoy);

  append(nesMenu);
    nesMenu.append(nesPower);
    nesMenu.append(nesReset);
    nesMenu.append(nesSeparator1);
    nesMenu.append(nesPort1);
      nesPort1.append(nesPort1Device[0]);
      nesPort1.append(nesPort1Device[1]);
    nesMenu.append(nesPort2);
      nesPort2.append(nesPort2Device[0]);
      nesPort2.append(nesPort2Device[1]);
    nesMenu.append(nesSeparator2);
    nesMenu.append(nesCartridgeUnload);

  append(snesMenu);
    snesMenu.append(snesPower);
    snesMenu.append(snesReset);
    snesMenu.append(snesSeparator1);
    snesMenu.append(snesPort1);
      snesPort1.append(snesPort1Device[0]);
      snesPort1.append(snesPort1Device[1]);
      snesPort1.append(snesPort1Device[2]);
      snesPort1.append(snesPort1Device[3]);
    snesMenu.append(snesPort2);
      snesPort2.append(snesPort2Device[0]);
      snesPort2.append(snesPort2Device[1]);
      snesPort2.append(snesPort2Device[2]);
      snesPort2.append(snesPort2Device[3]);
      snesPort2.append(snesPort2Device[4]);
      snesPort2.append(snesPort2Device[5]);
      snesPort2.append(snesPort2Device[6]);
      snesPort2.append(snesPort2Device[7]);
    snesMenu.append(snesSeparator2);
    snesMenu.append(snesCartridgeUnload);

  append(gameBoyMenu);
    gameBoyMenu.append(gameBoyPower);
    gameBoyMenu.append(gameBoySeparator);
    gameBoyMenu.append(gameBoyCartridgeUnload);

  append(settingsMenu);
    settingsMenu.append(settingsVideoFilter);
      settingsVideoFilter.append(settingsVideoFilterNone);
      if(videoFilterName.size())
      settingsVideoFilter.append(settingsVideoFilterSeparator);
      for(unsigned n = 0; n < videoFilterName.size(); n++)
      settingsVideoFilter.append(settingsVideoFilterList[n]);
    settingsMenu.append(settingsVideoShader);
      settingsVideoShader.append(settingsVideoShaderNone);
      settingsVideoShader.append(settingsVideoShaderBlur);
      if(videoShaderName.size())
      settingsVideoShader.append(settingsVideoShaderSeparator);
      for(unsigned n = 0; n < videoShaderName.size(); n++)
      settingsVideoShader.append(settingsVideoShaderList[n]);
    settingsMenu.append(settingsSeparator1);
    settingsMenu.append(settingsSynchronizeVideo);
    settingsMenu.append(settingsSynchronizeAudio);
    settingsMenu.append(settingsSeparator2);
    settingsMenu.append(settingsCorrectAspectRatio);
    settingsMenu.append(settingsMaskOverscan);
    settingsMenu.append(settingsMuteAudio);
    settingsMenu.append(settingsSeparator3);
    settingsMenu.append(settingsConfiguration);

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
    toolsMenu.append(toolsStateManager);

  setMenuVisible();

  setStatusText("No cartridge loaded");
  setStatusVisible();

  layout.append(viewport, { 0, 0, 512, 480 });
  append(layout);

  onClose = [&] { application->quit = true; };
  onSize = [&] { utility->resizeMainWindow(); };

  cartridgeLoadNES.onTick = [&] {
    fileBrowser->open("Load Cartridge - NES", FileBrowser::Mode::NES, [](string filename) {
      interface->nes.loadCartridge(filename);
    });
  };

  cartridgeLoadSNES.onTick = [&] {
    fileBrowser->open("Load Cartridge - SNES", FileBrowser::Mode::SNES, [](string filename) {
      interface->snes.loadCartridge(filename);
    });
  };

  cartridgeLoadGameBoy.onTick = [&] {
    fileBrowser->open("Load Cartridge - Game Boy", FileBrowser::Mode::GameBoy, [](string filename) {
      interface->gameBoy.loadCartridge(filename);
    });
  };

  cartridgeLoadSatellaviewSlotted.onTick = [&] { slotLoader->loadSatellaviewSlotted(); };
  cartridgeLoadSatellaview.onTick        = [&] { slotLoader->loadSatellaview(); };
  cartridgeLoadSufamiTurbo.onTick        = [&] { slotLoader->loadSufamiTurbo(); };
  cartridgeLoadSuperGameBoy.onTick       = [&] { slotLoader->loadSuperGameBoy(); };

  nesPower.onTick = { &Interface::power, interface };
  nesReset.onTick = { &Interface::reset, interface };

  nesPort1Device[0].onTick = [&] { interface->setController(0, 0); };
  nesPort1Device[1].onTick = [&] { interface->setController(0, 1); };

  nesPort2Device[0].onTick = [&] { interface->setController(1, 0); };
  nesPort2Device[1].onTick = [&] { interface->setController(1, 1); };

  nesCartridgeUnload.onTick = { &Interface::unloadCartridge, interface };

  snesPower.onTick = { &Interface::power, interface };
  snesReset.onTick = { &Interface::reset, interface };

  snesPort1Device[0].onTick = [&] { interface->setController(0, 0); };
  snesPort1Device[1].onTick = [&] { interface->setController(0, 1); };
  snesPort1Device[2].onTick = [&] { interface->setController(0, 2); };
  snesPort1Device[3].onTick = [&] { interface->setController(0, 3); };

  snesPort2Device[0].onTick = [&] { interface->setController(1, 0); };
  snesPort2Device[1].onTick = [&] { interface->setController(1, 1); };
  snesPort2Device[2].onTick = [&] { interface->setController(1, 2); };
  snesPort2Device[3].onTick = [&] { interface->setController(1, 3); };
  snesPort2Device[4].onTick = [&] { interface->setController(1, 4); };
  snesPort2Device[5].onTick = [&] { interface->setController(1, 5); };
  snesPort2Device[6].onTick = [&] { interface->setController(1, 6); };
  snesPort2Device[7].onTick = [&] { interface->setController(1, 7); };

  snesCartridgeUnload.onTick = { &Interface::unloadCartridge, interface };

  gameBoyPower.onTick = { &Interface::power, interface };
  gameBoyCartridgeUnload.onTick = { &Interface::unloadCartridge, interface };

  settingsVideoFilterNone.onTick = [&] {
    config->video.filter = "None";
    utility->bindVideoFilter();
  };

  settingsVideoShaderNone.onTick = [&] {
    config->video.shader = "None";
    utility->bindVideoShader();
  };

  settingsVideoShaderBlur.onTick = [&] {
    config->video.shader = "Blur";
    utility->bindVideoShader();
  };

  settingsSynchronizeVideo.onTick = [&] {
    config->video.synchronize = settingsSynchronizeVideo.checked();
    video.set(Video::Synchronize, config->video.synchronize);
  };

  settingsSynchronizeAudio.onTick = [&] {
    config->audio.synchronize = settingsSynchronizeAudio.checked();
    audio.set(Audio::Synchronize, config->audio.synchronize);
  };

  settingsCorrectAspectRatio.onTick = [&] {
    config->video.correctAspectRatio = settingsCorrectAspectRatio.checked();
    utility->resizeMainWindow();
  };

  settingsMaskOverscan.onTick = [&] {
    config->video.maskOverscan = settingsMaskOverscan.checked();
  };

  settingsMuteAudio.onTick = [&] {
    config->audio.mute = settingsMuteAudio.checked();
    dspaudio.setVolume(config->audio.mute == false ? 1.0 : 0.0);
  };

  settingsConfiguration.onTick = [&] { settingsWindow->setVisible(); };

  toolsStateSave1.onTick = [&] { interface->saveState(1); };
  toolsStateSave2.onTick = [&] { interface->saveState(2); };
  toolsStateSave3.onTick = [&] { interface->saveState(3); };
  toolsStateSave4.onTick = [&] { interface->saveState(4); };
  toolsStateSave5.onTick = [&] { interface->saveState(5); };

  toolsStateLoad1.onTick = [&] { interface->loadState(1); };
  toolsStateLoad2.onTick = [&] { interface->loadState(2); };
  toolsStateLoad3.onTick = [&] { interface->loadState(3); };
  toolsStateLoad4.onTick = [&] { interface->loadState(4); };
  toolsStateLoad5.onTick = [&] { interface->loadState(5); };

  toolsShrinkWindow.onTick = [&] { utility->resizeMainWindow(true); };
  toolsCheatEditor.onTick = [&] { cheatEditor->setVisible(); };
  toolsStateManager.onTick = [&] { stateManager->setVisible(); };

  synchronize();
}

void MainWindow::synchronize() {
  if(interface->cartridgeLoaded()) {
    toolsStateSave.setEnabled(true);
    toolsStateLoad.setEnabled(true);
  } else {
    toolsStateSave.setEnabled(false);
    toolsStateLoad.setEnabled(false);
  }
}

void MainWindow::setupVideoFilters() {
  lstring files = directory::files({ application->basepath, "filters/" }, "*.filter");
  if(files.size() == 0) files = directory::files({ application->userpath, "filters/" }, "*.filter");
  reference_array<RadioItem&> group;

  settingsVideoFilterList = new RadioItem[files.size()];
  for(unsigned n = 0; n < files.size(); n++) {
    string name = files[n];
    videoFilterName.append({ application->userpath, "filters/", name });
    if(auto position = name.position(".filter")) name[position()] = 0;

    settingsVideoFilterList[n].setText(name);
    settingsVideoFilterList[n].onTick = [&, n] {
      config->video.filter = videoFilterName[n];
      utility->bindVideoFilter();
    };
  }

  group.append(settingsVideoFilterNone);
  for(unsigned n = 0; n < files.size(); n++) group.append(settingsVideoFilterList[n]);
  RadioItem::group(group);

  if(config->video.filter == "None") settingsVideoFilterNone.setChecked();
  for(unsigned n = 0; n < files.size(); n++)
  if(config->video.filter == videoFilterName[n]) settingsVideoFilterList[n].setChecked();
}

void MainWindow::setupVideoShaders() {
  lstring files = directory::files({ application->basepath, "shaders/" }, { "*.", config->video.driver, ".shader" });
  if(files.size() == 0) files = directory::files({ application->userpath, "shaders/" }, { "*.", config->video.driver, ".shader" });
  reference_array<RadioItem&> group;

  settingsVideoShaderList = new RadioItem[files.size()];
  for(unsigned n = 0; n < files.size(); n++) {
    string name = files[n];
    videoShaderName.append({ application->userpath, "shaders/", name });
    if(auto position = name.position(string{ ".", config->video.driver, ".shader" })) name[position()] = 0;

    settingsVideoShaderList[n].setText(name);
    settingsVideoShaderList[n].onTick = [&, n] {
      config->video.shader = videoShaderName[n];
      utility->bindVideoShader();
    };
  }

  group.append(settingsVideoShaderNone);
  group.append(settingsVideoShaderBlur);
  for(unsigned n = 0; n < files.size(); n++) group.append(settingsVideoShaderList[n]);
  RadioItem::group(group);

  if(config->video.shader == "None") settingsVideoShaderNone.setChecked();
  if(config->video.shader == "Blur") settingsVideoShaderBlur.setChecked();
  for(unsigned n = 0; n < files.size(); n++)
  if(config->video.shader == videoShaderName[n]) settingsVideoShaderList[n].setChecked();
}
