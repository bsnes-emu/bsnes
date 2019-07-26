auto Program::load() -> void {
  unload();

  if(auto configuration = string::read(locate("configuration.bml"))) {
    emulator->configure(configuration);
    emulatorSettings.updateConfiguration();
  }
  if(!emulator->load()) return;

  gameQueue = {};
  screenshot = {};
  frameAdvance = false;
  if(!verified() && emulatorSettings.warnOnUnverifiedGames.checked()) {
    //Emulator::loaded() is true at this point:
    //prevent Program::main() from calling Emulator::run() during this dialog window
    auto lock = acquire();
    auto response = MessageDialog(
      "Warning: this game image is unverified.\n"
      "Running it *may* be a security risk.\n\n"
      "Do you wish to run the game anyway?"
    ).setAlignment(*presentation).question({"Always", "Yes", "No"});
    if(response == "No") {
      emulator->unload();
      return showMessage("Game loading cancelled");
    }
    if(response == "Always") {
      emulatorSettings.warnOnUnverifiedGames.setChecked(false).doToggle();
    }
  }
  hackCompatibility();
  emulator->power();
  if(emulatorSettings.autoLoadStateOnLoad.checked()) {
    program.loadState("Quick/Undo");
  }
  showMessage({
    verified() ? "Verified game loaded" : "Game loaded",
    appliedPatch() ? " and patch applied" : ""
  });
  presentation.setFocused();
  presentation.setTitle(emulator->titles().merge(" + "));
  presentation.resetSystem.setEnabled(true);
  presentation.unloadGame.setEnabled(true);
  presentation.toolsMenu.setVisible(true);
  presentation.updateStateMenus();
  presentation.speedNormal.setChecked();
  presentation.pauseEmulation.setChecked(false);
  presentation.updateProgramIcon();
  presentation.updateStatusIcon();
  rewindReset();  //starts rewind state recording
  movieMode(Movie::Mode::Inactive);  //to set initial movie menu state
  cheatFinder.restart();  //clear any old cheat search results
  cheatEditor.loadCheats();
  stateManager.loadStates();
  manifestViewer.loadManifest();

  string games;
  if(auto& game = superFamicom) games.append(game.option, ";", game.location, "|");
  if(auto& game = gameBoy     ) games.append(game.option, ";", game.location, "|");
  if(auto& game = bsMemory    ) games.append(game.option, ";", game.location, "|");
  if(auto& game = sufamiTurboA) games.append(game.option, ";", game.location, "|");
  if(auto& game = sufamiTurboB) games.append(game.option, ";", game.location, "|");
  presentation.addRecentGame(games.trimRight("|", 1L));

  updateVideoPalette();
  updateAudioEffects();
  updateAudioFrequency();
}

auto Program::loadFile(string location) -> vector<uint8_t> {
  if(Location::suffix(location).downcase() == ".zip") {
    Decode::ZIP archive;
    if(archive.open(location)) {
      for(auto& file : archive.file) {
        auto type = Location::suffix(file.name).downcase();
        if(type == ".sfc" || type == ".smc" || type == ".gb" || type == ".gbc" || type == ".bs" || type == ".st") {
          return archive.extract(file);
        }
      }
    }
    return {};
  } else if(Location::suffix(location).downcase() == ".7z") {
    return LZMA::extract(location);
  } else {
    return file::read(location);
  }
}

auto Program::loadSuperFamicom(string location) -> bool {
  string manifest;
  vector<uint8_t> rom;

  if(location.endsWith("/")) {
    manifest = file::read({location, "manifest.bml"});
    rom.append(file::read({location, "program.rom"}));
    rom.append(file::read({location, "data.rom"}));
    rom.append(file::read({location, "expansion.rom"}));
    for(auto& filename : directory::files(location, "*.boot.rom"   )) rom.append(file::read({location, filename}));
    for(auto& filename : directory::files(location, "*.program.rom")) rom.append(file::read({location, filename}));
    for(auto& filename : directory::files(location, "*.data.rom"   )) rom.append(file::read({location, filename}));
  } else {
    manifest = file::read({Location::notsuffix(location), ".bml"});
    rom = loadFile(location);
  }
  if(rom.size() < 0x8000) return false;

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
  if(auto document = BML::unserialize(string::read(locate("Database/Super Famicom.bml")))) {
    if(auto game = document[{"game(sha256=", sha256, ")"}]) {
      manifest = BML::serialize(game);
      superFamicom.verified = true;
    }
  }
  superFamicom.title = heuristics.title();
  superFamicom.manifest = manifest ? manifest : heuristics.manifest();
  hackPatchMemory(rom);
  hackOverclockSuperFX();
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

  return true;
}

auto Program::loadGameBoy(string location) -> bool {
  string manifest;
  vector<uint8_t> rom;

  if(location.endsWith("/")) {
    manifest = file::read({location, "manifest.bml"});
    rom.append(file::read({location, "program.rom"}));
  } else {
    manifest = file::read({Location::notsuffix(location), ".bml"});
    rom = loadFile(location);
  }
  if(rom.size() < 0x4000) return false;

  gameBoy.patched = applyPatchIPS(rom, location) || applyPatchBPS(rom, location);
  auto heuristics = Heuristics::GameBoy(rom, location);
  auto sha256 = Hash::SHA256(rom).digest();
  if(auto document = BML::unserialize(string::read(locate("Database/Game Boy.bml")))) {
    if(auto game = document[{"game(sha256=", sha256, ")"}]) {
      manifest = BML::serialize(game);
      gameBoy.verified = true;
    }
  }
  if(auto document = BML::unserialize(string::read(locate("Database/Game Boy Color.bml")))) {
    if(auto game = document[{"game(sha256=", sha256, ")"}]) {
      manifest = BML::serialize(game);
      gameBoy.verified = true;
    }
  }
  gameBoy.manifest = manifest ? manifest : heuristics.manifest();
  gameBoy.document = BML::unserialize(gameBoy.manifest);
  gameBoy.location = location;

  gameBoy.program = rom;
  return true;
}

