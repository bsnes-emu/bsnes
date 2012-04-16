void InterfaceGBA::initialize() {
  string filename = application->path("Game Boy Advance.system/manifest.xml");
  string markup;
  markup.readfile(filename);
  XML::Document document(markup);

  if(document["system"]["bios"].exists()) {
    auto &bios = document["system"]["bios"];
    string firmware = bios["firmware"].data;
    string hash = bios["sha256"].data;

    uint8_t *data;
    unsigned size;
    if(file::read({dir(filename),firmware}, data, size) == true) {
      if(nall::sha256(data, size) == hash) {
        GBA::bios.load(data, size);
      } else {
        MessageWindow::information(Window::None, "Warning: GBA BIOS SHA256 sum is incorrect.");
      }
    }
  }

  GBA::interface = this;
  GBA::system.init();
}

bool InterfaceGBA::cartridgeLoaded() {
  return GBA::cartridge.loaded();
}

bool InterfaceGBA::loadCartridge(const string &filename) {
  interface->unloadCartridge();

  uint8_t *data;
  unsigned size;

  if(filename.endswith("/")) {
    if(file::read({filename, "program.rom"}, data, size) == false) return false;
    interface->base = {true, filename};
  } else {
    if(file::read(filename, data, size) == false) return false;
    interface->base = {false, filename};
  }

  interface->game = interface->base;
  interface->cartridgeTitle = interface->base.title();
  interface->applyPatch(interface->base, data, size);

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));

  GBA::cartridge.load(markup, data, size);
  GBA::system.power();
  delete[] data;

  if(GBA::cartridge.ram_size()) {
    filemap fp;
    if(fp.open(interface->base.filename("save.ram", ".sav"), filemap::mode::read)) {
      memcpy(GBA::cartridge.ram_data(), fp.data(), min(GBA::cartridge.ram_size(), fp.size()));
    }
  }

  GBA::video.generate(GBA::Video::Format::RGB30);
  interface->loadCartridge(::Interface::Mode::GBA);
  return true;
}

void InterfaceGBA::unloadCartridge() {
  if(GBA::cartridge.ram_size()) {
    file::write(interface->base.filename("save.ram", ".sav"), GBA::cartridge.ram_data(), GBA::cartridge.ram_size());
  }

  GBA::cartridge.unload();
  interface->base.name = "";
}

void InterfaceGBA::power() {
  return GBA::system.power();
}

void InterfaceGBA::reset() {
  return GBA::system.power();  //GBA has no reset button
}

void InterfaceGBA::run() {
  return GBA::system.run();
}

serializer InterfaceGBA::serialize() {
  GBA::system.runtosave();
  return GBA::system.serialize();
}

bool InterfaceGBA::unserialize(serializer &s) {
  return GBA::system.unserialize(s);
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
