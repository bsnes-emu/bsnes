auto Cartridge::saveCartridge(Markup::Node node) -> void {
  if(auto node = board["ram"]) saveRAM(node);
  if(auto node = board["mcc"]) saveMCC(node);
  if(auto node = board["event"]) saveEvent(node);
  if(auto node = board["sa1"]) saveSA1(node);
  if(auto node = board["superfx"]) saveSuperFX(node);
  if(auto node = board["armdsp"]) saveARMDSP(node);
  if(auto node = board["hitachidsp"]) saveHitachiDSP(node);
  if(auto node = board["necdsp"]) saveNECDSP(node);
  if(auto node = board["epsonrtc"]) saveEpsonRTC(node);
  if(auto node = board["sharprtc"]) saveSharpRTC(node);
  if(auto node = board["spc7110"]) saveSPC7110(node);
  if(auto node = board["sdd1"]) saveSDD1(node);
  if(auto node = board["obc1"]) saveOBC1(node);
}

auto Cartridge::saveGameBoy(Markup::Node node) -> void {
}

auto Cartridge::saveBSMemory(Markup::Node node) -> void {
}

auto Cartridge::saveSufamiTurboA(Markup::Node node) -> void {
  saveMemory(sufamiturboA.ram, node["game/board/memory(type=RAM)"], sufamiturboA.pathID);
}

auto Cartridge::saveSufamiTurboB(Markup::Node node) -> void {
  saveMemory(sufamiturboB.ram, node["game/board/memory(type=RAM)"], sufamiturboB.pathID);
}

//

auto Cartridge::saveRAM(Markup::Node node) -> void {
  saveMemory(ram, node);
}

auto Cartridge::saveMCC(Markup::Node node) -> void {
  saveMemory(mcc.ram, node["ram"]);
}

auto Cartridge::saveEvent(Markup::Node node) -> void {
  saveMemory(event.ram, node["ram"]);
}

auto Cartridge::saveSA1(Markup::Node node) -> void {
  saveMemory(sa1.bwram, node["bwram"]);
  saveMemory(sa1.iram, node["iram"]);
}

auto Cartridge::saveSuperFX(Markup::Node node) -> void {
  saveMemory(superfx.ram, node["ram"]);
}

auto Cartridge::saveARMDSP(Markup::Node node) -> void {
  if(auto memory = game.memory(node["ram/name"].text())) {
    if(memory->battery) {
      if(auto fp = platform->open(ID::SuperFamicom, memory->name(), File::Write)) {
        for(auto n : range(16 * 1024)) fp->write(armdsp.programRAM[n]);
      }
    }
  }
}

auto Cartridge::saveHitachiDSP(Markup::Node node) -> void {
  saveMemory(hitachidsp.ram, node["ram"]);

  if(auto memory = game.memory(node["dram/name"].text())) {
    if(memory->battery) {
      if(auto fp = platform->open(ID::SuperFamicom, memory->name(), File::Write)) {
        for(auto n : range(3 * 1024)) fp->write(hitachidsp.dataRAM[n]);
      }
    }
  }
}

auto Cartridge::saveNECDSP(Markup::Node node) -> void {
  uint size = necdsp.revision == NECDSP::Revision::uPD7725 ? 256 : 2048;
  if(auto memory = game.memory(node["dram/name"].text())) {
    if(memory->battery) {
      if(auto fp = platform->open(ID::SuperFamicom, memory->name(), File::Write)) {
        for(auto n : range(size)) fp->writel(necdsp.dataRAM[n], 2);
      }
    }
  }
}

auto Cartridge::saveEpsonRTC(Markup::Node node) -> void {
  if(auto memory = game.memory(node["ram/name"].text())) {
    if(memory->battery) {
      if(auto fp = platform->open(ID::SuperFamicom, memory->name(), File::Write)) {
        uint8 data[16] = {0};
        epsonrtc.save(data);
        fp->write(data, 16);
      }
    }
  }
}

auto Cartridge::saveSharpRTC(Markup::Node node) -> void {
  if(auto memory = game.memory(node["ram/name"].text())) {
    if(memory->battery) {
      if(auto fp = platform->open(ID::SuperFamicom, memory->name(), File::Write)) {
        uint8 data[16] = {0};
        sharprtc.save(data);
        fp->write(data, 16);
      }
    }
  }
}

auto Cartridge::saveSPC7110(Markup::Node node) -> void {
  saveMemory(spc7110.ram, node["ram"]);
}

auto Cartridge::saveSDD1(Markup::Node node) -> void {
  saveMemory(sdd1.ram, node["ram"]);
}

auto Cartridge::saveOBC1(Markup::Node node) -> void {
  saveMemory(obc1.ram, node["ram"]);
}

//

auto Cartridge::saveMemory(MappedRAM& ram, Markup::Node node, maybe<uint> id) -> void {
  if(!id) id = pathID();
  if(auto memory = game.memory(node["name"].text())) {
    if(memory->type == "RAM" && !memory->battery) return;
    if(memory->type == "RTC" && !memory->battery) return;
    if(auto fp = platform->open(id(), memory->name(), File::Write)) {
      fp->write(ram.data(), ram.size());
    }
  }
}
