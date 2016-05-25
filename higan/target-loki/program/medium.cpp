auto Program::loadMedium(string location) -> void {
  location.transform("\\", "/");
  if(!location.endsWith("/")) location.append("/");
  if(!directory::exists(location)) return;

  string type = suffixname(location).trimLeft(".", 1L);
  for(auto& medium : emulator->media) {
    if(!medium.bootable) continue;
    if(medium.type != type) continue;
    return loadMedium(medium, location);
  }
}

auto Program::loadMedium(Emulator::Interface::Medium& medium, string location) -> void {
  unloadMedium();

  mediumPaths(0) = locate({medium.name, ".sys/"});
  mediumPaths(medium.id) = location;
  folderPaths.append(location);

  emulator->set("Blur Emulation", false);
  emulator->set("Color Emulation", false);
  emulator->set("Scanline Emulation", false);

  Emulator::audio.reset();
  Emulator::audio.setFrequency(audio->get(Audio::Frequency).get<uint>());
  emulator->connect((uint)SFC::Port::Controller1, (uint)SFC::Device::Gamepad);
  emulator->connect((uint)SFC::Port::Controller2, (uint)SFC::Device::None);
  emulator->connect((uint)SFC::Port::Expansion,   (uint)SFC::Device::None);

  emulator->load(medium.id);
  emulator->power();

  presentation->setTitle(emulator->title());
}

auto Program::unloadMedium() -> void {
  if(!emulator->loaded()) return;

  emulator->unload();
  mediumPaths.reset();
  folderPaths.reset();

  presentation->setTitle("");
}
