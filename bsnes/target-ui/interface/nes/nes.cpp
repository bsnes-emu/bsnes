void InterfaceNES::initialize() {
  FC::interface = this;
  FC::system.init();
}

string InterfaceNES::markup() {
  return FC::cartridge.information.markup;
}

void InterfaceNES::setController(bool port, unsigned device) {
  if(port == 0) config->nes.controllerPort1Device = device;
  if(port == 1) config->nes.controllerPort2Device = device;

  if(port == 0) switch(device) {
  case 0: return FC::input.connect(0, FC::Input::Device::None);
  case 1: return FC::input.connect(0, FC::Input::Device::Joypad);
  }

  if(port == 1) switch(device) {
  case 0: return FC::input.connect(1, FC::Input::Device::None);
  case 1: return FC::input.connect(1, FC::Input::Device::Joypad);
  }
}

bool InterfaceNES::cartridgeLoaded() {
  return FC::cartridge.loaded();
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

  FC::cartridge.load(markup, vectorstream{memory});
  FC::system.power();

  if(FC::cartridge.ram_size()) {
    filestream fs{interface->base.filename("save.ram", ".sav")};
    fs.read(FC::cartridge.ram_data(), min(FC::cartridge.ram_size(), fs.size()));
  }

  interface->loadCartridge(::Interface::Mode::FC);
  FC::video.generate_palette();
  return true;
}

void InterfaceNES::unloadCartridge() {
  if(FC::cartridge.ram_size()) {
    file::write(interface->base.filename("save.ram", ".sav"), FC::cartridge.ram_data(), FC::cartridge.ram_size());
  }
  FC::cartridge.unload();
  interface->base.name = "";
}

//

void InterfaceNES::power() {
  FC::system.power();
}

void InterfaceNES::reset() {
  FC::system.reset();
}

void InterfaceNES::run() {
  FC::system.run();
}

//

serializer InterfaceNES::serialize() {
  FC::system.runtosave();
  return FC::system.serialize();
}

bool InterfaceNES::unserialize(serializer &s) {
  return FC::system.unserialize(s);
}

//

void InterfaceNES::setCheats(const lstring &list) {
  FC::cheat.reset();
  for(auto &code : list) {
    lstring codelist;
    codelist.split("+", code);
    for(auto &part : codelist) {
      unsigned addr, data, comp;
      if(FC::Cheat::decode(part, addr, data, comp)) {
        FC::cheat.append({ addr, data, comp });
      }
    }
  }
  FC::cheat.synchronize();
}

//

uint32_t InterfaceNES::videoColor(unsigned source, uint16_t red, uint16_t green, uint16_t blue) {
  return color(red, green, blue);
}

void InterfaceNES::videoRefresh(const uint32_t *data, unsigned pitch, unsigned width, unsigned height) {
  interface->videoRefresh(data, pitch, width, height);
}

void InterfaceNES::audioSample(int16_t lsample, int16_t rsample) {
  //NES audio output is monaural; ruby only takes stereo audio
  signed samples[] = {lsample};
  dspaudio.sample(samples);
  while(dspaudio.pending()) {
    dspaudio.read(samples);
    audio.sample(samples[0], samples[0]);
  }
}

int16_t InterfaceNES::inputPoll(unsigned port, unsigned device, unsigned id) {
  if(port == 0 && device == 0) return inputManager->nes.port1.gamepad.poll(id);
  return 0;
}
