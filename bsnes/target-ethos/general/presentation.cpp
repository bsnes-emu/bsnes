Presentation *presentation = nullptr;

void Presentation::synchronize() {
  for(auto &emulator : emulatorList) emulator->menu.setVisible(false);
  for(auto &emulator : emulatorList) {
    if(emulator->interface == application->active) {
      active = emulator;
      emulator->menu.setVisible(true);
    }
  }

  shaderNone.setChecked();
  if(config->video.shader == "None") shaderNone.setChecked();
  if(config->video.shader == "Blur") shaderBlur.setChecked();
  for(auto &shader : shaderList) {
    string name = notdir(nall::basename(config->video.shader));
    if(auto position = name.position(".")) name[position()] = 0;
    if(name == shader->text()) shader->setChecked();
  }

  switch(config->video.scaleMode) {
  case 0: centerVideo.setChecked(); break;
  case 1: scaleVideo.setChecked(); break;
  case 2: stretchVideo.setChecked(); break;
  }
  aspectCorrection.setChecked(config->video.aspectCorrection);
  maskOverscan.setChecked(config->video.maskOverscan);
  synchronizeVideo.setChecked(config->video.synchronize);
  synchronizeAudio.setChecked(config->audio.synchronize);
  muteAudio.setChecked(config->audio.mute);

  if(application->active == nullptr) {
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

void Presentation::setSystemName(const string &name) {
  if(active) active->menu.setText(name);
}

Presentation::Presentation() : active(nullptr) {
  bootstrap();
  loadShaders();
  setGeometry({256, 256, 720, 480});
  windowManager->append(this, "Presentation");

  setTitle({::Emulator::Name, " v", ::Emulator::Version});
  setBackgroundColor({0, 0, 0});
  setMenuVisible();
  setStatusVisible();

  loadMenu.setText("Load");
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
  for(auto &item : loadListSystem) loadMenu.append(*item);
  if(loadListSubsystem.size() > 0) {
    loadMenu.append(*new Separator);
    for(auto &item : loadListSubsystem) loadMenu.append(*item);
  }
  for(auto &systemItem : emulatorList) append(systemItem->menu);
  append(settingsMenu);
    settingsMenu.append(videoMenu);
      videoMenu.append(centerVideo, scaleVideo, stretchVideo, *new Separator, aspectCorrection, maskOverscan);
    settingsMenu.append(shaderMenu);
      shaderMenu.append(shaderNone, shaderBlur);
      if(shaderList.size() > 0) shaderMenu.append(*new Separator);
      for(auto &shader : shaderList) shaderMenu.append(*shader);
    settingsMenu.append(*new Separator);
    settingsMenu.append(synchronizeVideo, synchronizeAudio, muteAudio);
    settingsMenu.append(*new Separator);
    settingsMenu.append(configurationSettings);
  append(toolsMenu);
    toolsMenu.append(saveStateMenu);
      for(unsigned n = 0; n < 5; n++) saveStateMenu.append(saveStateItem[n]);
    toolsMenu.append(loadStateMenu);
      for(unsigned n = 0; n < 5; n++) loadStateMenu.append(loadStateItem[n]);
    toolsMenu.append(stateMenuSeparator);
    toolsMenu.append(resizeWindow, stateManager, cheatEditor, synchronizeTime);

  append(layout);
  layout.append(viewport, {0, 0, 720, 480});

  onSize = [&] { utility->resize(); };
  onClose = [&] { application->quit = true; };

  shaderNone.onActivate = [&] { config->video.shader = "None"; utility->updateShader(); };
  shaderBlur.onActivate = [&] { config->video.shader = "Blur"; utility->updateShader(); };
  centerVideo.onActivate  = [&] { config->video.scaleMode = 0; utility->resize(); };
  scaleVideo.onActivate   = [&] { config->video.scaleMode = 1; utility->resize(); };
  stretchVideo.onActivate = [&] { config->video.scaleMode = 2; utility->resize(); };
  aspectCorrection.onToggle = [&] { config->video.aspectCorrection = aspectCorrection.checked(); utility->resize(); };
  maskOverscan.onToggle = [&] { config->video.maskOverscan = maskOverscan.checked(); };
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
  for(auto &emulator : application->emulator) {
    auto iEmulator = new Emulator;
    iEmulator->interface = emulator;

    for(auto &media : emulator->media) {
      Item *item = new Item;
      item->onActivate = [=, &media] {
        utility->loadMedia(iEmulator->interface, media);
      };
      if(media.load.empty()) {
        item->setText({media.name, " ..."});
        loadListSystem.append(item);
      } else {
        item->setText({nall::basename(media.load), " ..."});
        loadListSubsystem.append(item);
      }
    }

    iEmulator->menu.setText(emulator->information.name);
    iEmulator->power.setText("Power");
    iEmulator->reset.setText("Reset");
    iEmulator->unload.setText("Unload");

    for(auto &port : emulator->port) {
      auto iPort = new Emulator::Port;
      iPort->menu.setText(port.name);
      iEmulator->port.append(iPort);

      for(auto &device : port.device) {
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
    for(auto &iPort : iEmulator->port) {
      iEmulator->menu.append(iPort->menu);
      if(iPort->device.size() <= 1) iPort->menu.setVisible(false);
      else visiblePorts++;
      for(auto &iDevice : iPort->device) {
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
  string pathname = application->path("Video Shaders/");
  lstring files = directory::files(pathname);
  for(auto &filename : files) {
    auto shader = new RadioItem;
    string name = filename;
    if(auto position = name.position(".")) name[position()] = 0;
    shader->setText(name);
    shader->onActivate = [=] {
      config->video.shader = {pathname, filename};
      utility->updateShader();
    };
    shaderList.append(shader);
  }

  set<RadioItem&> group;
  group.append(shaderNone);
  group.append(shaderBlur);
  for(auto &shader : shaderList) group.append(*shader);
  RadioItem::group(group);
}
