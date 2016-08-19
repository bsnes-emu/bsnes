#include <ms/ms.hpp>

namespace MasterSystem {

Cartridge cartridge;

auto Cartridge::load() -> bool {
  information = {};

  switch(system.model()) {
  case Model::MasterSystem:
    if(auto pathID = interface->load(ID::MasterSystem, "Master System", "ms")) {
      information.pathID = pathID();
    } else return false;
    break;
  case Model::GameGear:
    if(auto pathID = interface->load(ID::GameGear, "Game Gear", "gg")) {
      information.pathID = pathID();
    } else return false;
    break;
  }

  if(auto fp = interface->open(pathID(), "manifest.bml", File::Read, File::Required)) {
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
        if(auto fp = interface->open(pathID(), name, File::Read, File::Required)) {
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
        if(auto fp = interface->open(pathID(), name, File::Read)) {
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
    if(auto fp = interface->open(pathID(), name, File::Write)) {
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

auto Cartridge::read(uint16 addr) -> uint8 {
  return rom.data[addr & rom.mask];
}

auto Cartridge::write(uint16 addr, uint8 data) -> void {
}

auto Cartridge::power() -> void {
}

auto Cartridge::reset() -> void {
}

}
