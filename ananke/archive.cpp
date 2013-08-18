vector<uint8_t> Ananke::extractROM() {
  unzip archive;
  if(archive.open(information.archive)) {
    for(auto &file : archive.file) {
      if(file.name.endswith(".fc")  || file.name.endswith(".nes")
      || file.name.endswith(".sfc") || file.name.endswith(".smc")
      || file.name.endswith(".st")  || file.name.endswith(".bs")
      || file.name.endswith(".gb")  || file.name.endswith(".gbc")
      || file.name.endswith(".gba")
      ) {
        information.name = notdir(file.name);
        return archive.extract(file);
      }
    }
  }
  return vector<uint8_t>();
}

vector<uint8_t> Ananke::extractFile(const string &filename) {
  unzip archive;
  if(archive.open(information.archive)) {
    for(auto &file : archive.file) {
      if(notdir(file.name) == filename) {
        return archive.extract(file);
      }
    }
  }
  return vector<uint8_t>();
}
