Presentation *presentation = nullptr;

void Presentation::synchronize() {
  for(auto &system : emulatorList) {
    system->menu.setVisible(system->interface == application->active);
  }
}

Presentation::Presentation() {
  bootstrap();

  setTitle("ethos");
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

  for(auto &system : emulatorList) {
    loadMenu.append(system->load);
  }
  append(loadMenu);
  for(auto &system : emulatorList) {
    append(system->menu);
  }
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

    system->name = emulator->information.name;
    system->filter = "*.gba";

    system->load.setText(system->name);
    system->load.onActivate = [=] {
      browser->open(system->interface->media[0], [=](string filename) {
        utility->loadMedia(system->interface, system->interface->media[0], filename);
      });
    };

    system->menu.setText(system->name);
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
