auto Program::openRomSuperFamicom(string name, vfs::file::mode mode) -> vfs::shared::file {
  if(name == "program.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(superFamicom.program.data(), superFamicom.program.size());
  }

  if(name == "data.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(superFamicom.data.data(), superFamicom.data.size());
  }

  if(name == "expansion.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(superFamicom.expansion.data(), superFamicom.expansion.size());
  }

  if(name == "arm6.program.rom" && mode == vfs::file::mode::read) {
    if(superFamicom.firmware.size() == 0x28000) {
      return vfs::memory::file::open(&superFamicom.firmware.data()[0x00000], 0x20000);
    }
    if(auto memory = superFamicom.document["game/board/memory(type=ROM,content=Program,architecture=ARM6)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".program.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "arm6.data.rom" && mode == vfs::file::mode::read) {
    if(superFamicom.firmware.size() == 0x28000) {
      return vfs::memory::file::open(&superFamicom.firmware.data()[0x20000], 0x08000);
    }
    if(auto memory = superFamicom.document["game/board/memory(type=ROM,content=Data,architecture=ARM6)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".data.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "hg51bs169.data.rom" && mode == vfs::file::mode::read) {
    if(superFamicom.firmware.size() == 0xc00) {
      return vfs::memory::file::open(superFamicom.firmware.data(), superFamicom.firmware.size());
    }
    if(auto memory = superFamicom.document["game/board/memory(type=ROM,content=Data,architecture=HG51BS169)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".data.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "lr35902.boot.rom" && mode == vfs::file::mode::read) {
    if(superFamicom.firmware.size() == 0x100) {
      return vfs::memory::file::open(superFamicom.firmware.data(), superFamicom.firmware.size());
    }
    if(auto memory = superFamicom.document["game/board/memory(type=ROM,content=Boot,architecture=LR35902)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".boot.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "upd7725.program.rom" && mode == vfs::file::mode::read) {
    if(superFamicom.firmware.size() == 0x2000) {
      return vfs::memory::file::open(&superFamicom.firmware.data()[0x0000], 0x1800);
    }
    if(auto memory = superFamicom.document["game/board/memory(type=ROM,content=Program,architecture=uPD7725)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".program.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "upd7725.data.rom" && mode == vfs::file::mode::read) {
    if(superFamicom.firmware.size() == 0x2000) {
      return vfs::memory::file::open(&superFamicom.firmware.data()[0x1800], 0x0800);
    }
    if(auto memory = superFamicom.document["game/board/memory(type=ROM,content=Data,architecture=uPD7725)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".data.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "upd96050.program.rom" && mode == vfs::file::mode::read) {
    if(superFamicom.firmware.size() == 0xd000) {
      return vfs::memory::file::open(&superFamicom.firmware.data()[0x0000], 0xc000);
    }
    if(auto memory = superFamicom.document["game/board/memory(type=ROM,content=Program,architecture=uPD96050)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".program.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "upd96050.data.rom" && mode == vfs::file::mode::read) {
    if(superFamicom.firmware.size() == 0xd000) {
      return vfs::memory::file::open(&superFamicom.firmware.data()[0xc000], 0x1000);
    }
    if(auto memory = superFamicom.document["game/board/memory(type=ROM,content=Data,architecture=uPD96050)"]) {
      string location = locate({"firmware/", memory["identifier"].text().downcase(), ".data.rom"});
      return vfs::fs::file::open(location, mode);
    }
  }

  if(name == "save.ram") {
    return vfs::fs::file::open(path("Saves", superFamicom.location, ".srm"), mode);
  }

  if(name == "download.ram") {
    return vfs::fs::file::open(path("Saves", superFamicom.location, ".psr"), mode);
  }

  if(name == "time.rtc") {
    return vfs::fs::file::open(path("Saves", superFamicom.location, ".rtc"), mode);
  }

  if(name == "arm6.data.ram") {
    return vfs::fs::file::open(path("Saves", superFamicom.location, ".srm"), mode);
  }

  if(name == "hg51bs169.data.ram") {
    return vfs::fs::file::open(path("Saves", superFamicom.location, ".srm"), mode);
  }

  if(name == "upd7725.data.ram") {
    return vfs::fs::file::open(path("Saves", superFamicom.location, ".srm"), mode);
  }

  if(name == "upd96050.data.ram") {
    return vfs::fs::file::open(path("Saves", superFamicom.location, ".srm"), mode);
  }

  if(name == "msu1/data.rom") {
    return vfs::fs::file::open({Location::notsuffix(superFamicom.location), ".msu"}, mode);
  }

  if(name.match("msu1/track-*.pcm")) {
    name.trimLeft("msu1/track-", 1L);
    return vfs::fs::file::open({Location::notsuffix(superFamicom.location), name}, mode);
  }

  return {};
}

auto Program::openRomGameBoy(string name, vfs::file::mode mode) -> vfs::shared::file {
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

auto Program::openRomBSMemory(string name, vfs::file::mode mode) -> vfs::shared::file {
  if(name == "program.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(bsMemory.program.data(), bsMemory.program.size());
  }

  if(name == "program.flash") {
    //writes are not flushed to disk in bsnes
    return vfs::memory::file::open(bsMemory.program.data(), bsMemory.program.size());
  }

  return {};
}

auto Program::openRomSufamiTurboA(string name, vfs::file::mode mode) -> vfs::shared::file {
  if(name == "program.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(sufamiTurboA.program.data(), sufamiTurboA.program.size());
  }

  if(name == "save.ram") {
    return vfs::fs::file::open(path("Saves", sufamiTurboA.location, ".srm"), mode);
  }

  return {};
}

auto Program::openRomSufamiTurboB(string name, vfs::file::mode mode) -> vfs::shared::file {
  if(name == "program.rom" && mode == vfs::file::mode::read) {
    return vfs::memory::file::open(sufamiTurboB.program.data(), sufamiTurboB.program.size());
  }

  if(name == "save.ram") {
    return vfs::fs::file::open(path("Saves", sufamiTurboB.location, ".srm"), mode);
  }

  return {};
}
