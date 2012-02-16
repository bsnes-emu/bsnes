void InterfaceSNES::initialize() {
  SNES::interface = this;
  SNES::system.init();
}

void InterfaceSNES::setController(bool port, unsigned device) {
  if(port == 0) config->snes.controllerPort1Device = device;
  if(port == 1) config->snes.controllerPort2Device = device;

  if(port == 0) switch(device) {
  case 0: return SNES::input.connect(0, SNES::Input::Device::None);
  case 1: return SNES::input.connect(0, SNES::Input::Device::Joypad);
  case 2: return SNES::input.connect(0, SNES::Input::Device::Multitap);
  case 3: return SNES::input.connect(0, SNES::Input::Device::Mouse);
  }

  if(port == 1) switch(device) {
  case 0: return SNES::input.connect(1, SNES::Input::Device::None);
  case 1: return SNES::input.connect(1, SNES::Input::Device::Joypad);
  case 2: return SNES::input.connect(1, SNES::Input::Device::Multitap);
  case 3: return SNES::input.connect(1, SNES::Input::Device::Mouse);
  case 4: return SNES::input.connect(1, SNES::Input::Device::SuperScope);
  case 5: return SNES::input.connect(1, SNES::Input::Device::Justifier);
  case 6: return SNES::input.connect(1, SNES::Input::Device::Justifiers);
  case 7: return SNES::input.connect(1, SNES::Input::Device::Serial);
  }
}

bool InterfaceSNES::cartridgeLoaded() {
  return SNES::cartridge.loaded();
}

bool InterfaceSNES::loadCartridge(const string &filename, CartridgePath &cartridge, uint8_t *&data, unsigned &size) {
  data = nullptr, size = 0u;
  auto backup = cartridge;
  string suffix;
  if(filename.endswith("/")) {
    cartridge = { true, filename };
  } else {
    suffix = { ".", extension(filename) };
    cartridge = { false, nall::basename(filename) };
  }
  if(file::read(cartridge.filename("program.rom", suffix), data, size) == false) {
    cartridge = backup;
    return false;
  }
  interface->applyPatch(cartridge, data, size);
  return true;
}

bool InterfaceSNES::loadCartridge(string basename) {
  uint8_t *data;
  unsigned size;
  if(loadCartridge(basename, interface->base, data, size) == false) return false;

  interface->unloadCartridge();
  interface->game = interface->base;
  interface->cartridgeTitle = interface->base.title();

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));
  if(markup.empty()) markup = SnesCartridge(data, size).markup;

  SNES::cartridge.rom.copy(data, size);
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, markup);
  SNES::system.power();

  delete[] data;

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  SNES::video.generate(SNES::Video::Format::RGB30);
  return true;
}

bool InterfaceSNES::loadSatellaviewSlottedCartridge(string basename, string slotname) {
  uint8_t *data[2];
  unsigned size[2];
  if(loadCartridge(basename, interface->base, data[0], size[0]) == false) return false;
  loadCartridge(slotname, interface->slot[0], data[1], size[1]);

  interface->unloadCartridge();
  interface->game = !data[1] ? interface->base : interface->slot[0];  //TODO: subfolder for folders; concatenation for files
  interface->cartridgeTitle = interface->base.title();
  if(data[1]) interface->cartridgeTitle.append(" + ", interface->slot[0].title());

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));
  if(markup.empty()) markup = SnesCartridge(data[0], size[0]).markup;

  SNES::cartridge.rom.copy(data[0], size[0]);
  if(data[1]) SNES::bsxflash.memory.copy(data[1], size[1]);
  SNES::cartridge.load(SNES::Cartridge::Mode::BsxSlotted, markup);
  SNES::system.power();

  delete[] data[0];
  if(data[1]) delete[] data[1];

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  SNES::video.generate(SNES::Video::Format::RGB30);
  return true;
}

