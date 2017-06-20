#include <md/md.hpp>

namespace MegaDrive {

Cartridge cartridge;
#include "serialization.cpp"

auto Cartridge::load() -> bool {
  information = {};

  if(auto loaded = platform->load(ID::MegaDrive, "Mega Drive", "md", {"NTSC-J", "NTSC-U", "PAL"})) {
    information.pathID = loaded.pathID();
    information.region = loaded.option();
  } else return false;

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
  ramEnable = 1;
  ramWritable = 1;
  for(auto n : range(8)) bank[n] = n;
}

auto Cartridge::read(uint24 addr) -> uint16 {
  if(addr.bit(21) && ram.size && ramEnable) {
    uint16 data = ram.data[addr + 0 & ram.mask] << 8;
    return data | ram.data[addr + 1 & ram.mask] << 0;
  } else {
    addr = bank[addr >> 19 & 7] << 19 | (addr & 0x7ffff);
    uint16 data = rom.data[addr + 0 & rom.mask] << 8;
    return data | rom.data[addr + 1 & rom.mask] << 0;
  }
}

auto Cartridge::write(uint24 addr, uint16 data) -> void {
  //emulating RAM write protect bit breaks some commercial software
  if(addr.bit(21) && ram.size && ramEnable /* && ramWritable */) {
    ram.data[addr + 0 & ram.mask] = data >> 8;
    ram.data[addr + 1 & ram.mask] = data >> 0;
  }
}

auto Cartridge::readIO(uint24 addr) -> uint16 {
  return 0x0000;
}

auto Cartridge::writeIO(uint24 addr, uint16 data) -> void {
  if(addr == 0xa130f1) ramEnable = data.bit(0), ramWritable = data.bit(1);
  if(addr == 0xa130f3) bank[1] = data;
  if(addr == 0xa130f5) bank[2] = data;
  if(addr == 0xa130f7) bank[3] = data;
  if(addr == 0xa130f9) bank[4] = data;
  if(addr == 0xa130fb) bank[5] = data;
  if(addr == 0xa130fd) bank[6] = data;
  if(addr == 0xa130ff) bank[7] = data;
}

}
