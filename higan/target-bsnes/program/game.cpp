auto Program::load() -> void {
  unload();

  for(auto& media : emulator->media) {
    if(media.type != "sfc") continue;

    Emulator::audio.reset(2, audio->frequency());
    if(emulator->load(media.id)) {
      gameQueue = {};
      connectDevices();
      emulator->power();
      presentation->setTitle(emulator->title());
      presentation->resetSystem.setEnabled(true);
      presentation->unloadGame.setEnabled(true);
      presentation->saveState.setEnabled(true);
      presentation->loadState.setEnabled(true);
      presentation->resizeViewport();

      string locations = superNintendo.location;
      if(auto location = gameBoy.location) locations.append("|", location);
      presentation->addRecentGame(locations);
    }

    break;
  }
}

auto Program::loadFile(string location) -> vector<uint8_t> {
  if(Location::suffix(location) == ".zip") {
    Decode::ZIP archive;
    if(archive.open(location)) {
      for(auto& file : archive.file) {
        auto type = Location::suffix(file.name);
        if(type == ".sfc" || type == ".smc" || type == ".gb" || type == ".gbc" || type == ".bs" || type == ".st") {
          return archive.extract(file);
        }
      }
    }
  } else {
    return file::read(location);
  }
}

auto Program::loadSuperNintendo(string location) -> void {
  auto rom = loadFile(location);
  if(!rom) return;

  //Heuristics::SuperFamicom() call will remove copier header from rom if present
  auto heuristics = Heuristics::SuperFamicom(rom, location);
  superNintendo.manifest = heuristics.manifest();
  superNintendo.document = BML::unserialize(superNintendo.manifest);
  superNintendo.location = location;

  uint offset = 0;
  if(auto size = heuristics.programRomSize()) {
    superNintendo.program.resize(size);
    memory::copy(&superNintendo.program[0], &rom[offset], size);
    offset += size;
  }
  if(auto size = heuristics.dataRomSize()) {
    superNintendo.data.resize(size);
    memory::copy(&superNintendo.data[0], &rom[offset], size);
    offset += size;
  }
  if(auto size = heuristics.expansionRomSize()) {
    superNintendo.expansion.resize(size);
    memory::copy(&superNintendo.expansion[0], &rom[offset], size);
    offset += size;
  }
  if(auto size = heuristics.firmwareRomSize()) {
    superNintendo.firmware.resize(size);
    memory::copy(&superNintendo.firmware[0], &rom[offset], size);
    offset += size;
  }
}

auto Program::loadGameBoy(string location) -> void {
  auto rom = loadFile(location);
  if(!rom) return;

  auto heuristics = Heuristics::GameBoy(rom, location);
  gameBoy.manifest = heuristics.manifest();
  gameBoy.document = BML::unserialize(gameBoy.manifest);
  gameBoy.location = location;

  gameBoy.program = rom;
}

auto Program::save() -> void {
  if(!emulator->loaded()) return;
  emulator->save();
}

auto Program::unload() -> void {
  if(!emulator->loaded()) return;
  emulator->unload();
  superNintendo = {};
  gameBoy = {};
  bsMemory = {};
  sufamiTurbo[0] = {};
  sufamiTurbo[1] = {};
  presentation->setTitle({"bsnes v", Emulator::Version});
  presentation->resetSystem.setEnabled(false);
  presentation->unloadGame.setEnabled(false);
  presentation->saveState.setEnabled(false);
  presentation->loadState.setEnabled(false);
  presentation->clearViewport();
}
