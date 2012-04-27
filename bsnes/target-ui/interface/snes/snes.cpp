void InterfaceSNES::initialize() {
  loadFirmware("Super Famicom.sys/manifest.xml", "system.smp", SNES::smp.iplrom, 64u);

  SNES::interface = this;
  SNES::system.init();
}

string InterfaceSNES::markup() {
  return SNES::cartridge.information.markup;
}

void InterfaceSNES::setController(bool port, unsigned device) {
  if(port == 0) config->snes.controllerPort1Device = device;
  if(port == 1) config->snes.controllerPort2Device = device;

  if(port == 0) switch(device) { default:
  case 0: return SNES::input.connect(0, SNES::Input::Device::None);
  case 1: return SNES::input.connect(0, SNES::Input::Device::Joypad);
  case 2: return SNES::input.connect(0, SNES::Input::Device::Multitap);
  case 3: return SNES::input.connect(0, SNES::Input::Device::Mouse);
  case 4: return SNES::input.connect(0, SNES::Input::Device::USART);
  }

  if(port == 1) switch(device) { default:
  case 0: return SNES::input.connect(1, SNES::Input::Device::None);
  case 1: return SNES::input.connect(1, SNES::Input::Device::Joypad);
  case 2: return SNES::input.connect(1, SNES::Input::Device::Multitap);
  case 3: return SNES::input.connect(1, SNES::Input::Device::Mouse);
  case 4: return SNES::input.connect(1, SNES::Input::Device::SuperScope);
  case 5: return SNES::input.connect(1, SNES::Input::Device::Justifier);
  case 6: return SNES::input.connect(1, SNES::Input::Device::Justifiers);
  case 7: return SNES::input.connect(1, SNES::Input::Device::USART);
  }
}

bool InterfaceSNES::cartridgeLoaded() {
  return SNES::cartridge.loaded();
}

vector<uint8_t> InterfaceSNES::loadCartridge(const string &filename, CartridgePath &cartridge) {
  vector<uint8_t> memory;
  auto backup = cartridge;
  string suffix;
  if(filename.endswith("/")) {
    cartridge = {true, filename};
  } else {
    suffix = {".", extension(filename)};
    cartridge = {false, nall::basename(filename)};
  }
  memory = file::read(cartridge.filename("program.rom", suffix));
  interface->applyPatch(cartridge, memory);
  if(memory.empty()) cartridge = backup;
  return memory;
}

bool InterfaceSNES::loadCartridge(string basename) {
  interface->unloadCartridge();

  auto memory = loadCartridge(basename, interface->base);
  if(memory.empty()) return false;

  interface->game = interface->base;
  interface->cartridgeTitle = interface->base.title();

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));
  if(markup.empty()) markup = SuperFamicomCartridge(memory.data(), memory.size()).markup;

  SNES::cartridge.rom.copy(memory.data(), memory.size());
  SNES::cartridge.load(SNES::Cartridge::Mode::Normal, markup);
  SNES::system.power();

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  SNES::video.generate_palette();
  return true;
}

bool InterfaceSNES::loadSatellaviewSlottedCartridge(string basename, string slotname) {
  interface->unloadCartridge();

  auto memory = loadCartridge(basename, interface->base);
  if(memory.empty()) return false;
  auto memoryBS = loadCartridge(slotname, interface->slot[0]);

  interface->game = memoryBS.empty() ? interface->base : interface->slot[0];  //TODO: subfolder for folders; concatenation for files
  interface->cartridgeTitle = interface->base.title();
  if(memoryBS) interface->cartridgeTitle.append(" + ", interface->slot[0].title());

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));
  if(markup.empty()) markup = SuperFamicomCartridge(memory.data(), memory.size()).markup;

  SNES::cartridge.rom.copy(memory.data(), memory.size());
  if(memoryBS) SNES::bsxflash.memory.copy(memoryBS.data(), memoryBS.size());
  SNES::cartridge.load(SNES::Cartridge::Mode::BsxSlotted, markup);
  SNES::system.power();

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  SNES::video.generate_palette();
  return true;
}

bool InterfaceSNES::loadSatellaviewCartridge(string basename, string slotname) {
  interface->unloadCartridge();

  auto memory = loadCartridge(basename, interface->base);
  if(memory.empty()) return false;
  auto memoryBS = loadCartridge(slotname, interface->slot[0]);

  interface->game = memoryBS.empty() ? interface->base : interface->slot[0];
  interface->cartridgeTitle = interface->base.title();
  if(memoryBS) interface->cartridgeTitle = interface->slot[0].title();

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));
  if(markup.empty()) markup = SuperFamicomCartridge(memory.data(), memory.size()).markup;

  SNES::cartridge.rom.copy(memory.data(), memory.size());
  if(memoryBS) SNES::bsxflash.memory.copy(memoryBS.data(), memoryBS.size());
  SNES::cartridge.load(SNES::Cartridge::Mode::Bsx, markup);
  SNES::system.power();

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  SNES::video.generate_palette();
  return true;
}

