void InterfaceGBA::initialize() {
  loadFirmware("Game Boy Advance.sys/manifest.xml", "system.bios", GBA::bios.data, 16384u);

  GBA::interface = this;
  GBA::system.init();
}

string InterfaceGBA::markup() {
  return GBA::cartridge.information.markup;
}

bool InterfaceGBA::cartridgeLoaded() {
  return GBA::cartridge.loaded();
}

bool InterfaceGBA::loadCartridge(const string &filename) {
  interface->unloadCartridge();

  vector<uint8_t> memory;
  if(filename.endswith("/")) {
    memory = file::read({filename, "program.rom"});
    interface->base = {true, filename};
  } else {
    memory = file::read(filename);
    interface->base = {false, nall::basename(filename)};
  }
  if(memory.empty()) return false;

  interface->game = interface->base;
  interface->cartridgeTitle = interface->base.title();
  interface->applyPatch(interface->base, memory);

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));
  if(markup.empty()) markup = GameBoyAdvanceCartridge(memory.data(), memory.size()).markup;

  GBA::cartridge.load(markup, vectorstream{memory});
  GBA::system.power();

  if(GBA::cartridge.ram_size()) {
    filemap fp;
    if(fp.open(interface->base.filename("save.ram", ".sav"), filemap::mode::read)) {
      memcpy(GBA::cartridge.ram_data(), fp.data(), min(GBA::cartridge.ram_size(), fp.size()));
    }
  }

  GBA::video.generate_palette();
  interface->loadCartridge(::Interface::Mode::GBA);
  return true;
}

void InterfaceGBA::unloadCartridge() {
  if(GBA::cartridge.ram_size()) {
    file::write(interface->base.filename("save.ram", ".sav"), GBA::cartridge.ram_data(), GBA::cartridge.ram_size());
  }

  GBA::cartridge.unload();
  interface->base.name = "";
}

void InterfaceGBA::power() {
  return GBA::system.power();
}

void InterfaceGBA::reset() {
  return GBA::system.power();  //GBA has no reset button
}

void InterfaceGBA::run() {
  return GBA::system.run();
}

serializer InterfaceGBA::serialize() {
  GBA::system.runtosave();
  return GBA::system.serialize();
}

bool InterfaceGBA::unserialize(serializer &s) {
  return GBA::system.unserialize(s);
}

void InterfaceGBA::setCheats(const lstring &list) {
}

//

uint32_t InterfaceGBA::videoColor(uint15_t source, uint16_t red, uint16_t green, uint16_t blue) {
  return color(red, green, blue);
}

void InterfaceGBA::videoRefresh(const uint32_t *data) {
  interface->videoRefresh(data, 240 * sizeof(uint32_t), 240, 160);
}

void InterfaceGBA::audioSample(int16_t lsample, int16_t rsample) {
  signed samples[] = { lsample, rsample };
  dspaudio.sample(samples);
  while(dspaudio.pending()) {
    dspaudio.read(samples);
    audio.sample(samples[0], samples[1]);
  }
}

bool InterfaceGBA::inputPoll(unsigned id) {
  return inputManager->gba.device.controller.poll(id);
}
