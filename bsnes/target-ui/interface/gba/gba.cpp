void InterfaceGBA::initialize() {
  GBA::interface = this;
  GBA::system.init();
}

bool InterfaceGBA::cartridgeLoaded() {
  return GBA::cartridge.loaded();
}

bool InterfaceGBA::loadCartridge(const string &filename) {
  interface->unloadCartridge();

  uint8_t *biosdata;
  unsigned biossize;

  uint8_t *cartdata;
  unsigned cartsize;

  if(filename.endswith("/")) {
    if(!file::exists({filename, "bios.rom"})) {
      message("Error: Game Boy Advance BIOS (bios.bin) not found.");
      return false;
    }
    if(file::read({filename, "bios.rom"}, biosdata, biossize) == false) return false;
    if(file::read({filename, "program.rom"}, cartdata, cartsize) == false) return false;
    interface->base = {true, filename};
  } else {
    if(!file::exists({dir(filename), "gbabios.rom"})) {
      message("Error: Game Boy Advance BIOS (gbabios.bin) not found.");
      return false;
    }
    if(file::read({dir(filename), "gbabios.rom"}, biosdata, biossize) == false) return false;
    if(file::read(filename, cartdata, cartsize) == false) return false;
    interface->base = {false, filename};
  }

  interface->game = interface->base;
  interface->cartridgeTitle = interface->base.title();
  interface->applyPatch(interface->base, cartdata, cartsize);

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));

  GBA::system.bios.load(biosdata, biossize);
  GBA::cartridge.load(markup, cartdata, cartsize);
  GBA::system.power();
  delete[] biosdata;
  delete[] cartdata;

  GBA::video.generate(GBA::Video::Format::RGB30);
  interface->loadCartridge(::Interface::Mode::GBA);
  return true;
}

void InterfaceGBA::unloadCartridge() {
  GBA::system.unload();
}

void InterfaceGBA::power() {
  GBA::system.power();
}

void InterfaceGBA::reset() {
  GBA::system.power();  //GBA lacks reset button
}

void InterfaceGBA::run() {
  GBA::system.run();
}

serializer InterfaceGBA::serialize() {
  return serializer();
}

bool InterfaceGBA::unserialize(serializer &s) {
  return false;
}

void InterfaceGBA::setCheats(const lstring &list) {
}

//

void InterfaceGBA::videoRefresh(const uint16_t *data) {
  static uint32_t output[240 * 160];

  for(unsigned y = 0; y < 160; y++) {
    const uint16_t *sp = data + y * 240;
    uint32_t *dp = output + y * 240;
    for(unsigned x = 0; x < 240; x++) {
      uint16_t color = *sp++;
      *dp++ = GBA::video.palette[color];
    }
  }

  interface->videoRefresh(output, 240 * sizeof(uint32_t), 240, 160);
}

void InterfaceGBA::audioSample(int16_t lsample, int16_t rsample) {
  signed samples[] = { lsample, rsample };
  dspaudio.sample(samples);
  while(dspaudio.pending()) {
    dspaudio.read(samples);
    audio.sample(samples[0], samples[1]);
  }
}

bool InterfaceGBA::inputPoll(unsigned id) {
  return inputManager->gba.device.controller.poll(id);
}
