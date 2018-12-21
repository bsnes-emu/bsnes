#include <ms/ms.hpp>

namespace MasterSystem {

Cartridge cartridge;
#include "mapper.cpp"
#include "serialization.cpp"

auto Cartridge::load() -> bool {
  information = {};

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
    rom.size = memory.size;
    rom.mask = bit::round(rom.size) - 1;
    rom.data = new uint8[rom.mask + 1];
    if(auto fp = platform->open(pathID(), memory.name(), File::Read, File::Required)) {
      fp->read(rom.data, rom.size);
    }
  }

  if(auto memory = Emulator::Game::Memory{document["game/board/memory(type=RAM,content=Save)"]}) {
    ram.size = memory.size;
    ram.mask = bit::round(ram.size) - 1;
    ram.data = new uint8[ram.mask + 1];
    if(memory.nonVolatile) {
      if(auto fp = platform->open(pathID(), memory.name(), File::Read)) {
        fp->read(ram.data, ram.size);
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
        fp->write(ram.data, ram.size);
      }
    }
  }
}

auto Cartridge::unload() -> void {
  delete[] rom.data;
  delete[] ram.data;
  rom = {};
  ram = {};
}

auto Cartridge::power() -> void {
  mapper = {};
  mapper.romPage0 = 0;
  mapper.romPage1 = 1;
  mapper.romPage2 = 2;
}

auto Cartridge::Memory::mirror(uint addr, uint size) -> uint {
  uint base = 0;
  uint mask = 1 << 21;
  while(addr >= size) {
    while(!(addr & mask)) mask >>= 1;
    addr -= mask;
    if(size > mask) {
      size -= mask;
      base += mask;
    }
    mask >>= 1;
  }
  return base + addr;
}

auto Cartridge::Memory::read(uint addr) -> uint8 {
  if(!size) return 0x00;
  return this->data[mirror(addr, size)];
}

auto Cartridge::Memory::write(uint addr, uint8 data) -> void {
  if(!size) return;
  this->data[mirror(addr, size)] = data;
}

}
