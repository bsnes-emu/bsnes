#include <ms/ms.hpp>

namespace MasterSystem {

Cartridge cartridge;
#include "mapper.cpp"
#include "serialization.cpp"

auto Cartridge::load() -> bool {
  information = {};

  if(Model::ColecoVision()) {
    if(auto loaded = platform->load(ID::ColecoVision, "ColecoVision", "cv", {"NTSC", "PAL"})) {
      information.pathID = loaded.pathID;
      information.region = loaded.option;
    } else return false;
  }

  if(Model::SG1000()) {
    if(auto loaded = platform->load(ID::SG1000, "SG-1000", "sg1000", {"NTSC", "PAL"})) {
      information.pathID = loaded.pathID;
      information.region = loaded.option;
    } else return false;
  }

  if(Model::SC3000()) {
    if(auto loaded = platform->load(ID::SC3000, "SC-3000", "sc3000", {"NTSC", "PAL"})) {
      information.pathID = loaded.pathID;
      information.region = loaded.option;
    } else return false;
  }

  if(Model::MasterSystem()) {
    if(auto loaded = platform->load(ID::MasterSystem, "Master System", "ms", {"NTSC", "PAL"})) {
      information.pathID = loaded.pathID;
      information.region = loaded.option;
    } else return false;
  }

  if(Model::GameGear()) {
    if(auto loaded = platform->load(ID::GameGear, "Game Gear", "gg", {"NTSC"})) {
      information.pathID = loaded.pathID;
    } else return false;
  }

  if(auto fp = platform->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  information.title = document["game/label"].text();

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=ROM,content=Program)"]}) {
    rom.allocate(memory.size);
    if(auto fp = platform->open(pathID(), memory.name(), File::Read, File::Required)) {
      rom.load(fp);
    } else return false;
  }

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=RAM,content=Save)"]}) {
    ram.allocate(memory.size);
    if(memory.nonVolatile) {
      if(auto fp = platform->open(pathID(), memory.name(), File::Read)) {
        ram.load(fp);
      }
    }
  }

  return true;
}

auto Cartridge::save() -> void {
  auto document = BML::unserialize(information.manifest);

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=RAM,content=Save)"]}) {
    if(memory.nonVolatile) {
      if(auto fp = platform->open(pathID(), memory.name(), File::Write)) {
        ram.save(fp);
      }
    }
  }
}

auto Cartridge::unload() -> void {
  rom.reset();
  ram.reset();
}

auto Cartridge::power() -> void {
  mapper = {};
  mapper.romPage0 = 0;
  mapper.romPage1 = 1;
  mapper.romPage2 = 2;
}

}
