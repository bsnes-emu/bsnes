void InterfaceSNES::setController(bool port, unsigned device) {
  if(port == 0) config->snes.controllerPort1Device = device;
  if(port == 1) config->snes.controllerPort2Device = device;

  if(port == 0) switch(device) {
  case 0: return connect(0, SNES::Input::Device::None);
  case 1: return connect(0, SNES::Input::Device::Joypad);
  case 2: return connect(0, SNES::Input::Device::Multitap);
  case 3: return connect(0, SNES::Input::Device::Mouse);
  }

  if(port == 1) switch(device) {
  case 0: return connect(1, SNES::Input::Device::None);
  case 1: return connect(1, SNES::Input::Device::Joypad);
  case 2: return connect(1, SNES::Input::Device::Multitap);
  case 3: return connect(1, SNES::Input::Device::Mouse);
  case 4: return connect(1, SNES::Input::Device::SuperScope);
  case 5: return connect(1, SNES::Input::Device::Justifier);
  case 6: return connect(1, SNES::Input::Device::Justifiers);
  case 7: return connect(1, SNES::Input::Device::Serial);
  }
}

bool InterfaceSNES::loadCartridge(const string &basename) {
  uint8_t *data;
  unsigned size;
  if(file::read(basename, data, size) == false) return false;

  interface->unloadCartridge();
  interface->baseName = nall::basename(basename);
  interface->slotName = { nall::basename(basename) };

  string xml;
  xml.readfile({ interface->baseName, ".xml" });
  if(xml == "") xml = SNESCartridge(data, size).xmlMemoryMap;

  SNES::Interface::loadCartridge({ xml, data, size });
  delete[] data;

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  return true;
}

bool InterfaceSNES::loadSatellaviewSlottedCartridge(const string &basename, const string &slotname) {
  uint8_t *data[2];
  unsigned size[2];
  if(file::read(basename, data[0], size[0]) == false) return false;
  file::read(slotname, data[1], size[1]);

  interface->unloadCartridge();
  interface->baseName = nall::basename(basename);
  if(data[1]) interface->baseName.append("+", nall::basename(notdir(slotname)));
  interface->slotName = { nall::basename(basename), nall::basename(slotname) };

  string xml;
  xml.readfile({ interface->baseName, ".xml" });
  if(xml == "") xml = SNESCartridge(data[0], size[0]).xmlMemoryMap;

  SNES::Interface::loadSatellaviewSlottedCartridge({ xml, data[0], size[0] }, { "", data[1], size[1] });
  delete[] data[0];
  if(data[1]) delete[] data[1];

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  return true;
}

bool InterfaceSNES::loadSatellaviewCartridge(const string &basename, const string &slotname) {
  uint8_t *data[2];
  unsigned size[2];
  if(file::read(basename, data[0], size[0]) == false) return false;
  file::read(slotname, data[1], size[1]);

  interface->unloadCartridge();
  interface->baseName = nall::basename(basename);
  if(data[1]) interface->baseName.append("+", nall::basename(notdir(slotname)));
  interface->slotName = { nall::basename(basename), nall::basename(slotname) };

  string xml;
  xml.readfile({ interface->baseName, ".xml" });
  if(xml == "") xml = SNESCartridge(data[0], size[0]).xmlMemoryMap;

  SNES::Interface::loadSatellaviewCartridge({ xml, data[0], size[0] }, { "", data[1], size[1] });
  delete[] data[0];
  if(data[1]) delete[] data[1];

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  return true;
}

bool InterfaceSNES::loadSufamiTurboCartridge(const string &basename, const string &slotAname, const string &slotBname) {
  uint8_t *data[3];
  unsigned size[3];
  if(file::read(basename, data[0], size[0]) == false) return false;
  file::read(slotAname, data[1], size[1]);
  file::read(slotBname, data[2], size[2]);

  interface->unloadCartridge();
  interface->baseName = nall::basename(basename);
  if(data[1] && data[2]) interface->baseName = { nall::basename(slotAname), "+", nall::basename(notdir(slotBname)) };
  else if(data[1]) interface->baseName = nall::basename(slotAname);
  else if(data[2]) interface->baseName = nall::basename(slotBname);
  interface->slotName = { nall::basename(basename), nall::basename(slotAname), nall::basename(slotBname) };

  string xml;
  xml.readfile({ interface->baseName, ".xml" });
  if(xml == "") xml = SNESCartridge(data[0], size[0]).xmlMemoryMap;

  SNES::Interface::loadSufamiTurboCartridge({ xml, data[0], size[0] }, { "", data[1], size[1] }, { "", data[2], size[2] });
  delete[] data[0];
  if(data[1]) delete[] data[1];
  if(data[2]) delete[] data[2];

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  return true;
}

