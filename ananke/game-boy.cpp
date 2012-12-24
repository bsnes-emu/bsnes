void Ananke::copyGameBoySaves(const string &pathname) {
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

string Ananke::createGameBoyHeuristic(vector<uint8_t> &buffer) {
  GameBoyCartridge info(buffer.data(), buffer.size());

  string pathname = {
    userpath(),
    "Emulation/Game Boy/",
    nall::basename(information.name),
    " (!).", (info.info.cgb ? "gbc" : "gb"), "/"
  };
  directory::create(pathname);

  string markup = info.markup;
  if(!information.manifest.empty()) markup = information.manifest;  //override with embedded beat manifest, if one exists

  file::write({pathname, "manifest.bml"}, info.markup);
  file::write({pathname, "program.rom"}, buffer);

  copyGameBoySaves(pathname);
  return pathname;
}

string Ananke::openGameBoy(vector<uint8_t> &buffer) {
  return createGameBoyHeuristic(buffer);
}
