auto Program::loadMedium() -> void {
  if(!mediumQueue) return;

  string location = mediumQueue.left();
  string type = Location::suffix(location).trimLeft(".", 1L);

  for(auto& emulator : emulators) {
    for(auto& medium : emulator->media) {
      if(medium.type != type) continue;
      return loadMedium(*emulator, medium);
    }
  }

  mediumQueue.reset();
}

auto Program::loadMedium(Emulator::Interface& interface, const Emulator::Interface::Medium& medium) -> void {
  unloadMedium();

  mediumPaths.append(locate({medium.name, ".sys/"}));

  Emulator::audio.reset(2, audio->get(Audio::Frequency).get<uint>(44100));
  inputManager->bind(emulator = &interface);
  connectDevices();
  if(!emulator->load(medium.id)) {
    emulator = nullptr;
    mediumPaths.reset();
    return;
  }
  updateAudioDriver();
  updateAudioEffects();
  presentation->draw();
  emulator->power();

  presentation->resizeViewport();
  presentation->setTitle(emulator->title());
  presentation->systemMenu.setText(medium.name).setVisible(true);
  presentation->toolsMenu.setVisible(true);
  presentation->updateEmulator();
  toolsManager->cheatEditor.loadCheats();
  toolsManager->stateManager.doRefresh();
  toolsManager->manifestViewer.doRefresh();
}

auto Program::unloadMedium() -> void {
  if(!emulator) return;

  presentation->draw();
  toolsManager->cheatEditor.saveCheats();
  emulator->unload();
  emulator = nullptr;
  mediumPaths.reset();

  presentation->resizeViewport();
  presentation->draw(Resource::Logo::higan);
  presentation->setTitle({"higan v", Emulator::Version});
  presentation->systemMenu.setVisible(false);
  presentation->toolsMenu.setVisible(false);
  toolsManager->setVisible(false);
}
