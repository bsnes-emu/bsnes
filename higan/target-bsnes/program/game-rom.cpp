auto Program::openRomSFC(string name, vfs::file::mode mode) -> vfs::shared::file {
  if(name == "program.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(superNintendo.program.data(), superNintendo.program.size());
  }

  if(name == "data.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(superNintendo.data.data(), superNintendo.data.size());
  }

  if(name == "expansion.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(superNintendo.expansion.data(), superNintendo.expansion.size());
  }

  if(name == "arm6.program.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0x28000) {
      return vfs::memory::file::open(&superNintendo.firmware.data()[0x00000], 0x20000);
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Program,architecture=ARM6)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".program.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "arm6.data.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0x28000) {
      return vfs::memory::file::open(&superNintendo.firmware.data()[0x20000], 0x08000);
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Data,architecture=ARM6)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".data.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "hg51bs169.data.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0xc00) {
      return vfs::memory::file::open(superNintendo.firmware.data(), superNintendo.firmware.size());
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Data,architecture=HG51BS169)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".data.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "lr35902.boot.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0x100) {
      return vfs::memory::file::open(superNintendo.firmware.data(), superNintendo.firmware.size());
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Boot,architecture=LR35902)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".boot.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "upd7725.program.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0x2000) {
      return vfs::memory::file::open(&superNintendo.firmware.data()[0x0000], 0x1800);
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Program,architecture=uPD7725)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".program.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "upd7725.data.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0x2000) {
      return vfs::memory::file::open(&superNintendo.firmware.data()[0x1800], 0x0800);
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Data,architecture=uPD7725)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".data.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "upd96050.program.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0xd000) {
      return vfs::memory::file::open(&superNintendo.firmware.data()[0x0000], 0xc000);
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Program,architecture=uPD96050)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".program.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "upd96050.data.rom" && mode == vfs::file::mode::read) {
    if(superNintendo.firmware.size() == 0xd000) {
      return vfs::memory::file::open(&superNintendo.firmware.data()[0xc000], 0x1000);
    }
    if(auto memory = superNintendo.document["game/board/memory(type=ROM,content=Data,architecture=uPD96050)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".data.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "save.ram") {
    return vfs::fs::file::open(path("Saves", superNintendo.location, ".srm"), mode);
  }

  if(name == "download.ram") {
    return vfs::fs::file::open(path("Saves", superNintendo.location, ".psr"), mode);
  }

  if(name == "time.rtc") {
    return vfs::fs::file::open(path("Saves", superNintendo.location, ".rtc"), mode);
  }

  if(name == "arm6.data.ram") {
    return vfs::fs::file::open(path("Saves", superNintendo.location, ".srm"), mode);
  }

  if(name == "hg51bs169.data.ram") {
    return vfs::fs::file::open(path("Saves", superNintendo.location, ".srm"), mode);
  }

  if(name == "upd7725.data.ram") {
    return vfs::fs::file::open(path("Saves", superNintendo.location, ".srm"), mode);
  }

  if(name == "upd96050.data.ram") {
    return vfs::fs::file::open(path("Saves", superNintendo.location, ".srm"), mode);
  }

  if(name == "msu1/data.rom") {
    return vfs::fs::file::open({Location::notsuffix(superNintendo.location), ".msu"}, mode);
  }

  if(name.match("msu1/track-*.pcm")) {
    name.trimLeft("msu1/track-", 1L);
    return vfs::fs::file::open({Location::notsuffix(superNintendo.location), name}, mode);
  }

  return {};
}

auto Program::openRomGB(string name, vfs::file::mode mode) -> vfs::shared::file {
  if(name == "program.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(gameBoy.program.data(), gameBoy.program.size());
  }

  if(name == "save.ram") {
    return vfs::fs::file::open(path("Saves", gameBoy.location, ".sav"), mode);
  }

  if(name == "time.rtc") {
    return vfs::fs::file::open(path("Saves", gameBoy.location, ".sav"), mode);
  }

  return {};
}
