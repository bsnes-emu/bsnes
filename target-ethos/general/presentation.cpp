Presentation* presentation = nullptr;

void Presentation::synchronize() {
  for(auto& emulator : emulatorList) emulator->menu.setVisible(false);
  for(auto& emulator : emulatorList) {
    if(emulator->interface == program->active) {
      active = emulator;
      emulator->menu.setVisible(true);
    }
  }

  shaderNone.setChecked();
  if(config->video.shader == "None") shaderNone.setChecked();
  if(config->video.shader == "Blur") shaderBlur.setChecked();
  if(config->video.shader == "Emulation") shaderEmulation.setChecked();
  for(auto& shader : shaderList) {
    string name = notdir(config->video.shader.split<1>(".shader/")(0));
    if(name == shader->text()) shader->setChecked();
  }

  switch(config->video.scaleMode) {
  case 0: centerVideo.setChecked(); break;
  case 1: scaleVideo.setChecked(); break;
  case 2: stretchVideo.setChecked(); break;
  }
  aspectCorrection.setChecked(config->video.aspectCorrection);
  maskOverscan.setChecked(config->video.maskOverscan.enable);
  synchronizeVideo.setChecked(config->video.synchronize);
  synchronizeAudio.setChecked(config->audio.synchronize);
  muteAudio.setChecked(config->audio.mute);

  if(program->active == nullptr) {
    toolsMenu.setVisible(false);
  } else {
    toolsMenu.setVisible(true);
    saveStateMenu.setVisible(system().information.capability.states);
    loadStateMenu.setVisible(system().information.capability.states);
    stateMenuSeparator.setVisible(system().information.capability.states);
    resizeWindow.setVisible(config->video.scaleMode != 2);
    stateManager.setVisible(system().information.capability.states);
    cheatEditor.setVisible(system().information.capability.cheats);
    synchronizeTime.setVisible(system().rtc());
  }
}

void Presentation::setSystemName(string name) {
  if(active) active->menu.setText(systemName = name);
}

Presentation::Presentation() {
  bootstrap();
  loadShaders();
  setGeometry({256, 256, 720, 480});
  windowManager->append(this, "Presentation");

  setTitle({::Emulator::Name, " v", ::Emulator::Version});
  setBackgroundColor({0, 0, 0});
  setMenuVisible();
  setStatusVisible();

  setDroppable();
  viewport.setDroppable();

  loadMenu.setText("Library");
    loadImport.setText("Import Game ...");
  settingsMenu.setText("Settings");
    videoMenu.setText("Video");
      centerVideo.setText("Center");
      scaleVideo.setText("Scale");
      stretchVideo.setText("Stretch");
      RadioItem::group(centerVideo, scaleVideo, stretchVideo);
      aspectCorrection.setText("Aspect Correction");
      maskOverscan.setText("Mask Overscan");
    shaderMenu.setText("Shader");
      shaderNone.setText("None");
      shaderBlur.setText("Blur");
      shaderEmulation.setText("Display Emulation");
    synchronizeVideo.setText("Synchronize Video");
    synchronizeAudio.setText("Synchronize Audio");
    muteAudio.setText("Mute Audio");
    configurationSettings.setText("Configuration ...");
  toolsMenu.setText("Tools");
    saveStateMenu.setText("Save State");
      for(unsigned n = 0; n < 5; n++) saveStateItem[n].setText({"Slot ", 1 + n});
    loadStateMenu.setText("Load State");
      for(unsigned n = 0; n < 5; n++) loadStateItem[n].setText({"Slot ", 1 + n});
    resizeWindow.setText("Resize Window");
    stateManager.setText("State Manager");
    cheatEditor.setText("Cheat Editor");
    synchronizeTime.setText("Synchronize Time");

  append(loadMenu);
    for(auto& item : loadListSystem) loadMenu.append(*item);
    if(program->ananke.open()) loadMenu.append(loadSeparator, loadImport);
  for(auto& systemItem : emulatorList) append(systemItem->menu);
  append(settingsMenu);
    settingsMenu.append(videoMenu);
      videoMenu.append(centerVideo, scaleVideo, stretchVideo, *new Separator, aspectCorrection, maskOverscan);
    settingsMenu.append(shaderMenu);
      shaderMenu.append(shaderNone, shaderBlur);
      if(config->video.driver == "OpenGL") shaderMenu.append(shaderEmulation);
      if(shaderList.size() > 0) {
        shaderMenu.append(*new Separator);
        for(auto& shader : shaderList) shaderMenu.append(*shader);
      }
    settingsMenu.append(*new Separator);
    settingsMenu.append(synchronizeVideo, synchronizeAudio, muteAudio);
    if(Intrinsics::platform() != Intrinsics::Platform::OSX) {
      settingsMenu.append(*new Separator);
      settingsMenu.append(configurationSettings);
    }
  append(toolsMenu);
    toolsMenu.append(saveStateMenu);
      for(unsigned n = 0; n < 5; n++) saveStateMenu.append(saveStateItem[n]);
    toolsMenu.append(loadStateMenu);
      for(unsigned n = 0; n < 5; n++) loadStateMenu.append(loadStateItem[n]);
    toolsMenu.append(stateMenuSeparator);
    toolsMenu.append(resizeWindow, stateManager, cheatEditor, synchronizeTime);

  append(layout);
  layout.append(viewport, {0, 0, 1, 1});

  onDrop = viewport.onDrop = [&](lstring paths) {
    if(paths.size() && directory::exists(paths[0])) {
      utility->loadMedia(paths[0]);
      setFocused();
    }
  };

  onSize = [&] {
    utility->resize();
  };

  onClose = [&] {
    setVisible(false);
    if(Intrinsics::platform() == Intrinsics::Platform::OSX) {
      utility->unload();
    } else {
      Application::quit();
    }
  };

  loadImport.onActivate = [&] {
    if(program->ananke.open() == false) return;
    function<string ()> browse = program->ananke.sym("ananke_browse");
    if(!browse) return;
    string pathname = browse();
    if(pathname.empty()) return;
    utility->loadMedia(pathname);
  };

  shaderNone.onActivate = [&] { config->video.shader = "None"; utility->updateShader(); };
  shaderBlur.onActivate = [&] { config->video.shader = "Blur"; utility->updateShader(); };
  shaderEmulation.onActivate = [&] { config->video.shader = "Emulation"; utility->updateShader(); };
  centerVideo.onActivate  = [&] { config->video.scaleMode = 0; utility->resize(); };
  scaleVideo.onActivate   = [&] { config->video.scaleMode = 1; utility->resize(); };
  stretchVideo.onActivate = [&] { config->video.scaleMode = 2; utility->resize(); };
  aspectCorrection.onToggle = [&] { config->video.aspectCorrection = aspectCorrection.checked(); utility->resize(); };
  maskOverscan.onToggle = [&] { config->video.maskOverscan.enable = maskOverscan.checked(); };
  synchronizeVideo.onToggle = [&] { config->video.synchronize = synchronizeVideo.checked(); utility->synchronizeRuby(); };
  synchronizeAudio.onToggle = [&] { config->audio.synchronize = synchronizeAudio.checked(); utility->synchronizeRuby(); };
  muteAudio.onToggle = [&] { config->audio.mute = muteAudio.checked(); utility->synchronizeRuby(); };
  configurationSettings.onActivate = [&] { settings->setVisible(); settings->panelList.setFocused(); };
  for(unsigned n = 0; n < 5; n++) saveStateItem[n].onActivate = [=] { utility->saveState(1 + n); };
  for(unsigned n = 0; n < 5; n++) loadStateItem[n].onActivate = [=] { utility->loadState(1 + n); };
  resizeWindow.onActivate = [&] { utility->resize(true); };
  stateManager.onActivate = [&] { ::stateManager->setVisible(); };
  cheatEditor.onActivate = [&] { ::cheatEditor->setVisible(); };
  synchronizeTime.onActivate = [&] { system().rtcsync(); };

  synchronize();
}

