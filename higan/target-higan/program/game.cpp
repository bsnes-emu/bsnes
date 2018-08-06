auto Program::load() -> void {
  if(!gameQueue) return;

  string location = gameQueue.left();
  string extension = Location::suffix(location).trimLeft(".", 1L);

  for(auto& emulator : emulators) {
    auto information = emulator->information();
    if(information.extension == extension) return load(*emulator);
  }

  gameQueue.reset();
}

auto Program::load(Emulator::Interface& interface) -> void {
  unload();

  auto information = interface.information();
  gamePaths.append(locate({"systems/", information.name, ".sys/"}));

  inputManager->bind(emulator = &interface);

  if(auto configuration = string::read({gamePaths[0], "configuration.bml"})) {
    emulator->configure(configuration);
  }
  if(auto configuration = emulator->configuration()) {
    file::write({gamePaths[0], "configuration.bml"}, configuration);
  }

  presentation->updateEmulatorMenu();
  if(!emulator->load()) {
    emulator = nullptr;
    gamePaths.reset();
    return;
  }
  emulator->power();
  emulator->set("Blur Emulation", presentation->blurEmulation.checked());
  emulator->set("Color Emulation", presentation->colorEmulation.checked());
  emulator->set("Scanline Emulation", presentation->scanlineEmulation.checked());
  updateAudioDriver();
  updateAudioEffects();

  presentation->viewportLayout.remove(presentation->iconLayout);
  presentation->resizeViewport();
  presentation->setTitle(emulator->titles().merge(" + "));
  presentation->systemMenu.setText(information.name).setVisible(true);
  presentation->toolsMenu.setVisible(true);
  toolsManager->cheatEditor.loadCheats();
  toolsManager->stateManager.doRefresh();
  toolsManager->manifestViewer.doRefresh();
  toolsManager->gameNotes.loadNotes();
}

auto Program::unload() -> void {
  if(!emulator) return;

  presentation->clearViewport();
  toolsManager->cheatEditor.saveCheats();
  toolsManager->gameNotes.saveNotes();
  emulator->unload();
  emulator = nullptr;
  gamePaths.reset();

  presentation->viewportLayout.append(presentation->iconLayout, Size{0, ~0});
  presentation->resizeViewport();
  presentation->setTitle({"higan v", Emulator::Version});
  presentation->systemMenu.setVisible(false);
  presentation->toolsMenu.setVisible(false);
  toolsManager->setVisible(false);
}
