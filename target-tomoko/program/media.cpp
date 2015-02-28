auto Program::loadMedia(string location) -> void {
  location.transform("\\", "/");
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
  mediaPaths(0) = {userpath(), "Emulation/System/", media.name, ".sys/"};
  mediaPaths(media.id) = location;

  setEmulator(_emulator);
  emulator().paletteUpdate(Emulator::Interface::PaletteMode::Standard);
  emulator().load(media.id);
  emulator().power();

  presentation->setTitle(emulator().title());
}
