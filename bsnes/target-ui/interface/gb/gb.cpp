void InterfaceGB::initialize() {
  loadFirmware("Game Boy.sys/manifest.xml",       "system.boot",    GB::system.bootROM.dmg,  256u);
  loadFirmware("Super Game Boy.sfc/manifest.xml", "cartridge.boot", GB::system.bootROM.sgb,  256u);
  loadFirmware("Game Boy Color.sys/manifest.xml", "system.boot",    GB::system.bootROM.cgb, 2048u);

  GB::interface = this;
  GB::system.init();
}

string InterfaceGB::markup() {
  return GB::cartridge.information.markup;
}

bool InterfaceGB::cartridgeLoaded() {
  return GB::cartridge.loaded();
}

bool InterfaceGB::loadCartridge(GB::System::Revision revision, const string &filename) {
  interface->unloadCartridge();

  uint8_t *data;
  unsigned size;

  if(filename.endswith("/")) {
    if(file::read({ filename, "program.rom" }, data, size) == false) return false;
    interface->base = { true, filename };
  } else {
    if(file::read(filename, data, size) == false) return false;
    interface->base = { false, nall::basename(filename) };
  }

  interface->game = interface->base;
  interface->cartridgeTitle = interface->base.title();
  interface->applyPatch(interface->base, data, size);

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));
  if(markup.empty()) markup = GameBoyCartridge(data, size).markup;

  GB::cartridge.load(revision, markup, data, size);
  GB::system.power();
  delete[] data;

  if(GB::cartridge.ramsize) {
    filemap fp;
    if(fp.open(interface->base.filename("save.ram", ".sav"), filemap::mode::read)) {
      memcpy(GB::cartridge.ramdata, fp.data(), min(GB::cartridge.ramsize, fp.size()));
    }
  }

  GB::interface = this;
  GB::video.generate(GB::Video::Format::RGB30);
  interface->loadCartridge(::Interface::Mode::GB);
  return true;
}

void InterfaceGB::unloadCartridge() {
  if(GB::cartridge.ramsize) {
    file::write(interface->base.filename("save.ram", ".sav"), GB::cartridge.ramdata, GB::cartridge.ramsize);
  }

  GB::cartridge.unload();
  interface->base.name = "";
}

void InterfaceGB::power() {
  GB::system.power();
}

void InterfaceGB::reset() {
  GB::system.power();  //Game Boy lacks reset button
}

void InterfaceGB::run() {
  do {
    GB::system.run();
  } while(GB::scheduler.exit_reason() != GB::Scheduler::ExitReason::FrameEvent);
}

serializer InterfaceGB::serialize() {
  GB::system.runtosave();
  return GB::system.serialize();
}

bool InterfaceGB::unserialize(serializer &s) {
  return GB::system.unserialize(s);
}

void InterfaceGB::setCheats(const lstring &list) {
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
}

//

void InterfaceGB::videoRefresh(const uint16_t *data) {
  static uint32_t output[160 * 144];

  for(unsigned y = 0; y < 144; y++) {
    const uint16_t *sp = data + y * 160;
    uint32_t *dp = output + y * 160;
    for(unsigned x = 0; x < 160; x++) {
      uint16_t color = *sp++;
      *dp++ = GB::video.palette[color];
    }
  }

  interface->videoRefresh(output, 160 * sizeof(uint32_t), 160, 144);
}

void InterfaceGB::audioSample(int16_t csample, int16_t lsample, int16_t rsample) {
  signed samples[] = { lsample, rsample };
  dspaudio.sample(samples);
  while(dspaudio.pending()) {
    dspaudio.read(samples);
    audio.sample(samples[0], samples[1]);
  }
}

bool InterfaceGB::inputPoll(unsigned id) {
  return inputManager->gb.device.controller.poll(id);
}
