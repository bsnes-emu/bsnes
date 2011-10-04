bool InterfaceGameBoy::loadCartridge(const string &filename) {
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) return false;

  interface->unloadCartridge();
  interface->baseName = nall::basename(filename);

  GameBoyCartridge info(data, size);
  GameBoy::Interface::loadCartridge(info.markup, data, size);
  delete[] data;

  if(GameBoy::Interface::memorySize(GameBoy::Interface::Memory::RAM) > 0) {
    filemap fp;
    if(fp.open(string{ interface->baseName, ".sav" }, filemap::mode::read)) {
      memcpy(GameBoy::Interface::memoryData(GameBoy::Interface::Memory::RAM), fp.data(),
        min(GameBoy::Interface::memorySize(GameBoy::Interface::Memory::RAM), fp.size())
      );
    }
  }

  interface->loadCartridge(::Interface::Mode::GameBoy);
  return true;
}

void InterfaceGameBoy::unloadCartridge() {
  if(GameBoy::Interface::memorySize(GameBoy::Interface::Memory::RAM) > 0) {
    file::write({ interface->baseName, ".sav" },
      GameBoy::Interface::memoryData(GameBoy::Interface::Memory::RAM),
      GameBoy::Interface::memorySize(GameBoy::Interface::Memory::RAM)
    );
  }

  GameBoy::Interface::unloadCartridge();
  interface->baseName = "";
}

bool InterfaceGameBoy::saveState(const string &filename) {
  serializer s = serialize();
  return file::write(filename, s.data(), s.size());
}

bool InterfaceGameBoy::loadState(const string &filename) {
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) return false;
  serializer s(data, size);
  delete[] data;
  return unserialize(s);
}

//

void InterfaceGameBoy::videoRefresh(const uint8_t *data) {
  static uint16_t output[160 * 144];
  static uint32_t palette[] = {
    0x9bbc0f, 0x8bac0f, 0x306230, 0x0f380f
  };

  for(unsigned y = 0; y < 144; y++) {
    const uint8_t *sp = data + y * 160;
    uint16_t *dp = output + y * 160;
    for(unsigned x = 0; x < 160; x++) {
      uint32_t color = palette[*sp++];
      *dp++ = ((color & 0xf80000) >> 9) | ((color & 0x00f800) >> 6) | ((color & 0x0000f8) >> 3);
    }
  }

  interface->videoRefresh(output, 160 * 2, 160, 144);
}

void InterfaceGameBoy::audioSample(int16_t csample, int16_t lsample, int16_t rsample) {
  signed samples[] = { lsample, rsample };
  dspaudio.sample(samples);
  while(dspaudio.pending()) {
    dspaudio.read(samples);
    audio.sample(samples[0], samples[1]);
  }
}

bool InterfaceGameBoy::inputPoll(unsigned id) {
  return inputManager->gameBoy.device.controller.poll(id);
}
