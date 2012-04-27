void InterfaceNES::initialize() {
  NES::interface = this;
  NES::system.init();
}

string InterfaceNES::markup() {
  return NES::cartridge.information.markup;
}

void InterfaceNES::setController(bool port, unsigned device) {
  if(port == 0) config->nes.controllerPort1Device = device;
  if(port == 1) config->nes.controllerPort2Device = device;

  if(port == 0) switch(device) {
  case 0: return NES::input.connect(0, NES::Input::Device::None);
  case 1: return NES::input.connect(0, NES::Input::Device::Joypad);
  }

  if(port == 1) switch(device) {
  case 0: return NES::input.connect(1, NES::Input::Device::None);
  case 1: return NES::input.connect(1, NES::Input::Device::Joypad);
  }
}

bool InterfaceNES::cartridgeLoaded() {
  return NES::cartridge.loaded();
}

bool InterfaceNES::loadCartridge(const string &filename) {
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
  if(markup.empty()) markup = FamicomCartridge(memory.data(), memory.size()).markup;

  NES::cartridge.load(markup, memory.data(), memory.size());
  NES::system.power();

  if(NES::cartridge.ram_size()) {
    filemap fp;
    if(fp.open(interface->base.filename("save.ram", ".sav"), filemap::mode::read)) {
      memcpy(NES::cartridge.ram_data(), fp.data(), min(NES::cartridge.ram_size(), fp.size()));
    }
  }

  interface->loadCartridge(::Interface::Mode::NES);
  NES::video.generate_palette();
  return true;
}

void InterfaceNES::unloadCartridge() {
  if(NES::cartridge.ram_size()) {
    file::write(interface->base.filename("save.ram", ".sav"), NES::cartridge.ram_data(), NES::cartridge.ram_size());
  }
  NES::cartridge.unload();
  interface->base.name = "";
}

//

void InterfaceNES::power() {
  NES::system.power();
}

void InterfaceNES::reset() {
  NES::system.reset();
}

void InterfaceNES::run() {
  NES::system.run();
}

//

serializer InterfaceNES::serialize() {
  NES::system.runtosave();
  return NES::system.serialize();
}

bool InterfaceNES::unserialize(serializer &s) {
  return NES::system.unserialize(s);
}

//

void InterfaceNES::setCheats(const lstring &list) {
  NES::cheat.reset();
  for(auto &code : list) {
    lstring codelist;
    codelist.split("+", code);
    for(auto &part : codelist) {
      unsigned addr, data, comp;
      if(NES::Cheat::decode(part, addr, data, comp)) {
        NES::cheat.append({ addr, data, comp });
      }
    }
  }
  NES::cheat.synchronize();
}

//

uint32_t InterfaceNES::videoColor(uint9_t source, uint16_t red, uint16_t green, uint16_t blue) {
  return color(red, green, blue);
}

void InterfaceNES::videoRefresh(const uint32_t *data) {
  interface->videoRefresh(data, 256 * sizeof(uint32_t), 256, 240);
}

void InterfaceNES::audioSample(int16_t sample) {
  signed samples[] = { sample };
  dspaudio.sample(samples);
  while(dspaudio.pending()) {
    dspaudio.read(samples);
    audio.sample(samples[0], samples[0]);  //NES audio output is monaural; ruby only takes stereo audio
  }
}

int16_t InterfaceNES::inputPoll(bool port, unsigned device, unsigned id) {
  if(port == 0 && device == 0) return inputManager->nes.port1.gamepad.poll(id);
  return 0;
}
