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

  vector<uint8_t> memory;
  if(filename.endswith("/")) {
    memory = file::read({filename, "program.rom"});
    interface->base = {true, filename};
  } else {
    memory = file::read(filename);
    interface->base = {false, nall::basename(filename)};
  }
  if(memory.empty()) return false;

  interface->game = interface->base;
  interface->cartridgeTitle = interface->base.title();
  interface->applyPatch(interface->base, memory);

  string markup;
  markup.readfile(interface->base.filename("manifest.xml", ".xml"));
  if(markup.empty()) markup = GameBoyCartridge(memory.data(), memory.size()).markup;

  GB::cartridge.load(revision, markup, vectorstream{memory});
  GB::system.power();

  if(GB::cartridge.ramsize) {
    filestream fs{interface->base.filename("save.ram", ".sav")};
    fs.read(GB::cartridge.ramdata, min(GB::cartridge.ramsize, fs.size()));
  }

  GB::interface = this;
  GB::video.generate_palette();
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

uint32_t InterfaceGB::videoColor(unsigned source, uint16_t red, uint16_t green, uint16_t blue) {
  return color(red, green, blue);
}

void InterfaceGB::videoRefresh(const uint32_t *data, unsigned pitch, unsigned width, unsigned height) {
  interface->videoRefresh(data, pitch, width, height);
}

void InterfaceGB::audioSample(int16_t lsample, int16_t rsample) {
  signed samples[] = {lsample, rsample};
  dspaudio.sample(samples);
  while(dspaudio.pending()) {
    dspaudio.read(samples);
    audio.sample(samples[0], samples[1]);
  }
}

int16_t InterfaceGB::inputPoll(unsigned port, unsigned device, unsigned id) {
  return inputManager->gb.device.controller.poll(id);
}
