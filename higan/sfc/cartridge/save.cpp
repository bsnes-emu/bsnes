auto Cartridge::saveCartridge(Markup::Node node) -> void {
  if(auto node = board["memory(type=RAM,content=Save)"]) saveRAM(node);
  if(auto node = board["mcc"]) saveMCC(node);
  if(auto node = board["event"]) saveEvent(node);
  if(auto node = board["processor(architecture=W65C816S)"]) saveSA1(node);
  if(auto node = board["processor(architecture=GSU)"]) saveSuperFX(node);
  if(auto node = board["armdsp"]) saveARMDSP(node);
  if(auto node = board["processor(architecture=HG51BS169)"]) saveHitachiDSP(node);
  if(auto node = board["necdsp"]) saveNECDSP(node);
  if(auto node = board["rtc(manufacturer=Epson)"]) saveEpsonRTC(node);
  if(auto node = board["rtc(manufacturer=Sharp)"]) saveSharpRTC(node);
  if(auto node = board["processor(identifier=SPC7110)"]) saveSPC7110(node);
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

//processor(architecture=W65C816S)
auto Cartridge::saveSA1(Markup::Node node) -> void {
  if(auto memory = node["memory(type=RAM,content=Save)"]) {
    saveMemory(sa1.bwram, memory);
  }

  if(auto memory = node["memory(type=RAM,content=Internal)"]) {
    saveMemory(sa1.iram, memory);
  }
}

//processor(architecture=GSU)
auto Cartridge::saveSuperFX(Markup::Node node) -> void {
  if(auto memory = node["memory(type=RAM,content=Save)"]) {
    saveMemory(superfx.ram, memory);
  }
}

auto Cartridge::saveARMDSP(Markup::Node node) -> void {
  if(auto memory = game.memory(node["memory(type=RAM,content=Data)"])) {
    if(memory->nonVolatile) {
      if(auto fp = platform->open(ID::SuperFamicom, memory->name(), File::Write)) {
        for(auto n : range(16 * 1024)) fp->write(armdsp.programRAM[n]);
      }
    }
  }
}

//processor(architecture=HG51BS169)
auto Cartridge::saveHitachiDSP(Markup::Node node) -> void {
  saveMemory(hitachidsp.ram, node["ram"]);

  if(auto memory = node["memory(type=RAM,content=Save)"]) {
    saveMemory(hitachidsp.ram, memory);
  }

  if(auto memory = node["memory(type=RAM,content=Data,architecture=HG51BS169)"]) {
    if(auto file = game.memory(memory)) {
      if(file->nonVolatile) {
        if(auto fp = platform->open(ID::SuperFamicom, file->name(), File::Write)) {
          for(auto n : range(3 * 1024)) fp->write(hitachidsp.dataRAM[n]);
        }
      }
    }
  }
}

auto Cartridge::saveNECDSP(Markup::Node node) -> void {
  uint size = necdsp.revision == NECDSP::Revision::uPD7725 ? 256 : 2048;
  if(auto memory = Emulator::Game::Memory{node["memory(type=RAM,content=Data)"]}) {
    if(memory.nonVolatile) {
      if(auto fp = platform->open(ID::SuperFamicom, memory.name(), File::Write)) {
        for(auto n : range(size)) fp->writel(necdsp.dataRAM[n], 2);
      }
    }
  }
}

//rtc(manufacturer=Epson)
auto Cartridge::saveEpsonRTC(Markup::Node node) -> void {
  if(auto memory = node["memory(type=RTC,content=Time,manufacturer=Epson)"]) {
    if(auto file = game.memory(memory)) {
      if(file->nonVolatile) {
        if(auto fp = platform->open(ID::SuperFamicom, file->name(), File::Write)) {
          uint8 data[16] = {0};
          epsonrtc.save(data);
          fp->write(data, 16);
        }
      }
    }
  }
}

//rtc(manufacturer=Sharp)
auto Cartridge::saveSharpRTC(Markup::Node node) -> void {
  if(auto memory = node["memory(type=RTC,content=Time,manufacturer=Sharp)"]) {
    if(auto file = game.memory(memory)) {
      if(file->nonVolatile) {
        if(auto fp = platform->open(ID::SuperFamicom, file->name(), File::Write)) {
          uint8 data[16] = {0};
          sharprtc.save(data);
          fp->write(data, 16);
        }
      }
    }
  }
}

//processor(identifier=SPC7110)
auto Cartridge::saveSPC7110(Markup::Node node) -> void {
  if(auto memory = node["memory(type=RAM,content=Save)"]) {
    saveMemory(spc7110.ram, memory);
  }
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
  if(auto memory = game.memory(node)) {
    if(memory->type == "RAM" && !memory->nonVolatile) return;
    if(memory->type == "RTC" && !memory->nonVolatile) return;
    if(auto fp = platform->open(id(), memory->name(), File::Write)) {
      fp->write(ram.data(), ram.size());
    }
  }
}