bool InterfaceSNES::loadSuperGameBoyCartridge(const string &basename, const string &slotname) {
  uint8_t *data[2];
  unsigned size[2];
  if(file::read(basename, data[0], size[0]) == false) return false;
  file::read(slotname, data[1], size[1]);

  interface->unloadCartridge();
  interface->baseName = nall::basename(basename);
  if(data[1]) interface->baseName = nall::basename(slotname);
  interface->slotName = { nall::basename(basename), nall::basename(slotname) };

  string xml;
  xml.readfile({ interface->baseName, ".xml" });
  if(xml == "") xml = SNESCartridge(data[0], size[0]).xmlMemoryMap;

  string gbXml;
  gbXml.readfile({ nall::basename(slotname), ".xml" });
  if(gbXml == "") gbXml = GameBoyCartridge(data[1], size[1]).xml;
  SNES::Interface::loadSuperGameBoyCartridge({ xml, data[0], size[0] }, { gbXml, data[1], size[1] });
  delete[] data[0];
  if(data[1]) delete[] data[1];

  loadMemory();
  interface->loadCartridge(::Interface::Mode::SNES);
  return true;
}

void InterfaceSNES::unloadCartridge() {
  saveMemory();
  SNES::Interface::unloadCartridge();
  interface->baseName = "";
}

//slot[] array = Cartridge::Slot to slot# conversion:
//{ Base, Bsx, SufamiTurbo, SufamiTurboA, SufamiTurboB, GameBoy }

void InterfaceSNES::loadMemory() {
  static unsigned slot[] = { 0, 0, 0, 1, 2, 1 };
  for(auto &memory : SNES::Interface::memory()) {
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
  for(auto &memory : SNES::Interface::memory()) {
    if(memory.size == 0) continue;
    string filename = { interface->slotName[slot[(unsigned)memory.slot]], memory.id };
    file::write(filename, memory.data, memory.size);
  }
}

bool InterfaceSNES::saveState(const string &filename) {
  serializer s = serialize();
  return file::write(filename, s.data(), s.size());
}

bool InterfaceSNES::loadState(const string &filename) {
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) return false;
  serializer s(data, size);
  delete[] data;
  return unserialize(s);
}

//

void InterfaceSNES::videoRefresh(const uint32_t *data, bool hires, bool interlace, bool overscan) {
  static uint16_t output[512 * 478];

  unsigned width = 256 << hires;
  unsigned height = (config->video.enableOverscan ? 240 : 224) << interlace;
  unsigned pitch = 1024 >> interlace;

  //data[] = scanline { 8 (blank) + 240 (video) + 8 (blank) }
  //first line of video data is not rendered (effectively blank as well)
  if(config->video.enableOverscan) {
    if(overscan == false) data +=  1 * 1024;  // 8 + 224 +  8
    if(overscan == true ) data +=  9 * 1024;  // 0 + 240 +  0
  } else {
    if(overscan == false) data +=  9 * 1024;  // 0 + 224 +  0
    if(overscan == true ) data += 16 * 1024;  //-8 + 224 + -8
  }

  for(unsigned y = 0; y < height; y++) {
    const uint32_t *sp = data + y * pitch;
    uint16_t *dp = output + y * 512;
    for(unsigned x = 0; x < width; x++) {
      *dp++ = palette[*sp++];
    }
  }

  interface->videoRefresh(output, 512 * 2, width, height);
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

InterfaceSNES::InterfaceSNES() {
  //{llll bbbbb ggggg rrrrr} -> { rrrrr ggggg bbbbb }
  for(unsigned l = 0; l < 16; l++) {
    for(unsigned r = 0; r < 32; r++) {
      for(unsigned g = 0; g < 32; g++) {
        for(unsigned b = 0; b < 32; b++) {
          double luma = (double)l / 15.0;
          unsigned ar = (luma * r + 0.5);
          unsigned ag = (luma * g + 0.5);
          unsigned ab = (luma * b + 0.5);
          palette[(l << 15) + (r << 10) + (g << 5) + (b << 0)] = (ab << 10) + (ag << 5) + (ar << 0);
        }
      }
    }
  }
}