bool InterfaceSNES::loadSatellaviewCartridge(string basename, string slotname) {
  uint8_t *data[2];
  unsigned size[2];
  if(loadCartridge(basename, interface->base, data[0], size[0]) == false) return false;
  loadCartridge(slotname, interface->slot[0], data[1], size[1]);

  interface->unloadCartridge();
  interface->game = !data[1] ? interface->base : interface->slot[0];
  interface->cartridgeTitle = interface->base.title();
  if(data[1]) interface->cartridgeTitle = interface->slot[0].title();

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));
  if(markup.empty()) markup = SnesCartridge(data[0], size[0]).markup;

  SNES::cartridge.rom.copy(data[0], size[0]);
  if(data[1]) SNES::bsxflash.memory.copy(data[1], size[1]);
  SNES::cartridge.load(SNES::Cartridge::Mode::Bsx, markup);
  SNES::system.power();

  delete[] data[0];
  if(data[1]) delete[] data[1];

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  SNES::video.generate(SNES::Video::Format::RGB30);
  return true;
}

bool InterfaceSNES::loadSufamiTurboCartridge(string basename, string slotAname, string slotBname) {
  uint8_t *data[3];
  unsigned size[3];
  if(loadCartridge(basename, interface->base, data[0], size[0]) == false) return false;
  loadCartridge(slotAname, interface->slot[0], data[1], size[1]);
  loadCartridge(slotBname, interface->slot[1], data[2], size[2]);

  interface->unloadCartridge();
  interface->game = !data[1] ? interface->base : interface->slot[0];  //TODO: subfolder for folders; concatenation for files
  interface->cartridgeTitle = interface->base.title();
  if( data[1] && !data[2]) interface->cartridgeTitle = interface->slot[0].title();
  if(!data[1] &&  data[2]) interface->cartridgeTitle = interface->slot[1].title();
  if( data[1] &&  data[2]) interface->cartridgeTitle = {
    interface->slot[0].title(), " + ", interface->slot[1].title()
  };

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));
  if(markup.empty()) markup = SnesCartridge(data[0], size[0]).markup;

  SNES::cartridge.rom.copy(data[0], size[0]);
  if(data[1]) SNES::sufamiturbo.slotA.rom.copy(data[1], size[1]);
  if(data[2]) SNES::sufamiturbo.slotB.rom.copy(data[1], size[1]);
  SNES::cartridge.load(SNES::Cartridge::Mode::SufamiTurbo, markup);
  SNES::system.power();

  delete[] data[0];
  if(data[1]) delete[] data[1];
  if(data[2]) delete[] data[2];

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  SNES::video.generate(SNES::Video::Format::RGB30);
  return true;
}

bool InterfaceSNES::loadSuperGameBoyCartridge(string basename, string slotname) {
  uint8_t *data[2];
  unsigned size[2];
  if(loadCartridge(basename, interface->base, data[0], size[0]) == false) return false;
  loadCartridge(slotname, interface->slot[0], data[1], size[1]);

  interface->unloadCartridge();
  interface->game = !data[1] ? interface->base : interface->slot[0];
  interface->cartridgeTitle = interface->base.title();
  if(data[1]) interface->cartridgeTitle = interface->slot[0].title();

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));
  if(markup.empty()) markup = SnesCartridge(data[0], size[0]).markup;

  string gbMarkup;
  gbMarkup.readfile(interface->slot[0].filename("manifest.xml", ".xml"));
  if(gbMarkup.empty()) gbMarkup = GameBoyCartridge(data[1], size[1]).markup;

  SNES::cartridge.rom.copy(data[0], size[0]);
  GameBoy::cartridge.load(GameBoy::System::Revision::SuperGameBoy, gbMarkup, data[1], size[1]);
  SNES::cartridge.load(SNES::Cartridge::Mode::SuperGameBoy, markup);
  SNES::system.power();

  delete[] data[0];
  if(data[1]) delete[] data[1];

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  SNES::video.generate(SNES::Video::Format::RGB30);
  return true;
}

void InterfaceSNES::unloadCartridge() {
  saveMemory();
  SNES::cartridge.unload();
}

void InterfaceSNES::power() {
  SNES::system.power();
}

