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

  mediumPaths.append(locate({"systems/", medium.name, ".sys/"}));

  Emulator::audio.reset(2, audio->frequency());
  inputManager->bind(emulator = &interface);
  if(!emulator->load(medium.id)) {
    emulator = nullptr;
    mediumPaths.reset();
    return;
  }
  updateAudioDriver();
  updateAudioEffects();
  connectDevices();
  emulator->power();

  presentation->resizeViewport();
  presentation->setTitle(emulator->title());
  presentation->systemMenu.setText(medium.name).setVisible(true);
  presentation->toolsMenu.setVisible(true);
  presentation->updateEmulator();
  toolsManager->cheatEditor.loadCheats();
  toolsManager->stateManager.doRefresh();
  toolsManager->manifestViewer.doRefresh();
  toolsManager->gameNotes.loadNotes();
}

auto Program::unloadMedium() -> void {
  if(!emulator) return;

  presentation->clearViewport();
  toolsManager->cheatEditor.saveCheats();
  toolsManager->gameNotes.saveNotes();
  emulator->unload();
  emulator = nullptr;
  mediumPaths.reset();

  presentation->resizeViewport();
  presentation->setTitle({"higan v", Emulator::Version});
  presentation->systemMenu.setVisible(false);
  presentation->toolsMenu.setVisible(false);
  toolsManager->setVisible(false);
}
