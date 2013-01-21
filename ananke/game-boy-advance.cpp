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
    libraryPath, "Game Boy Advance/",
    nall::basename(information.name),
    ".gba/"
  };
  directory::create(pathname);
  file::create({pathname, "unverified"});

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

string Ananke::syncGameBoyAdvance(const string &pathname) {
  auto buffer = file::read({pathname, "program.rom"});
  if(buffer.size() == 0) return "";

  auto save = file::read({pathname, "save.ram"});
  if(save.size() == 0) save = file::read({pathname, "save.rwm"});

  auto rtc = file::read({pathname, "rtc.ram"});
  if(rtc.size() == 0) rtc = file::read({pathname, "rtc.rwm"});

  directory::remove(pathname);
  information.path = pathname;
  information.name = notdir(string{pathname}.rtrim<1>("/"));
  string outputPath = openGameBoyAdvance(buffer);

  if(save.size()) file::write({outputPath, "save.ram"}, save);
  if(rtc.size()) file::write({outputPath, "rtc.ram"}, rtc);

  return outputPath;
}
