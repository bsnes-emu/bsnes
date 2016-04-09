auto Program::loadMedia(string location) -> void {
  location.transform("\\", "/");
  if(!location.endsWith("/")) location.append("/");
  if(!directory::exists(location)) return;

  string type = suffixname(location).ltrim(".", 1L);
  for(auto& emulator : emulators) {
    for(auto& media : emulator->media) {
      if(!media.bootable) continue;
      if(media.type != type) continue;
      return loadMedia(*emulator, media, location);
    }
  }
}

auto Program::loadMedia(Emulator::Interface& interface, Emulator::Interface::Media& media, string location) -> void {
  unloadMedia();

  mediaPaths(0) = locate({media.name, ".sys/"});
  mediaPaths(media.id) = location;
  folderPaths.append(location);

  //note: the order of operations in this block of code is critical
  emulator = &interface;
  connectDevices();
  emulator->load(media.id);
  updateAudio();
  emulator->power();

  presentation->resizeViewport();
  presentation->setTitle(emulator->title());
  presentation->systemMenu.setText(media.name).setVisible(true);
  presentation->toolsMenu.setVisible(true);
  presentation->updateEmulator();
  toolsManager->cheatEditor.loadCheats();
  toolsManager->stateManager.doRefresh();
  toolsManager->manifestViewer.doRefresh();
}

auto Program::unloadMedia() -> void {
  if(!emulator) return;

  toolsManager->cheatEditor.saveCheats();
  emulator->unload();
  emulator = nullptr;

  mediaPaths.reset();
  folderPaths.reset();

  presentation->setTitle({"higan v", Emulator::Version});
  presentation->systemMenu.setVisible(false);
  presentation->toolsMenu.setVisible(false);
  toolsManager->setVisible(false);
}
