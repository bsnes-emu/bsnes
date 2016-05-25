auto Program::loadMedium(string location) -> void {
  location.transform("\\", "/");
  if(!location.endsWith("/")) location.append("/");
  if(!directory::exists(location)) return;

  string type = suffixname(location).trimLeft(".", 1L);
  for(auto& emulator : emulators) {
    for(auto& medium : emulator->media) {
      if(!medium.bootable) continue;
      if(medium.type != type) continue;
      return loadMedium(*emulator, medium, location);
    }
  }
}

auto Program::loadMedium(Emulator::Interface& interface, Emulator::Interface::Medium& medium, string location) -> void {
  unloadMedium();

  mediumPaths(0) = locate({medium.name, ".sys/"});
  mediumPaths(medium.id) = location;
  folderPaths.append(location);

  //note: the order of operations in this block of code is critical
  Emulator::audio.reset();
  Emulator::audio.setFrequency(audio->get(Audio::Frequency).get<uint>());
  emulator = &interface;
  connectDevices();
  emulator->load(medium.id);
  updateAudioDriver();
  updateAudioEffects();
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

  toolsManager->cheatEditor.saveCheats();
  emulator->unload();
  emulator = nullptr;

  mediumPaths.reset();
  folderPaths.reset();

  presentation->setTitle({"higan v", Emulator::Version});
  presentation->systemMenu.setVisible(false);
  presentation->toolsMenu.setVisible(false);
  toolsManager->setVisible(false);
}
