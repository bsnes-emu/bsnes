void Ananke::copyFamicomSaves(const string &pathname) {
  if(!file::exists({pathname, "save.ram"})) {
    if(file::exists({information.path, nall::basename(information.name), ".sav"})) {
      file::copy({information.path, nall::basename(information.name), ".srm"}, {pathname, "save.ram"});
    }
  }
}

string Ananke::createFamicomHeuristic(vector<uint8_t> &buffer) {
  string pathname = {
    libraryPath, "Famicom/",
    nall::basename(information.name),
    ".fc/"
  };
  directory::create(pathname);

  FamicomCartridge info(buffer.data(), buffer.size());
  string markup = {"unverified\n\n", info.markup};
  markup.append("\ninformation\n  title: ", nall::basename(information.name), "\n");
  if(!information.manifest.empty()) markup = information.manifest;  //override with embedded beat manifest, if one exists

  file::write({pathname, "manifest.bml"}, markup);
  file::write({pathname, "program.rom"}, buffer.data() + 16, info.prgrom);
  if(info.chrrom > 0) file::write({pathname, "character.rom"}, buffer.data() + 16 + info.prgrom, info.chrrom);

  copyFamicomSaves(pathname);
  return pathname;
}

string Ananke::openFamicom(vector<uint8_t> &buffer) {
  return createFamicomHeuristic(buffer);
}

//this currently cannot work:
//game folders discard iNES header required for heuristic detection
//a games database of all commercial Famicom software will be required
string Ananke::syncFamicom(const string &pathname) {
  return "";
}
