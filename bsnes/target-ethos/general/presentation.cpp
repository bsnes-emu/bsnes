Presentation *presentation = nullptr;

void Presentation::synchronize() {
  for(auto &system : emulatorList) system->menu.setVisible(false);
  for(auto &system : emulatorList) {
    if(system->interface == application->active) {
      activeSystem = system;
      system->menu.setVisible(true);
      return;
    }
  }
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
    configurationSettings.setText("Configuration ...");
  toolsMenu.setText("Tools");

  append(loadMenu);
  for(auto &item : loadList) loadMenu.append(*item);
  for(auto &system : emulatorList) append(system->menu);
  append(settingsMenu);
    settingsMenu.append(configurationSettings);
  append(toolsMenu);

  append(layout);
  layout.append(viewport, {0, 0, 720, 480});

  onClose = [&] { application->quit = true; };

  configurationSettings.onActivate = [&] { settings->setVisible(); };

  synchronize();
}

void Presentation::bootstrap() {
  for(auto &emulator : application->emulator) {
    System *system = new System;
    system->interface = emulator;

    for(auto &media : emulator->media) {
      Item *item = new Item;
      item->setText({media.displayname, " ..."});
      item->onActivate = [=, &media] {
        browser->open(media, [=, &media](string filename) {
          utility->loadMedia(system->interface, media, filename);
        });
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
