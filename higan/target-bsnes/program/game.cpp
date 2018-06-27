auto Program::load() -> void {
  unload();

  for(auto& media : emulator->media) {
    if(media.type != "sfc") continue;

    Emulator::audio.reset(2, audio->frequency());
    if(emulator->load(media.id)) {
      gameQueue = {};
      updateInputDevices();
      applyHacks();
      emulator->power();
      updateVideoPalette();
      updateAudioEffects();
      showMessage(!appliedPatch() ? "Game loaded" : "Game loaded and patch applied");
      presentation->setTitle(emulator->title());
      presentation->resetSystem.setEnabled(true);
      presentation->unloadGame.setEnabled(true);
      presentation->toolsMenu.setVisible(true);
      presentation->pauseEmulation.setChecked(false);
      presentation->resizeViewport();
      toolsWindow->cheatEditor.loadCheats();
      toolsWindow->stateManager.loadStates();

      string locations = superFamicom.location;
      if(auto location = gameBoy.location) locations.append("|", location);
      if(auto location = bsMemory.location) locations.append("|", location);
      if(auto location = sufamiTurboA.location) locations.append("|", location);
      if(auto location = sufamiTurboB.location) locations.append("|", location);
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

auto Program::loadSuperFamicom(string location) -> void {
  string manifest;
  vector<uint8_t> rom;

  if(location.endsWith("/")) {
    manifest = file::read({location, "manifest.bml"});
    rom.append(file::read({location, "program.rom"}));
    rom.append(file::read({location, "data.rom"}));
    rom.append(file::read({location, "expansion.rom"}));
    for(auto filename : directory::files(location, "*.boot.rom"   )) rom.append(file::read({location, filename}));
    for(auto filename : directory::files(location, "*.program.rom")) rom.append(file::read({location, filename}));
    for(auto filename : directory::files(location, "*.data.rom"   )) rom.append(file::read({location, filename}));
  } else {
    manifest = file::read({Location::notsuffix(location), ".bml"});
    rom = loadFile(location);
  }

  //assume ROM and IPS agree on whether a copier header is present
  superFamicom.patched = applyPatchIPS(rom, location);
  if((rom.size() & 0x7fff) == 512) {
    //remove copier header
    memory::move(&rom[0], &rom[512], rom.size() - 512);
    rom.resize(rom.size() - 512);
  }
  //assume BPS is made against a ROM without a copier header
  if(!superFamicom.patched) superFamicom.patched = applyPatchBPS(rom, location);
  auto heuristics = Heuristics::SuperFamicom(rom, location);
  auto sha256 = Hash::SHA256(rom).digest();
  if(auto document = BML::unserialize(string::read(locate("database/Super Famicom.bml")))) {
    if(auto game = document[{"game(sha256=", sha256, ")"}]) {
      manifest = BML::serialize(game);
    }
  }
  superFamicom.label = heuristics.label();
  superFamicom.manifest = manifest ? manifest : heuristics.manifest();
  applyHackOverclockSuperFX();
  superFamicom.document = BML::unserialize(superFamicom.manifest);
  superFamicom.location = location;

  uint offset = 0;
  if(auto size = heuristics.programRomSize()) {
    superFamicom.program.resize(size);
    memory::copy(&superFamicom.program[0], &rom[offset], size);
    offset += size;
  }
  if(auto size = heuristics.dataRomSize()) {
    superFamicom.data.resize(size);
    memory::copy(&superFamicom.data[0], &rom[offset], size);
    offset += size;
  }
  if(auto size = heuristics.expansionRomSize()) {
    superFamicom.expansion.resize(size);
    memory::copy(&superFamicom.expansion[0], &rom[offset], size);
    offset += size;
  }
  if(auto size = heuristics.firmwareRomSize()) {
    superFamicom.firmware.resize(size);
    memory::copy(&superFamicom.firmware[0], &rom[offset], size);
    offset += size;
  }
}

auto Program::loadGameBoy(string location) -> void {
  string manifest;
  vector<uint8_t> rom;

  if(location.endsWith("/")) {
    manifest = file::read({location, "manifest.bml"});
    rom.append(file::read({location, "program.rom"}));
  } else {
    manifest = file::read({Location::notsuffix(location), ".bml"});
    rom = loadFile(location);
  }

  gameBoy.patched = applyPatchIPS(rom, location) || applyPatchBPS(rom, location);
  auto heuristics = Heuristics::GameBoy(rom, location);
  auto sha256 = Hash::SHA256(rom).digest();
  if(auto document = BML::unserialize(string::read(locate("database/Game Boy.bml")))) {
    if(auto game = document[{"game(sha256=", sha256, ")"}]) {
      manifest = BML::serialize(game);
    }
  }
  if(auto document = BML::unserialize(string::read(locate("database/Game Boy Color.bml")))) {
    if(auto game = document[{"game(sha256=", sha256, ")"}]) {
      manifest = BML::serialize(game);
    }
  }
  gameBoy.manifest = manifest ? manifest : heuristics.manifest();
  gameBoy.document = BML::unserialize(gameBoy.manifest);
  gameBoy.location = location;

  gameBoy.program = rom;
}

auto Program::loadBSMemory(string location) -> void {
  string manifest;
  vector<uint8_t> rom;

  if(location.endsWith("/")) {
    manifest = file::read({location, "manifest.bml"});
    rom.append(file::read({location, "program.rom"}));
    rom.append(file::read({location, "program.flash"}));
  } else {
    manifest = file::read({Location::notsuffix(location), ".bml"});
    rom = loadFile(location);
  }

  bsMemory.patched = applyPatchIPS(rom, location) || applyPatchBPS(rom, location);
  auto heuristics = Heuristics::BSMemory(rom, location);
  auto sha256 = Hash::SHA256(rom).digest();
  if(auto document = BML::unserialize(string::read(locate("database/BS Memory.bml")))) {
    if(auto game = document[{"game(sha256=", sha256, ")"}]) {
      manifest = BML::serialize(game);
    }
  }
  bsMemory.manifest = manifest ? manifest : heuristics.manifest();
  bsMemory.document = BML::unserialize(bsMemory.manifest);
  bsMemory.location = location;

  bsMemory.program = rom;
}

auto Program::loadSufamiTurboA(string location) -> void {
  string manifest;
  vector<uint8_t> rom;

  if(location.endsWith("/")) {
    manifest = file::read({location, "manifest.bml"});
    rom.append(file::read({location, "program.rom"}));
  } else {
    manifest = file::read({Location::notsuffix(location), ".bml"});
    rom = loadFile(location);
  }

  sufamiTurboA.patched = applyPatchIPS(rom, location) || applyPatchBPS(rom, location);
  auto heuristics = Heuristics::SufamiTurbo(rom, location);
  auto sha256 = Hash::SHA256(rom).digest();
  if(auto document = BML::unserialize(string::read(locate("database/Sufami Turbo.bml")))) {
    if(auto game = document[{"game(sha256=", sha256, ")"}]) {
      manifest = BML::serialize(game);
    }
  }
  sufamiTurboA.manifest = manifest ? manifest : heuristics.manifest();
  sufamiTurboA.document = BML::unserialize(sufamiTurboA.manifest);
  sufamiTurboA.location = location;

  sufamiTurboA.program = rom;
}

auto Program::loadSufamiTurboB(string location) -> void {
  string manifest;
  vector<uint8_t> rom;

  if(location.endsWith("/")) {
    manifest = file::read({location, "manifest.bml"});
    rom.append(file::read({location, "program.rom"}));
  } else {
    manifest = file::read({Location::notsuffix(location), ".bml"});
    rom = loadFile(location);
  }

  sufamiTurboB.patched = applyPatchIPS(rom, location) || applyPatchBPS(rom, location);
  auto heuristics = Heuristics::SufamiTurbo(rom, location);
  auto sha256 = Hash::SHA256(rom).digest();
  if(auto document = BML::unserialize(string::read(locate("database/Sufami Turbo.bml")))) {
    if(auto game = document[{"game(sha256=", sha256, ")"}]) {
      manifest = BML::serialize(game);
    }
  }
  sufamiTurboB.manifest = manifest ? manifest : heuristics.manifest();
  sufamiTurboB.document = BML::unserialize(sufamiTurboB.manifest);
  sufamiTurboB.location = location;

  sufamiTurboB.program = rom;
}

auto Program::save() -> void {
  if(!emulator->loaded()) return;
  emulator->save();
}

auto Program::reset() -> void {
  if(!emulator->loaded()) return;
  applyHacks();
  emulator->reset();
  showMessage("Game reset");
}

auto Program::unload() -> void {
  if(!emulator->loaded()) return;
  toolsWindow->cheatEditor.saveCheats();
  toolsWindow->setVisible(false);
  saveRecoveryState();
  emulator->unload();
  showMessage("Game unloaded");
  superFamicom = {};
  gameBoy = {};
  bsMemory = {};
  sufamiTurboA = {};
  sufamiTurboB = {};
  presentation->setTitle({"bsnes v", Emulator::Version});
  presentation->resetSystem.setEnabled(false);
  presentation->unloadGame.setEnabled(false);
  presentation->toolsMenu.setVisible(false);
  presentation->clearViewport();
}
