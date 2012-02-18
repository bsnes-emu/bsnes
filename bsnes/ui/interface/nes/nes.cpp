void InterfaceNES::initialize() {
  NES::interface = this;
  NES::system.init();
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

  uint8_t *data;
  unsigned size;

  if(filename.endswith("/")) {
    if(file::exists({ filename, "program.rom" }) && file::exists({ filename, "character.rom" })) {
      unsigned prgsize = file::size({ filename, "program.rom" });
      unsigned chrsize = file::size({ filename, "character.rom" });
      data = new uint8_t[size = prgsize + chrsize];
      nall::file fp;
      fp.open({ filename, "program.rom" }, file::mode::read);
      fp.read(data, fp.size());
      fp.close();
      fp.open({ filename, "character.rom" }, file::mode::read);
      fp.read(data + prgsize, fp.size());
      fp.close();
    } else {
      return false;
    }
    interface->base = { true, filename };
  } else {
    file::read(filename, data, size);
    interface->base = { false, nall::basename(filename) };
  }

  interface->game = interface->base;
  interface->cartridgeTitle = interface->base.title();
  interface->applyPatch(interface->base, data, size);

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));

  NES::cartridge.load(markup, data, size);
  NES::system.power();
  delete[] data;

  if(NES::cartridge.ram_size()) {
    filemap fp;
    if(fp.open(interface->base.filename("program.ram", ".sav"), filemap::mode::read)) {
      memcpy(NES::cartridge.ram_data(), fp.data(), min(NES::cartridge.ram_size(), fp.size()));
    }
  }

  interface->loadCartridge(::Interface::Mode::NES);
  NES::video.generate(NES::Video::Format::RGB30);
  return true;
}

void InterfaceNES::unloadCartridge() {
  if(NES::cartridge.ram_size()) {
    file::write(interface->base.filename("program.ram", ".sav"), NES::cartridge.ram_data(), NES::cartridge.ram_size());
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

void InterfaceNES::videoRefresh(const uint16_t *data) {
  static uint32_t output[256 * 240];

  for(unsigned y = 0; y < 240; y++) {
    const uint16_t *sp = data + y * 256;
    uint32_t *dp = output + y * 256;
    for(unsigned x = 0; x < 256; x++) {
      uint32_t color = *sp++;
      *dp++ = NES::video.palette[color];
    }
  }

  if(config->video.maskOverscan) {
    unsigned osw = config->video.maskOverscanHorizontal;
    unsigned osh = config->video.maskOverscanVertical;

    for(unsigned y = 0; y < 240; y++) {
      uint32_t *dp = output + y * 256;
      if(y < osh || y >= 240 - osh) {
        memset(dp, 0, 256 * sizeof(uint32_t));
      } else {
        memset(dp + 0, 0, osw * sizeof(uint32_t));
        memset(dp + 256 - osw, 0, osw * sizeof(uint32_t));
      }
    }
  }

  interface->videoRefresh(output, 256 * sizeof(uint32_t), 256, 240);
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
