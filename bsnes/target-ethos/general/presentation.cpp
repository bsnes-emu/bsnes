Presentation *presentation = nullptr;

void Presentation::synchronize() {
  for(auto &system : emulatorList) system->menu.setVisible(false);
  for(auto &system : emulatorList) {
    if(system->interface == application->active) {
      activeSystem = system;
      system->menu.setVisible(true);
    }
  }

  switch(config->video.scaleMode) {
  case 0: centerVideo.setChecked(); break;
  case 1: scaleVideo.setChecked(); break;
  case 2: stretchVideo.setChecked(); break;
  }
  aspectCorrection.setChecked(config->video.aspectCorrection);
  resizeWindow.setVisible(application->active && config->video.scaleMode != 2);
}

void Presentation::setSystemName(const string &name) {
  if(activeSystem) activeSystem->menu.setText(name);
}

Presentation::Presentation() : activeSystem(nullptr) {
  bootstrap();

  setTitle({Emulator::Name, " v", Emulator::Version});
  setGeometry({1024, 600, 720, 480});
  setBackgroundColor({0, 0, 0});
  setMenuFont(application->normalFont);
  setMenuVisible();
  setStatusFont(application->boldFont);
  setStatusVisible();

  loadMenu.setText("Load");
  settingsMenu.setText("Settings");
    videoMenu.setText("Video");
      centerVideo.setText("Center");
      scaleVideo.setText("Scale");
      stretchVideo.setText("Stretch");
      RadioItem::group(centerVideo, scaleVideo, stretchVideo);
      aspectCorrection.setText("Correct Aspect Ratio");
    configurationSettings.setText("Configuration ...");
  toolsMenu.setText("Tools");
    resizeWindow.setText("Resize Window");

  append(loadMenu);
  for(auto &item : loadList) loadMenu.append(*item);
  for(auto &system : emulatorList) append(system->menu);
  append(settingsMenu);
    settingsMenu.append(videoMenu);
      videoMenu.append(centerVideo, scaleVideo, stretchVideo, *new Separator, aspectCorrection);
    settingsMenu.append(*new Separator);
    settingsMenu.append(configurationSettings);
  append(toolsMenu);
    toolsMenu.append(resizeWindow);

  append(layout);
  layout.append(viewport, {0, 0, 720, 480});

  onSize = [&] { utility->resize(); };
  onClose = [&] { application->quit = true; };

  centerVideo.onActivate  = [&] { config->video.scaleMode = 0; utility->resize(); };
  scaleVideo.onActivate   = [&] { config->video.scaleMode = 1; utility->resize(); };
  stretchVideo.onActivate = [&] { config->video.scaleMode = 2; utility->resize(); };
  aspectCorrection.onToggle = [&] { config->video.aspectCorrection = aspectCorrection.checked(); utility->resize(); };
  configurationSettings.onActivate = [&] { settings->setVisible(); };
  resizeWindow.onActivate = [&] { utility->resize(true); };

  synchronize();
}

void Presentation::bootstrap() {
  for(auto &emulator : application->emulator) {
    System *system = new System;
    system->interface = emulator;

    for(auto &schema : emulator->schema) {
      Item *item = new Item;
      item->setText({schema.displayname, " ..."});
      item->onActivate = [=, &schema] {
        utility->loadSchema(system->interface, schema);
      };
      loadList.append(item);
    }

    system->menu.setText(emulator->information.name);
    system->power.setText("Power");
    system->reset.setText("Reset");
    system->unload.setText("Unload");

    system->menu.append(system->power);
    if(emulator->information.resettable)
    system->menu.append(system->reset);
    system->menu.append(system->separator);
    system->menu.append(system->unload);

    system->power.onActivate = {&Utility::power, utility};
    system->reset.onActivate = {&Utility::reset, utility};
    system->unload.onActivate = {&Utility::unload, utility};

    emulatorList.append(system);
  }
}