bool InterfaceSNES::loadSufamiTurboCartridge(string basename, string slotAname, string slotBname) {
  interface->unloadCartridge();

  auto memory = loadCartridge(basename, interface->base);
  if(memory.empty()) return false;
  auto memorySTA = loadCartridge(slotAname, interface->slot[0]);
  auto memorySTB = loadCartridge(slotBname, interface->slot[1]);

  interface->game = memorySTA.empty() ? interface->base : interface->slot[0];  //TODO: subfolder for folders; concatenation for files
  interface->cartridgeTitle = interface->base.title();
  if( memorySTA && !memorySTB) interface->cartridgeTitle = interface->slot[0].title();
  if(!memorySTA &&  memorySTB) interface->cartridgeTitle = interface->slot[1].title();
  if( memorySTA &&  memorySTB) interface->cartridgeTitle = {
    interface->slot[0].title(), " + ", interface->slot[1].title()
  };

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));
  if(markup.empty()) markup = SuperFamicomCartridge(memory.data(), memory.size()).markup;

  SNES::cartridge.rom.copy(memory.data(), memory.size());
  if(memorySTA) SNES::sufamiturbo.slotA.rom.copy(memory.data(), memory.size());
  if(memorySTB) SNES::sufamiturbo.slotB.rom.copy(memory.data(), memory.size());
  SNES::cartridge.load(SNES::Cartridge::Mode::SufamiTurbo, markup);
  SNES::system.power();

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  SNES::video.generate_palette();
  return true;
}

bool InterfaceSNES::loadSuperGameBoyCartridge(string basename, string slotname) {
  interface->unloadCartridge();

  auto memory = loadCartridge(basename, interface->base);
  if(memory.empty()) return false;
  auto memoryGB = loadCartridge(slotname, interface->slot[0]);

  interface->game = memoryGB.empty() ? interface->base : interface->slot[0];
  interface->cartridgeTitle = interface->base.title();
  if(memoryGB) interface->cartridgeTitle = interface->slot[0].title();

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));
  if(markup.empty()) markup = SuperFamicomCartridge(memory.data(), memory.size()).markup;

  string gbMarkup;
  gbMarkup.readfile(interface->slot[0].filename("manifest.xml", ".xml"));
  if(gbMarkup.empty()) gbMarkup = GameBoyCartridge(memoryGB.data(), memoryGB.size()).markup;

  SNES::cartridge.rom.copy(memory.data(), memory.size());
  GB::cartridge.load(GB::System::Revision::SuperGameBoy, gbMarkup, vectorstream{memoryGB});
  SNES::cartridge.load(SNES::Cartridge::Mode::SuperGameBoy, markup);
  SNES::system.power();

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  SNES::video.generate_palette();
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
    if(memory.id == "save.ram") return interface->base.filename("save.ram", ".srm");
    if(memory.id == "rtc.ram") return interface->base.filename("rtc.ram", ".rtc");
    if(memory.id == "upd96050.ram") return interface->base.filename("upd96050.ram", ".nec");
    if(memory.id == "bsx.ram") return interface->base.filename("bsx.ram", ".bss");
    if(memory.id == "bsx.psram") return interface->base.filename("bsx.psram", ".bsp");
  }
  if(memory.slot == SNES::Cartridge::Slot::SufamiTurboA) {
    if(memory.id == "save.ram") return interface->slot[0].filename("save.ram", ".sts");
  }
  if(memory.slot == SNES::Cartridge::Slot::SufamiTurboB) {
    if(memory.id == "save.ram") return interface->slot[1].filename("save.ram", ".sts");
  }
  return "";
}

void InterfaceSNES::loadMemory() {
  for(auto &memory : SNES::cartridge.nvram) {
    if(memory.size == 0) continue;

    string filename = memoryName(memory);
    if(filename.empty()) continue;

    if(auto read = file::read(filename)) {
      memcpy(memory.data, read.data(), min(memory.size, read.size()));
    }
  }

  if(SNES::cartridge.mode() == SNES::Cartridge::Mode::SuperGameBoy) {
    if(GB::cartridge.ramsize) {
      if(auto read = file::read(interface->slot[0].filename("save.ram", ".sav"))) {
        memcpy(GB::cartridge.ramdata, read.data(), min(GB::cartridge.ramsize, read.size()));
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
    if(GB::cartridge.ramsize) {
      file::write(interface->slot[0].filename("save.ram", ".sav"),
        GB::cartridge.ramdata, GB::cartridge.ramsize
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
    GB::cheat.reset();
    for(auto &code : list) {
      lstring codelist;
      codelist.split("+", code);
      for(auto &part : codelist) {
        unsigned addr, data, comp;
        if(GB::Cheat::decode(part, addr, data, comp)) {
          GB::cheat.append({ addr, data, comp });
        }
      }
    }
    GB::cheat.synchronize();
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

uint32_t InterfaceSNES::videoColor(uint19_t source, uint16_t red, uint16_t green, uint16_t blue) {
  return color(red, green, blue);
}

void InterfaceSNES::videoRefresh(const uint32_t *data, bool hires, bool interlace, bool overscan) {
  unsigned width  =  256 << hires;
  unsigned height =  240 << interlace;
  unsigned pitch  = 1024 >> interlace;

  //skip first line; as it is always blank (by SNES design)
  if(overscan == false) data += 1 * 1024;  // 8 + 224 +  8
  if(overscan == true ) data += 9 * 1024;  // 0 + 240 +  0

  interface->videoRefresh(data, pitch * sizeof(uint32_t), width, height);
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
    if(hint == "uart.so") return { dir(interface->base.name), "uart.so" };
    if(hint == "usart.so") return { dir(interface->base.name), "usart.so" };
    if(hint.endswith(".rom")) return { dir(interface->base.name), hint };
  }
  return { dir(interface->base.name), hint };
}

void InterfaceSNES::message(const string &text) {
  MessageWindow::information(*mainWindow, text);
}
