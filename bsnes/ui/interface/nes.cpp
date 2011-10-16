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

  for(unsigned y = 0; y < 240; y++) {
    const uint16_t *sp = data + y * 256;
    uint16_t *dp = output + y * 256;
    for(unsigned x = 0; x < 256; x++) {
      uint32_t color = palette[*sp++];
      *dp++ = ((color & 0xf80000) >> 9) | ((color & 0x00f800) >> 6) | ((color & 0x0000f8) >> 3);;
    }
  }

  if(config->video.maskOverscan) {
    unsigned osw = config->video.maskOverscanHorizontal;
    unsigned osh = config->video.maskOverscanVertical;

    for(unsigned y = 0; y < 240; y++) {
      uint16_t *dp = output + y * 256;
      if(y < osh || y >= 240 - osh) {
        memset(dp, 0, 256 * 2);
      } else {
        memset(dp + 0, 0, osw * 2);
        memset(dp + 256 - osw, 0, osw * 2);
      }
    }
  }

  interface->videoRefresh(output, 256 * 2, 256, 240);
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

//

//n = BGRCCCCCC (BGR=emphasis bits; C=color)
unsigned InterfaceNES::paletteColor(
  unsigned n, double saturation, double hue,
  double contrast, double brightness, double gamma
) {
  signed color = (n & 0x0f), level = color < 0xe ? (n >> 4) & 3 : 1;

  static const double black = 0.518, white = 1.962, attenuation = 0.746;
  static const double levels[8] = {
    0.350, 0.518, 0.962, 1.550,
    1.094, 1.506, 1.962, 1.962,
  };

  double lo_and_hi[2] = {
    levels[level + 4 * (color == 0x0)],
    levels[level + 4 * (color <  0xd)],
  };

  double y = 0.0, i = 0.0, q = 0.0;
  auto wave = [](signed p, signed color) { return (color + p + 8) % 12 < 6; };
  for(signed p = 0; p < 12; p++) {
    double spot = lo_and_hi[wave(p, color)];

    if(((n & 0x040) && wave(p, 12))
    || ((n & 0x080) && wave(p,  4))
    || ((n & 0x100) && wave(p,  8))
    ) spot *= attenuation;

    double v = (spot - black) / (white - black);

    v = (v - 0.5) * contrast + 0.5;
    v *= brightness / 12.0;

    y += v;
    i += v * std::cos((3.141592653 / 6.0) * (p + hue));
    q += v * std::sin((3.141592653 / 6.0) * (p + hue));
  }

  i *= saturation;
  q *= saturation;

  auto gammaAdjust = [=](double f) { return f < 0.0 ? 0.0 : std::pow(f, 2.2 / gamma); };
  return (uclamp<8>(255.0 * gammaAdjust(y +  0.946882 * i +  0.623557 * q)) << 16)
       + (uclamp<8>(255.0 * gammaAdjust(y + -0.274788 * i + -0.635691 * q)) <<  8)
       + (uclamp<8>(255.0 * gammaAdjust(y + -1.108545 * i +  1.709007 * q)) <<  0);
}

InterfaceNES::InterfaceNES() {
  for(unsigned n = 0; n < 512; n++) {
    palette[n] = paletteColor(n, 2.0);
  }

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