auto Program::loadBSMemory(string location) -> bool {
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
  if(rom.size() < 0x8000) return false;

  bsMemory.patched = applyPatchIPS(rom, location) || applyPatchBPS(rom, location);
  auto heuristics = Heuristics::BSMemory(rom, location);
  auto sha256 = Hash::SHA256(rom).digest();
  if(auto document = BML::unserialize(string::read(locate("Database/BS Memory.bml")))) {
    if(auto game = document[{"game(sha256=", sha256, ")"}]) {
      manifest = BML::serialize(game);
      bsMemory.verified = true;
    }
  }
  bsMemory.manifest = manifest ? manifest : heuristics.manifest();
  bsMemory.document = BML::unserialize(bsMemory.manifest);
  bsMemory.location = location;

  bsMemory.program = rom;
  return true;
}

auto Program::loadSufamiTurboA(string location) -> bool {
  string manifest;
  vector<uint8_t> rom;

  if(location.endsWith("/")) {
    manifest = file::read({location, "manifest.bml"});
    rom.append(file::read({location, "program.rom"}));
  } else {
    manifest = file::read({Location::notsuffix(location), ".bml"});
    rom = loadFile(location);
  }
  if(rom.size() < 0x20000) return false;

  sufamiTurboA.patched = applyPatchIPS(rom, location) || applyPatchBPS(rom, location);
  auto heuristics = Heuristics::SufamiTurbo(rom, location);
  auto sha256 = Hash::SHA256(rom).digest();
  if(auto document = BML::unserialize(string::read(locate("Database/Sufami Turbo.bml")))) {
    if(auto game = document[{"game(sha256=", sha256, ")"}]) {
      manifest = BML::serialize(game);
      sufamiTurboA.verified = true;
    }
  }
  sufamiTurboA.manifest = manifest ? manifest : heuristics.manifest();
  sufamiTurboA.document = BML::unserialize(sufamiTurboA.manifest);
  sufamiTurboA.location = location;

  sufamiTurboA.program = rom;
  return true;
}

auto Program::loadSufamiTurboB(string location) -> bool {
  string manifest;
  vector<uint8_t> rom;

  if(location.endsWith("/")) {
    manifest = file::read({location, "manifest.bml"});
    rom.append(file::read({location, "program.rom"}));
  } else {
    manifest = file::read({Location::notsuffix(location), ".bml"});
    rom = loadFile(location);
  }
  if(rom.size() < 0x20000) return false;

  sufamiTurboB.patched = applyPatchIPS(rom, location) || applyPatchBPS(rom, location);
  auto heuristics = Heuristics::SufamiTurbo(rom, location);
  auto sha256 = Hash::SHA256(rom).digest();
  if(auto document = BML::unserialize(string::read(locate("Database/Sufami Turbo.bml")))) {
    if(auto game = document[{"game(sha256=", sha256, ")"}]) {
      manifest = BML::serialize(game);
      sufamiTurboB.verified = true;
    }
  }
  sufamiTurboB.manifest = manifest ? manifest : heuristics.manifest();
  sufamiTurboB.document = BML::unserialize(sufamiTurboB.manifest);
  sufamiTurboB.location = location;

  sufamiTurboB.program = rom;
  return true;
}

auto Program::save() -> void {
  if(!emulator->loaded()) return;
  emulator->save();
}

auto Program::reset() -> void {
  if(!emulator->loaded()) return;
  rewindReset();  //don't allow rewinding past a reset point
  hackCompatibility();
  emulator->reset();
  showMessage("Game reset");
}

auto Program::unload() -> void {
  if(!emulator->loaded()) return;
  //todo: video.clear() is not working on macOS/OpenGL 3.2
  if(auto [output, length] = video.acquire(1, 1); output) {
    *output = 0;
    video.release();
    video.output();
  }
  audio.clear();
  rewindReset();  //free up memory that is no longer needed
  movieStop();  //in case a movie is currently being played or recorded
  cheatEditor.saveCheats();
  toolsWindow.setVisible(false);
  if(emulatorSettings.autoSaveStateOnUnload.checked()) {
    saveUndoState();
  }
  if(auto configuration = emulator->configuration()) {
    file::write(locate("configuration.bml"), configuration);
  }
  emulator->unload();
  showMessage("Game unloaded");
  superFamicom = {};
  gameBoy = {};
  bsMemory = {};
  sufamiTurboA = {};
  sufamiTurboB = {};
  presentation.setTitle({"bsnes v", Emulator::Version});
  presentation.resetSystem.setEnabled(false);
  presentation.unloadGame.setEnabled(false);
  presentation.toolsMenu.setVisible(false);
  presentation.updateProgramIcon();
  presentation.updateStatusIcon();
}

//a game is considered verified if the game plus its slot(s) are found in the games database
auto Program::verified() const -> bool {
  if(!emulator->loaded()) return false;
  if(superFamicom && !superFamicom.verified) return false;
  if(gameBoy && !gameBoy.verified) return false;
  if(bsMemory && !bsMemory.verified) return false;
  if(sufamiTurboA && !sufamiTurboA.verified) return false;
  if(sufamiTurboB && !sufamiTurboB.verified) return false;
  return true;
}
