#include <pce/pce.hpp>

namespace PCEngine {

Cartridge cartridge;

auto Cartridge::load() -> bool {
  information = {};

  if(auto pathID = interface->load(ID::PCEngine, "PC Engine", "pce")) {
    information.pathID = pathID();
  } else return false;

  if(auto fp = interface->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  information.title = document["information/title"].text();

  if(auto node = document["board/rom"]) {
    rom.size = node["size"].natural();
    if(rom.size) {
      rom.data = new uint8[rom.size]();
      if(auto name = node["name"].text()) {
        if(auto fp = interface->open(pathID(), name, File::Read, File::Required)) {
          fp->read(rom.data, rom.size);
        }
      }
    }
  }

  if(auto node = document["board/ram"]) {
    ram.size = node["size"].natural();
    if(ram.size) {
      ram.data = new uint8[ram.size]();
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

auto Cartridge::power() -> void {
}

auto Cartridge::Memory::mirror(uint addr, uint size) -> uint {
  uint base = 0;
  uint mask = 1 << 23;
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
