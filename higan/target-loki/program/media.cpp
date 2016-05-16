auto Program::loadMedia(string location) -> void {
  location.transform("\\", "/");
  if(!location.endsWith("/")) location.append("/");
  if(!directory::exists(location)) return;

  string type = suffixname(location).trimLeft(".", 1L);
  for(auto& media : emulator->media) {
    if(!media.bootable) continue;
    if(media.type != type) continue;
    return loadMedia(media, location);
  }
}

auto Program::loadMedia(Emulator::Interface::Media& media, string location) -> void {
  unloadMedia();

  mediaPaths(0) = locate({media.name, ".sys/"});
  mediaPaths(media.id) = location;
  folderPaths.append(location);

  emulator->set("Blur Emulation", false);
  emulator->set("Color Emulation", false);
  emulator->set("Scanline Emulation", false);

  Emulator::audio.reset();
  Emulator::audio.setFrequency(audio->get(Audio::Frequency).get<uint>());
  emulator->connect((uint)SFC::Port::Controller1, (uint)SFC::Device::Gamepad);
  emulator->connect((uint)SFC::Port::Controller2, (uint)SFC::Device::None);
  emulator->connect((uint)SFC::Port::Expansion,   (uint)SFC::Device::None);

  emulator->load(media.id);
  emulator->power();

  presentation->setTitle(emulator->title());
}

auto Program::unloadMedia() -> void {
  if(!emulator->loaded()) return;

  emulator->unload();
  mediaPaths.reset();
  folderPaths.reset();

  presentation->setTitle("");
}
