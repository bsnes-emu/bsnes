#include <ms/ms.hpp>

namespace MasterSystem {

Cartridge cartridge;
#include "mapper.cpp"
#include "serialization.cpp"

auto Cartridge::load() -> bool {
  information = {};

  if(Model::MasterSystem()) {
    if(auto pathID = platform->load(ID::MasterSystem, "Master System", "ms")) {
      information.pathID = pathID();
    } else return false;
  }

  if(Model::GameGear()) {
    if(auto pathID = platform->load(ID::GameGear, "Game Gear", "gg")) {
      information.pathID = pathID();
    } else return false;
  }

  if(auto fp = platform->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  information.title = document["information/title"].text();

  if(auto node = document["board/rom"]) {
    rom.size = node["size"].natural();
    rom.mask = bit::round(rom.size) - 1;
    if(rom.size) {
      rom.data = new uint8[rom.mask + 1];
      if(auto name = node["name"].text()) {
        if(auto fp = platform->open(pathID(), name, File::Read, File::Required)) {
          fp->read(rom.data, rom.size);
        }
      }
    }
  }

  if(auto node = document["board/ram"]) {
    ram.size = node["size"].natural();
    ram.mask = bit::round(ram.size) - 1;
    if(ram.size) {
      ram.data = new uint8[ram.mask + 1];
      if(auto name = node["name"].text()) {
        if(auto fp = platform->open(pathID(), name, File::Read)) {
          fp->read(ram.data, ram.size);
        }
      }
    }
  }

  return true;
}

auto Cartridge::save() -> void {
  auto document = BML::unserialize(information.manifest);

  if(auto name = document["board/ram/name"].text()) {
    if(auto fp = platform->open(pathID(), name, File::Write)) {
      fp->write(ram.data, ram.size);
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
  memory::fill(&mapper, sizeof(Mapper));
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
