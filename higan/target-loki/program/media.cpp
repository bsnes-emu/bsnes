auto Program::loadMedia(string location) -> void {
  location.transform("\\", "/");
  if(!location.endsWith("/")) location.append("/");
  if(!directory::exists(location)) return;

  string type = suffixname(location).ltrim(".", 1L);
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

  emulator->connect((uint)SFC::Device::Port::Controller1, (uint)SFC::Device::ID::Gamepad);
  emulator->connect((uint)SFC::Device::Port::Controller2, (uint)SFC::Device::ID::None);
  emulator->connect((uint)SFC::Device::Port::Expansion,   (uint)SFC::Device::ID::None);

  emulator->load(media.id);
  emulator->power();

  dsp.setResampler(DSP::ResampleEngine::Sinc);
  dsp.setResamplerFrequency(96000.0);
  double inputRatio = emulator->audioFrequency() / emulator->videoFrequency();
  double outputRatio = 96000.0 / 60.0;
  dsp.setFrequency(inputRatio / outputRatio * 96000.0);

  presentation->setTitle(emulator->title());
}

auto Program::unloadMedia() -> void {
  if(!emulator->loaded()) return;

  emulator->unload();
  mediaPaths.reset();
  folderPaths.reset();

  presentation->setTitle("");
}