void Presentation::bootstrap() {
  for(auto& emulator : program->emulator) {
    auto iEmulator = new Emulator;
    iEmulator->interface = emulator;

    for(auto& media : emulator->media) {
      if(media.bootable == false) continue;
      auto item = new Item;
      item->onActivate = [=, &media] {
        utility->loadMedia(iEmulator->interface, media);
      };
      item->setText({media.name, " ..."});
      loadListSystem.append(item);
    }

    iEmulator->menu.setText(emulator->information.name);
    iEmulator->power.setText("Power");
    iEmulator->reset.setText("Reset");
    iEmulator->unload.setText("Unload");

    for(auto& port : emulator->port) {
      auto iPort = new Emulator::Port;
      iPort->menu.setText(port.name);
      iEmulator->port.append(iPort);

      for(auto& device : port.device) {
        auto iDevice = new RadioItem;
        iDevice->setText(device.name);
        iDevice->onActivate = [=] { utility->connect(port.id, device.id); };
        iPort->group.append(*iDevice);
        iPort->device.append(iDevice);
      }

      RadioItem::group(iPort->group);
    }

    iEmulator->menu.append(iEmulator->power);
    if(emulator->information.resettable)
    iEmulator->menu.append(iEmulator->reset);
    iEmulator->menu.append(*new Separator);
    unsigned visiblePorts = 0;
    for(auto& iPort : iEmulator->port) {
      iEmulator->menu.append(iPort->menu);
      if(iPort->device.size() <= 1) iPort->menu.setVisible(false);
      else visiblePorts++;
      for(auto& iDevice : iPort->device) {
        iPort->menu.append(*iDevice);
      }
    }
    iEmulator->menu.append(iEmulator->controllerSeparator);
    if(visiblePorts == 0) iEmulator->controllerSeparator.setVisible(false);
    iEmulator->menu.append(iEmulator->unload);

    iEmulator->power.onActivate = {&Utility::power, utility};
    iEmulator->reset.onActivate = {&Utility::reset, utility};
    iEmulator->unload.onActivate = {&Utility::unload, utility};

    emulatorList.append(iEmulator);
  }
}

void Presentation::loadShaders() {
  //only the OpenGL driver has video shader support
  if(config->video.driver == "OpenGL") {
    string pathname = program->path("Video Shaders/");
    lstring shaders = directory::folders(pathname, "*.shader");
    for(auto& name : shaders) {
      auto shader = new RadioItem;
      shader->setText(name.split<1>(".shader/")(0));
      shader->onActivate = [=] {
        config->video.shader = {pathname, name};
        utility->updateShader();
      };
      shaderList.append(shader);
    }
  }

  nall::group<RadioItem> group;
  group.append(shaderNone);
  group.append(shaderBlur);
  group.append(shaderEmulation);
  for(auto& shader : shaderList) group.append(*shader);
  RadioItem::group(group);
}
