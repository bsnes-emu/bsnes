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
  filemap fp;
  if(fp.open(filename, filemap::mode::read) == false) return false;

  interface->unloadCartridge();
  interface->baseName = nall::basename(filename);

  string markup;
  markup.readfile({ interface->baseName, ".bml" });

  NES::Interface::loadCartridge(markup, fp.data(), fp.size());
  fp.close();

  if(NES::Interface::memorySize(NES::Interface::Memory::RAM) > 0) {
    if(fp.open(string{ interface->baseName, ".sav" }, filemap::mode::read)) {
      memcpy(NES::Interface::memoryData(NES::Interface::Memory::RAM), fp.data(),
        min(NES::Interface::memorySize(NES::Interface::Memory::RAM), fp.size())
      );
    }
  }

  interface->loadCartridge(::Interface::Mode::NES);
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
  static uint16_t output[256 * 240];

  unsigned height = 240;
  if(config->video.enableOverscan == false) {
    height = 224;
    data += 8 * 256;
  }

  for(unsigned y = 0; y < height; y++) {
    const uint16_t *sp = data + y * 256;
    uint16_t *dp = output + y * 256;
    for(unsigned x = 0; x < 256; x++) {
      uint32_t color = palette[*sp++];
      *dp++ = ((color & 0xf80000) >> 9) | ((color & 0x00f800) >> 6) | ((color & 0x0000f8) >> 3);;
    }
  }

  interface->videoRefresh(output, 256 * 2, 256, height);
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

InterfaceNES::InterfaceNES() {
  unsigned base_palette[] = {
    0x7c7c7c, 0x0000fc, 0x0000bc, 0x4428bc,
    0x940084, 0xa80020, 0xa81000, 0x881400,
    0x503000, 0x007800, 0x006800, 0x005800,
    0x004058, 0x000000, 0x000000, 0x000000,
    0xbcbcbc, 0x0078f8, 0x0058f8, 0x6844fc,
    0xd800cc, 0xe40058, 0xf83800, 0xe45c10,
    0xac7c00, 0x00b800, 0x00a800, 0x00a844,
    0x008888, 0x000000, 0x000000, 0x000000,
    0xf8f8f8, 0x3cbcfc, 0x6888fc, 0x9878f8,
    0xf878f8, 0xf85898, 0xf87858, 0xfca044,
    0xf8b800, 0xb8f818, 0x58d854, 0x58f898,
    0x00e8d8, 0x787878, 0x000000, 0x000000,
    0xfcfcfc, 0xa4e4fc, 0xb8b8b8, 0xd8d8f8,
    0xf8b8f8, 0xf8a4c0, 0xf0d0b0, 0xfce0a8,
    0xf8d878, 0xd8f878, 0xb8f8b8, 0xb8f8d8,
    0x00fcfc, 0xf8d8f8, 0x000000, 0x000000,
  };
  memcpy(palette, base_palette, sizeof base_palette);

  for(unsigned e = 1; e < 8; e++) {
    static const double rfactor[8] = { 1.000, 1.239, 0.794, 1.019, 0.905, 1.023, 0.741, 0.750 };
    static const double gfactor[8] = { 1.000, 0.915, 1.086, 0.980, 1.026, 0.908, 0.987, 0.750 };
    static const double bfactor[8] = { 1.000, 0.743, 0.882, 0.653, 1.277, 0.979, 0.101, 0.750 };
    for(unsigned n = 0; n < 64; n++) {
      unsigned c = palette[n];
      uint8_t r = c >> 16, g = c >> 8, b = c >> 0;
      r = uclamp<8>((unsigned)(r * rfactor[e]));
      g = uclamp<8>((unsigned)(g * gfactor[e]));
      b = uclamp<8>((unsigned)(b * bfactor[e]));
      palette[e * 64 + n] = (r << 16) | (g << 8) | (b << 0);
    }
  }
}
