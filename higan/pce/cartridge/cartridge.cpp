#include <pce/pce.hpp>

namespace PCEngine {

Cartridge cartridge;

auto Cartridge::load() -> bool {
  information = {};

  if(auto pathID = platform->load(ID::PCEngine, "PC Engine", "pce")) {
    information.pathID = pathID();
  } else return false;

  if(auto fp = platform->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  information.title = document["information/title"].text();

  if(auto node = document["board/rom"]) {
    rom.size = node["size"].natural();
    if(rom.size) {
      rom.data = new uint8[rom.size]();
      if(auto name = node["name"].text()) {
        if(auto fp = platform->open(pathID(), name, File::Read, File::Required)) {
          fp->read(rom.data, rom.size);
        }
      }
    }
  }

  return true;
}

auto Cartridge::save() -> void {
  auto document = BML::unserialize(information.manifest);
}

auto Cartridge::unload() -> void {
  delete[] rom.data;
  rom = {};
}

auto Cartridge::power() -> void {
}

auto Cartridge::read(uint20 addr) -> uint8 {
  if(!rom.size) return 0x00;
  return rom.data[mirror(addr, rom.size)];
}

auto Cartridge::write(uint20 addr, uint8 data) -> void {
}

auto Cartridge::mirror(uint addr, uint size) -> uint {
  //384KB games have unusual mirroring (only second ROM repeats)
  if(size == 0x60000) {
    if(addr <= 0x3ffff) return addr;
    return 0x40000 + (addr & 0x1ffff);
  }

  uint base = 0;
  uint mask = 1 << 20;
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

}
