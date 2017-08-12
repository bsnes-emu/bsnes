#include <md/md.hpp>

namespace MegaDrive {

Cartridge cartridge;
#include "serialization.cpp"

auto Cartridge::load() -> bool {
  information = {};

  if(auto loaded = platform->load(ID::MegaDrive, "Mega Drive", "md", {"Auto", "NTSC-J", "NTSC-U", "PAL"})) {
    information.pathID = loaded.pathID();
    information.region = loaded.option();
  } else return false;

  if(auto fp = platform->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  information.title = document["information/title"].text();

  if(information.region == "Auto") {
    if(auto region = document["board/region"].text()) {
      information.region = region.upcase();
    } else {
      information.region = "NTSC-J";
    }
  }

  if(auto node = document["board/rom"]) {
    rom.size = node["size"].natural() >> 1;
    rom.mask = bit::round(rom.size) - 1;
    if(rom.size) {
      rom.data = new uint16[rom.mask + 1]();
      if(auto name = node["name"].text()) {
        if(auto fp = platform->open(pathID(), name, File::Read, File::Required)) {
          for(uint n : range(rom.size)) rom.data[n] = fp->readm(2);
        }
      }
    }
  }

  if(auto node = document["board/ram"]) {
    if(auto mode = node["mode"].text()) {
      if(mode == "lo"  ) ram.bits = 0x00ff;
      if(mode == "hi"  ) ram.bits = 0xff00;
      if(mode == "word") ram.bits = 0xffff;
    }
    ram.size = node["size"].natural() >> (ram.bits == 0xffff);
    ram.mask = bit::round(ram.size) - 1;
    if(ram.size) {
      ram.data = new uint16[ram.mask + 1]();
      if(auto name = node["name"].text()) {
        if(auto fp = platform->open(pathID(), name, File::Read)) {
          for(uint n : range(ram.size)) {
            if(ram.bits != 0xffff) ram.data[n] = fp->readm(1) * 0x0101;
            if(ram.bits == 0xffff) ram.data[n] = fp->readm(2);
          }
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
      for(uint n : range(ram.size)) {
        if(ram.bits != 0xffff) fp->writem(ram.data[n], 1);
        if(ram.bits == 0xffff) fp->writem(ram.data[n], 2);
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
  ramEnable = 1;
  ramWritable = 1;
  for(auto n : range(8)) bank[n] = n;
}

auto Cartridge::read(uint24 address) -> uint16 {
  if(address.bit(21) && ram.size && ramEnable) {
    return ram.data[address >> 1 & ram.mask];
  } else {
    address = bank[address.bits(19,21)] << 19 | address.bits(0,18);
    return rom.data[address >> 1 & rom.mask];
  }
}

auto Cartridge::write(uint24 address, uint16 data) -> void {
  //emulating RAM write protect bit breaks some commercial software
  if(address.bit(21) && ram.size && ramEnable /* && ramWritable */) {
    if(ram.bits == 0x00ff) data = data.byte(0) * 0x0101;
    if(ram.bits == 0xff00) data = data.byte(1) * 0x0101;
    ram.data[address >> 1 & ram.mask] = data;
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
