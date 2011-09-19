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

bool InterfaceSNES::loadCartridge(const string &filename) {
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) return false;

  interface->unloadCartridge();
  interface->baseName = nall::basename(filename);

  string xml = SNESCartridge(data, size).xmlMemoryMap;
  SNES::Interface::loadCartridge({ xml, data, size });
  delete[] data;

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

  string xml = SNESCartridge(data[0], size[0]).xmlMemoryMap;
  SNES::Interface::loadSatellaviewSlottedCartridge({ xml, data[0], size[0] }, { "", data[1], size[1] });
  delete[] data[0];
  if(data[1]) delete[] data[1];

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

  string xml = SNESCartridge(data[0], size[0]).xmlMemoryMap;
  SNES::Interface::loadSatellaviewCartridge({ xml, data[0], size[0] }, { "", data[1], size[1] });
  delete[] data[0];
  if(data[1]) delete[] data[1];

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

  string xml = SNESCartridge(data[0], size[0]).xmlMemoryMap;
  SNES::Interface::loadSufamiTurboCartridge({ xml, data[0], size[0] }, { "", data[1], size[1] }, { "", data[2], size[2] });
  delete[] data[0];
  if(data[1]) delete[] data[1];
  if(data[2]) delete[] data[2];

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

  string xml = SNESCartridge(data[0], size[0]).xmlMemoryMap;
  string gbXml = GameBoyCartridge(data[1], size[1]).xml;
  SNES::Interface::loadSuperGameBoyCartridge({ xml, data[0], size[0] }, { gbXml, data[1], size[1] });
  delete[] data[0];
  if(data[1]) delete[] data[1];

  interface->loadCartridge(::Interface::Mode::SNES);
  return true;
}

void InterfaceSNES::unloadCartridge() {
  SNES::Interface::unloadCartridge();
  interface->baseName = "";
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

void InterfaceSNES::videoRefresh(const uint16_t *data, bool hires, bool interlace, bool overscan) {
  interface->videoRefresh();

  unsigned width = hires ? 512 : 256;
  unsigned height = 0 ? 224 : 239;
  if(interlace) height <<= 1;
  unsigned inpitch = interlace ? 1024 : 2048;

  if(0) {  //NTSC
    if(overscan == false) data +=  9 * 1024;  // 0 + 224 +  0
    if(overscan == true ) data += 16 * 1024;  //-7 + 224 + -7
  }

  if(1) {  //PAL
    if(overscan == false) data +=  1 * 1024;  // 8 + 224 +  7
    if(overscan == true ) data +=  9 * 1024;  // 0 + 239 +  0
  }

  uint32_t *output;
  unsigned outpitch;
  if(video.lock(output, outpitch, width, height)) {
    for(unsigned y = 0; y < height; y++) {
      const uint16_t *sp = data + y * (inpitch >> 1);
      uint32_t *dp = output + y * (outpitch >> 2);
      for(unsigned x = 0; x < width; x++) {
        uint32_t color = *sp++;
        color = ((color & 0x7c00) << 9) | ((color & 0x03e0) << 6) | ((color & 0x001f) << 3);
        *dp++ = color | ((color >> 3) & 0x070707);
      }
    }

    video.unlock();
    video.refresh();
  }
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
  return dir(interface->baseName);
}