void InterfaceSNES::reset() {
  SNES::system.reset();
}

void InterfaceSNES::run() {
  SNES::system.run();
}

string InterfaceSNES::memoryName(SNES::Cartridge::NonVolatileRAM &memory) {
  if(memory.slot == SNES::Cartridge::Slot::Base) {
    if(memory.id == "program.ram") return interface->base.filename("program.ram", ".srm");
    if(memory.id == "upd96050.ram") return interface->base.filename("upd96050.ram", ".nec");
    if(memory.id == "bsx.ram") return interface->base.filename("bsx.ram", ".bss");
    if(memory.id == "bsx.psram") return interface->base.filename("bsx.psram", ".bsp");
  }
  if(memory.slot == SNES::Cartridge::Slot::SufamiTurboA) {
    if(memory.id == "program.ram") return interface->slot[0].filename("program.ram", ".sts");
  }
  if(memory.slot == SNES::Cartridge::Slot::SufamiTurboB) {
    if(memory.id == "program.ram") return interface->slot[1].filename("program.ram", ".sts");
  }
  return "";
}

void InterfaceSNES::loadMemory() {
  for(auto &memory : SNES::cartridge.nvram) {
    if(memory.size == 0) continue;

    string filename = memoryName(memory);
    if(filename.empty()) continue;

    uint8_t *data;
    unsigned size;
    if(file::read(filename, data, size)) {
      memcpy(memory.data, data, min(memory.size, size));
      delete[] data;
    }
  }

  if(SNES::cartridge.mode() == SNES::Cartridge::Mode::SuperGameBoy) {
    if(GameBoy::cartridge.ramsize) {
      uint8_t *data;
      unsigned size;
      if(file::read(interface->slot[0].filename("program.ram", ".sav"), data, size)) {
        memcpy(GameBoy::cartridge.ramdata, data, min(GameBoy::cartridge.ramsize, size));
        delete[] data;
      }
    }
  }
}

void InterfaceSNES::saveMemory() {
  for(auto &memory : SNES::cartridge.nvram) {
    if(memory.size == 0) continue;

    string filename = memoryName(memory);
    if(filename.empty()) continue;

    file::write(filename, memory.data, memory.size);
  }

  if(SNES::cartridge.mode() == SNES::Cartridge::Mode::SuperGameBoy) {
    if(GameBoy::cartridge.ramsize) {
      file::write(interface->slot[0].filename("program.ram", ".sav"),
        GameBoy::cartridge.ramdata, GameBoy::cartridge.ramsize
      );
    }
  }
}

serializer InterfaceSNES::serialize() {
  SNES::system.runtosave();
  return SNES::system.serialize();
}

bool InterfaceSNES::unserialize(serializer &s) {
  return SNES::system.unserialize(s);
}

void InterfaceSNES::setCheats(const lstring &list) {
  if(SNES::cartridge.mode() == SNES::Cartridge::Mode::SuperGameBoy) {
    GameBoy::cheat.reset();
    for(auto &code : list) {
      lstring codelist;
      codelist.split("+", code);
      for(auto &part : codelist) {
        unsigned addr, data, comp;
        if(GameBoy::Cheat::decode(part, addr, data, comp)) {
          GameBoy::cheat.append({ addr, data, comp });
        }
      }
    }
    GameBoy::cheat.synchronize();
    return;
  }

  SNES::cheat.reset();
  for(auto &code : list) {
    lstring codelist;
    codelist.split("+", code);
    for(auto &part : codelist) {
      unsigned addr, data;
      if(SNES::Cheat::decode(part, addr, data)) {
        SNES::cheat.append({ addr, data });
      }
    }
  }
  SNES::cheat.synchronize();
}

//

