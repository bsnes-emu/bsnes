void InterfaceGameBoy::initialize() {
  GameBoy::interface = this;
  GameBoy::system.init();
}

bool InterfaceGameBoy::cartridgeLoaded() {
  return GameBoy::cartridge.loaded();
}

bool InterfaceGameBoy::loadCartridge(GameBoy::System::Revision revision, const string &filename) {
  uint8_t *data;
  unsigned size;
  if(interface->loadFile(filename, data, size) == false) return false;

  interface->unloadCartridge();
  interface->baseName = nall::basename(filename);

  GameBoyCartridge info(data, size);
  GameBoy::cartridge.load(revision, info.markup, data, size);
  GameBoy::system.power();
  delete[] data;

  if(GameBoy::cartridge.ramsize) {
    filemap fp;
    if(fp.open(string{ interface->baseName, ".sav" }, filemap::mode::read)) {
      memcpy(GameBoy::cartridge.ramdata, fp.data(), min(GameBoy::cartridge.ramsize, fp.size()));
    }
  }

  GameBoy::interface = this;
  GameBoy::video.generate(GameBoy::Video::Format::RGB24);
  interface->loadCartridge(::Interface::Mode::GameBoy);
  return true;
}

void InterfaceGameBoy::unloadCartridge() {
  if(GameBoy::cartridge.ramsize) {
    file::write({ interface->baseName, ".sav" }, GameBoy::cartridge.ramdata, GameBoy::cartridge.ramsize);
  }

  GameBoy::cartridge.unload();
  interface->baseName = "";
}

void InterfaceGameBoy::power() {
  GameBoy::system.power();
}

void InterfaceGameBoy::reset() {
  GameBoy::system.power();  //Game Boy lacks reset button
}

void InterfaceGameBoy::run() {
  do {
    GameBoy::system.run();
  } while(GameBoy::scheduler.exit_reason() != GameBoy::Scheduler::ExitReason::FrameEvent);
}

serializer InterfaceGameBoy::serialize() {
  GameBoy::system.runtosave();
  return GameBoy::system.serialize();
}

bool InterfaceGameBoy::unserialize(serializer &s) {
  return GameBoy::system.unserialize(s);
}

void InterfaceGameBoy::setCheats(const lstring &list) {
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
}

//

void InterfaceGameBoy::videoRefresh(const uint16_t *data) {
  static uint32_t output[160 * 144];

  for(unsigned y = 0; y < 144; y++) {
    const uint16_t *sp = data + y * 160;
    uint32_t *dp = output + y * 160;
    for(unsigned x = 0; x < 160; x++) {
      uint16_t color = *sp++;
      *dp++ = GameBoy::video.palette[color];
    }
  }

  interface->videoRefresh(output, 160 * 4, 160, 144);
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
