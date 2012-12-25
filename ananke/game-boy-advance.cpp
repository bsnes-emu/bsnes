void Ananke::copyGameBoyAdvanceSaves(const string &pathname) {
  if(!file::exists({pathname, "save.ram"})) {
    if(file::exists({information.path, nall::basename(information.name), ".sav"})) {
      file::copy({information.path, nall::basename(information.name), ".sav"}, {pathname, "save.ram"});
    }
  }

  if(!file::exists({pathname, "rtc.ram"})) {
    if(file::exists({information.path, nall::basename(information.name), ".rtc"})) {
      file::copy({information.path, nall::basename(information.name), ".rtc"}, {pathname, "rtc.ram"});
    }
  }
}

string Ananke::createGameBoyAdvanceHeuristic(vector<uint8_t> &buffer) {
  string pathname = {
    userpath(), "Emulation/Game Boy Advance/",
    nall::basename(information.name),
    " (!).gba/"
  };
  directory::create(pathname);

  GameBoyAdvanceCartridge info(buffer.data(), buffer.size());
  string markup = info.markup;
  markup.append("\ninformation\n  title: ", nall::basename(information.name), "\n");
  if(!information.manifest.empty()) markup = information.manifest;  //override with embedded beat manifest, if one exists

  file::write({pathname, "manifest.bml"}, markup);
  file::write({pathname, "program.rom"}, buffer);

  copyGameBoyAdvanceSaves(pathname);
  return pathname;
}

string Ananke::openGameBoyAdvance(vector<uint8_t> &buffer) {
  return createGameBoyAdvanceHeuristic(buffer);
}