void InterfaceSNES::videoRefresh(const uint32_t *data, bool hires, bool interlace, bool overscan) {
  static uint32_t output[512 * 480];

  unsigned width = 256 << hires;
  unsigned height = 240 << interlace;
  unsigned pitch = 1024 >> interlace;

  //skip first line; as it is always blank (by SNES design)
  if(overscan == false) data +=  1 * 1024;  // 8 + 224 +  8
  if(overscan == true ) data +=  9 * 1024;  // 0 + 240 +  0

  for(unsigned y = 0; y < height; y++) {
    const uint32_t *sp = data + y * pitch;
    uint32_t *dp = output + y * 512;
    for(unsigned x = 0; x < width; x++) {
      *dp++ = SNES::video.palette[*sp++];
    }
  }

  if(config->video.maskOverscan) {
    unsigned osw = config->video.maskOverscanHorizontal << hires;
    unsigned osh = config->video.maskOverscanVertical << interlace;

    for(unsigned y = 0; y < height; y++) {
      uint32_t *dp = output + y * 512;
      if(y < osh || y >= height - osh) {
        memset(dp, 0, width * sizeof(uint32_t));
      } else {
        memset(dp + 0, 0, osw * sizeof(uint32_t));
        memset(dp + width - osw, 0, osw * sizeof(uint32_t));
      }
    }
  }

  interface->videoRefresh(output, 512 * sizeof(uint32_t), width, height);
}

void InterfaceSNES::audioSample(int16_t lsample, int16_t rsample) {
  signed samples[] = { lsample, rsample };
  dspaudio.sample(samples);
  while(dspaudio.pending()) {
    dspaudio.read(samples);
    audio.sample(samples[0], samples[1]);
  }
}

int16_t InterfaceSNES::inputPoll(bool port, SNES::Input::Device device, unsigned index, unsigned id) {
  if(port == 0) {
    if(device == SNES::Input::Device::Joypad) return inputManager->snes.port1.gamepad.poll(id);
    if(device == SNES::Input::Device::Multitap) {
      if(index == 0) return inputManager->snes.port1.multitap1.poll(id);
      if(index == 1) return inputManager->snes.port1.multitap2.poll(id);
      if(index == 2) return inputManager->snes.port1.multitap3.poll(id);
      if(index == 3) return inputManager->snes.port1.multitap4.poll(id);
    }
    if(device == SNES::Input::Device::Mouse) return inputManager->snes.port1.mouse.poll(id);
  }

  if(port == 1) {
    if(device == SNES::Input::Device::Joypad) return inputManager->snes.port2.gamepad.poll(id);
    if(device == SNES::Input::Device::Multitap) {
      if(index == 0) return inputManager->snes.port2.multitap1.poll(id);
      if(index == 1) return inputManager->snes.port2.multitap2.poll(id);
      if(index == 2) return inputManager->snes.port2.multitap3.poll(id);
      if(index == 3) return inputManager->snes.port2.multitap4.poll(id);
    }
    if(device == SNES::Input::Device::Mouse) return inputManager->snes.port2.mouse.poll(id);
    if(device == SNES::Input::Device::SuperScope) return inputManager->snes.port2.superScope.poll(id);
    if(device == SNES::Input::Device::Justifier) return inputManager->snes.port2.justifier1.poll(id);
    if(device == SNES::Input::Device::Justifiers) {
      if(index == 0) return inputManager->snes.port2.justifier1.poll(id);
      if(index == 1) return inputManager->snes.port2.justifier2.poll(id);
    }
  }

  return 0;
}

string InterfaceSNES::path(SNES::Cartridge::Slot slot, const string &hint) {
  if(slot == SNES::Cartridge::Slot::Base) {
    if(hint == "msu1.rom") return interface->base.filename("msu1.rom", ".msu");
    if(hint.wildcard("track-*.pcm")) {
      string track = hint;
      track.trim<1>("track-", ".pcm");
      return interface->base.filename(hint, { "-", decimal(track), ".pcm" });
    }
    if(hint == "serial.so") return { dir(interface->base.name), "libserial.so" };
    if(hint.endswith(".rom")) return { dir(interface->base.name), hint };
  }
  return { dir(interface->base.name), hint };
}

void InterfaceSNES::message(const string &text) {
  MessageWindow::information(*mainWindow, text);
}
