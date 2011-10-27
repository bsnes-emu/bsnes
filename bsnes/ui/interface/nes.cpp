void InterfaceNES::setController(bool port, unsigned device) {
  if(port == 0) config->nes.controllerPort1Device = device;
  if(port == 1) config->nes.controllerPort2Device = device;

  if(port == 0) switch(device) {
  case 0: return connect(0, NES::Input::Device::None);
  case 1: return connect(0, NES::Input::Device::Joypad);
  }

  if(port == 1) switch(device) {
  case 0: return connect(1, NES::Input::Device::None);
  case 1: return connect(1, NES::Input::Device::Joypad);
  }
}

bool InterfaceNES::loadCartridge(const string &filename) {
  uint8_t *data;
  unsigned size;
  if(interface->loadFile(filename, data, size) == false) return false;

  interface->unloadCartridge();
  interface->baseName = nall::basename(filename);

  string markup;
  markup.readfile({ interface->baseName, ".bml" });

  NES::Interface::loadCartridge(markup, data, size);
  delete[] data;

  if(NES::Interface::memorySize(NES::Interface::Memory::RAM) > 0) {
    filemap fp;
    if(fp.open(string{ interface->baseName, ".sav" }, filemap::mode::read)) {
      memcpy(NES::Interface::memoryData(NES::Interface::Memory::RAM), fp.data(),
        min(NES::Interface::memorySize(NES::Interface::Memory::RAM), fp.size())
      );
    }
  }

  interface->loadCartridge(::Interface::Mode::NES);
  NES::video.generate(NES::Video::Format::RGB24);
  return true;
}

void InterfaceNES::unloadCartridge() {
  if(NES::Interface::memorySize(NES::Interface::Memory::RAM) > 0) {
    file::write({ interface->baseName, ".sav" },
      NES::Interface::memoryData(NES::Interface::Memory::RAM),
      NES::Interface::memorySize(NES::Interface::Memory::RAM)
    );
  }

  NES::Interface::unloadCartridge();
  interface->baseName = "";
}

//

bool InterfaceNES::saveState(const string &filename) {
  serializer s = serialize();
  return file::write(filename, s.data(), s.size());
}

bool InterfaceNES::loadState(const string &filename) {
  uint8_t *data;
  unsigned size;
  if(file::read(filename, data, size) == false) return false;
  serializer s(data, size);
  delete[] data;
  return unserialize(s);
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
        memset(dp, 0, 256 * 2);
      } else {
        memset(dp + 0, 0, osw * 2);
        memset(dp + 256 - osw, 0, osw * 2);
      }
    }
  }

  interface->videoRefresh(output, 256 * 4, 256, 240);
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
