auto Program::loadMedia(string location) -> void {
  location.transform("\\", "/");
  if(!location.endsWith("/")) location.append("/");
  if(!directory::exists(location)) return;

  string type = suffixname(location).ltrim(".");
  for(auto& emulator : emulators) {
    for(auto& media : emulator->media) {
      if(media.bootable == false) continue;
      if(media.type != type) continue;
      return loadMedia(*emulator, media, location);
    }
  }
}

auto Program::loadMedia(Emulator::Interface& _emulator, Emulator::Interface::Media& media, const string& location) -> void {
  unloadMedia();

  mediaPaths(0) = {userpath(), "Emulation/System/", media.name, ".sys/"};
  mediaPaths(media.id) = location;

  setEmulator(&_emulator);
  emulator().paletteUpdate(Emulator::Interface::PaletteMode::Standard);
  emulator().load(media.id);
  emulator().power();

  presentation->resizeViewport();
  presentation->setTitle(emulator().title());
  presentation->systemMenu.setVisible(true);
}

auto Program::unloadMedia() -> void {
  if(activeEmulator == nullptr) return;
  emulator().unload();

  setEmulator(nullptr);

  presentation->setTitle({"tomoko v", Emulator::Version});
  presentation->systemMenu.setVisible(false);
}
