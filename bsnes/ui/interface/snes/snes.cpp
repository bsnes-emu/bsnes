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

bool InterfaceSNES::loadCartridge(const string &basename) {
  uint8_t *data;
  unsigned size;
  if(interface->loadFile(basename, data, size) == false) return false;

  interface->unloadCartridge();
  interface->baseName = nall::basename(basename);
  interface->slotName = { nall::basename(basename) };

  string markup;
  markup.readfile({ interface->baseName, ".bml" });
  if(markup == "") markup = SnesCartridge(data, size).markup;

  SNES::cartridge.rom.copy(data, size);
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, markup);
  SNES::system.power();

  delete[] data;

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  SNES::video.generate(SNES::Video::Format::RGB24);
  return true;
}

bool InterfaceSNES::loadSatellaviewSlottedCartridge(const string &basename, const string &slotname) {
  uint8_t *data[2];
  unsigned size[2];
  if(interface->loadFile(basename, data[0], size[0]) == false) return false;
  interface->loadFile(slotname, data[1], size[1]);

  interface->unloadCartridge();
  interface->baseName = nall::basename(basename);
  if(data[1]) interface->baseName.append("+", nall::basename(notdir(slotname)));
  interface->slotName = { nall::basename(basename), nall::basename(slotname) };

  string markup;
  markup.readfile({ interface->baseName, ".bml" });
  if(markup == "") markup = SnesCartridge(data[0], size[0]).markup;

  SNES::cartridge.rom.copy(data[0], size[0]);
  if(data[1]) SNES::bsxflash.memory.copy(data[1], size[1]);
  SNES::cartridge.load(SNES::Cartridge::Mode::BsxSlotted, markup);
  SNES::system.power();

  delete[] data[0];
  if(data[1]) delete[] data[1];

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  SNES::video.generate(SNES::Video::Format::RGB24);
  return true;
}

bool InterfaceSNES::loadSatellaviewCartridge(const string &basename, const string &slotname) {
  uint8_t *data[2];
  unsigned size[2];
  if(interface->loadFile(basename, data[0], size[0]) == false) return false;
  interface->loadFile(slotname, data[1], size[1]);

  interface->unloadCartridge();
  interface->baseName = nall::basename(basename);
  if(data[1]) interface->baseName.append("+", nall::basename(notdir(slotname)));
  interface->slotName = { nall::basename(basename), nall::basename(slotname) };

  string markup;
  markup.readfile({ interface->baseName, ".bml" });
  if(markup == "") markup = SnesCartridge(data[0], size[0]).markup;

  SNES::cartridge.rom.copy(data[0], size[0]);
  if(data[1]) SNES::bsxflash.memory.copy(data[1], size[1]);
  SNES::cartridge.load(SNES::Cartridge::Mode::Bsx, markup);
  SNES::system.power();

  delete[] data[0];
  if(data[1]) delete[] data[1];

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  SNES::video.generate(SNES::Video::Format::RGB24);
  return true;
}

bool InterfaceSNES::loadSufamiTurboCartridge(const string &basename, const string &slotAname, const string &slotBname) {
  uint8_t *data[3];
  unsigned size[3];
  if(interface->loadFile(basename, data[0], size[0]) == false) return false;
  interface->loadFile(slotAname, data[1], size[1]);
  interface->loadFile(slotBname, data[2], size[2]);

  interface->unloadCartridge();
  interface->baseName = nall::basename(basename);
  if(data[1] && data[2]) interface->baseName = { nall::basename(slotAname), "+", nall::basename(notdir(slotBname)) };
  else if(data[1]) interface->baseName = nall::basename(slotAname);
  else if(data[2]) interface->baseName = nall::basename(slotBname);
  interface->slotName = { nall::basename(basename), nall::basename(slotAname), nall::basename(slotBname) };

  string markup;
  markup.readfile({ interface->baseName, ".bml" });
  if(markup == "") markup = SnesCartridge(data[0], size[0]).markup;

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
  SNES::video.generate(SNES::Video::Format::RGB24);
  return true;
}

bool InterfaceSNES::loadSuperGameBoyCartridge(const string &basename, const string &slotname) {
  uint8_t *data[2];
  unsigned size[2];
  if(interface->loadFile(basename, data[0], size[0]) == false) return false;
  interface->loadFile(slotname, data[1], size[1]);

  interface->unloadCartridge();
  interface->baseName = nall::basename(basename);
  if(data[1]) interface->baseName = nall::basename(slotname);
  interface->slotName = { nall::basename(basename), nall::basename(slotname) };

  string markup;
  markup.readfile({ interface->baseName, ".bml" });
  if(markup == "") markup = SnesCartridge(data[0], size[0]).markup;

  string gbMarkup;
  gbMarkup.readfile({ nall::basename(slotname), ".bml" });
  if(gbMarkup == "") gbMarkup = GameBoyCartridge(data[1], size[1]).markup;

  SNES::cartridge.rom.copy(data[0], size[0]);
  GameBoy::cartridge.load(GameBoy::System::Revision::SuperGameBoy, gbMarkup, data[1], size[1]);
  SNES::cartridge.load(SNES::Cartridge::Mode::SuperGameBoy, markup);
  SNES::system.power();

  delete[] data[0];
  if(data[1]) delete[] data[1];

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  SNES::video.generate(SNES::Video::Format::RGB24);
  return true;
}

void InterfaceSNES::unloadCartridge() {
  saveMemory();
  SNES::cartridge.unload();
  interface->baseName = "";
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

//slot[] array = Cartridge::Slot to slot# conversion:
//{ Base, Bsx, SufamiTurbo, SufamiTurboA, SufamiTurboB, GameBoy }

void InterfaceSNES::loadMemory() {
  static unsigned slot[] = { 0, 0, 0, 1, 2, 1 };
  for(auto &memory : SNES::cartridge.nvram) {
    if(memory.size == 0) continue;
    string filename = { interface->slotName[slot[(unsigned)memory.slot]], memory.id };
    uint8_t *data;
    unsigned size;
    if(file::read(filename, data, size)) {
      memcpy(memory.data, data, min(memory.size, size));
      delete[] data;
    }
  }
}

void InterfaceSNES::saveMemory() {
  static unsigned slot[] = { 0, 0, 0, 1, 2, 1 };
  for(auto &memory : SNES::cartridge.nvram) {
    if(memory.size == 0) continue;
    string filename = { interface->slotName[slot[(unsigned)memory.slot]], memory.id };
    file::write(filename, memory.data, memory.size);
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
      if(index == 1) return inputManager->snes.port1.multitap1.poll(id);
      if(index == 2) return inputManager->snes.port1.multitap1.poll(id);
      if(index == 3) return inputManager->snes.port1.multitap1.poll(id);
    }
    if(device == SNES::Input::Device::Mouse) return inputManager->snes.port1.mouse.poll(id);
  }

  if(port == 1) {
    if(device == SNES::Input::Device::Joypad) return inputManager->snes.port2.gamepad.poll(id);
    if(device == SNES::Input::Device::Multitap) {
      if(index == 0) return inputManager->snes.port2.multitap1.poll(id);
      if(index == 1) return inputManager->snes.port2.multitap1.poll(id);
      if(index == 2) return inputManager->snes.port2.multitap1.poll(id);
      if(index == 3) return inputManager->snes.port2.multitap1.poll(id);
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
  static unsigned index[] = { 0, 0, 0, 1, 2, 1 };
  return { interface->slotName[index[(unsigned)slot]], hint };
}

void InterfaceSNES::message(const string &text) {
  MessageWindow::information(*mainWindow, text);
}
