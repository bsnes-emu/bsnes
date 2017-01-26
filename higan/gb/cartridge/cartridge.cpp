#include <gb/gb.hpp>

namespace GameBoy {

#include "mbc0/mbc0.cpp"
#include "mbc1/mbc1.cpp"
#include "mbc1m/mbc1m.cpp"
#include "mbc2/mbc2.cpp"
#include "mbc3/mbc3.cpp"
#include "mbc5/mbc5.cpp"
#include "mmm01/mmm01.cpp"
#include "huc1/huc1.cpp"
#include "huc3/huc3.cpp"
#include "serialization.cpp"
Cartridge cartridge;

auto Cartridge::load() -> bool {
  information = {};

  if(Model::GameBoy()) {
    if(auto pathID = platform->load(ID::GameBoy, "Game Boy", "gb")) {
      information.pathID = pathID();
    } else return false;
  }

  if(Model::GameBoyColor()) {
    if(auto pathID = platform->load(ID::GameBoyColor, "Game Boy Color", "gbc")) {
      information.pathID = pathID();
    } else return false;
  }

  if(Model::SuperGameBoy()) {
    if(auto pathID = platform->load(ID::SuperGameBoy, "Game Boy", "gb")) {
      information.pathID = pathID();
    } else return false;
  }

  if(auto fp = platform->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  auto board = document["board"];
  information.title = document["information/title"].text();

  auto mapperid = document["board/mapper"].text();
  if(mapperid == "none" ) information.mapper = Mapper::MBC0;
  if(mapperid == "MBC1" ) information.mapper = Mapper::MBC1;
  if(mapperid == "MBC1M") information.mapper = Mapper::MBC1M;
  if(mapperid == "MBC2" ) information.mapper = Mapper::MBC2;
  if(mapperid == "MBC3" ) information.mapper = Mapper::MBC3;
  if(mapperid == "MBC5" ) information.mapper = Mapper::MBC5;
  if(mapperid == "MMM01") information.mapper = Mapper::MMM01;
  if(mapperid == "HuC1" ) information.mapper = Mapper::HuC1;
  if(mapperid == "HuC3" ) information.mapper = Mapper::HuC3;

  information.rtc = false;
  information.rumble = false;

  rom.size = max(32768u, board["rom/size"].natural());
  rom.data = (uint8*)memory::allocate(rom.size, 0xff);

  ram.size = board["ram/size"].natural();
  ram.data = (uint8*)memory::allocate(ram.size, 0xff);

  if(auto name = board["rom/name"].text()) {
    if(auto fp = platform->open(pathID(), name, File::Read, File::Required)) {
      fp->read(rom.data, min(rom.size, fp->size()));
    }
  }
  if(auto name = board["ram/name"].text()) {
    if(auto fp = platform->open(pathID(), name, File::Read, File::Optional)) {
      fp->read(ram.data, min(ram.size, fp->size()));
    }
  }

  information.battery = (bool)board["ram/name"];

  switch(information.mapper) { default:
  case Mapper::MBC0:  mapper = &mbc0;  break;
  case Mapper::MBC1:  mapper = &mbc1;  break;
  case Mapper::MBC1M: mapper = &mbc1m; break;
  case Mapper::MBC2:  mapper = &mbc2;  break;
  case Mapper::MBC3:  mapper = &mbc3;  break;
  case Mapper::MBC5:  mapper = &mbc5;  break;
  case Mapper::MMM01: mapper = &mmm01; break;
  case Mapper::HuC1:  mapper = &huc1;  break;
  case Mapper::HuC3:  mapper = &huc3;  break;
  }

  information.sha256 = Hash::SHA256(rom.data, rom.size).digest();
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

auto Cartridge::readROM(uint addr) -> uint8 {
  if(addr >= rom.size) addr %= rom.size;
  return rom.data[addr];
}

auto Cartridge::writeROM(uint addr, uint8 data) -> void {
  if(addr >= rom.size) addr %= rom.size;
  rom.data[addr] = data;
}

auto Cartridge::readRAM(uint addr) -> uint8 {
  if(ram.size == 0) return 0xff;
  if(addr >= ram.size) addr %= ram.size;
  return ram.data[addr];
}

auto Cartridge::writeRAM(uint addr, uint8 data) -> void {
  if(ram.size == 0) return;
  if(addr >= ram.size) addr %= ram.size;
  ram.data[addr] = data;
}

auto Cartridge::readIO(uint16 addr) -> uint8 {
  if(addr == 0xff50) return 0xff;

  if(bootromEnable) {
    const uint8* data = nullptr;
    if(Model::GameBoy()) data = system.bootROM.dmg;
    if(Model::GameBoyColor()) data = system.bootROM.cgb;
    if(Model::SuperGameBoy()) data = system.bootROM.sgb;
    if(addr >= 0x0000 && addr <= 0x00ff) return data[addr];
    if(addr >= 0x0200 && addr <= 0x08ff && Model::GameBoyColor()) return data[addr - 256];
  }

  return mapper->readIO(addr);
}

auto Cartridge::writeIO(uint16 addr, uint8 data) -> void {
  if(bootromEnable && addr == 0xff50) {
    bootromEnable = false;
    return;
  }

  mapper->writeIO(addr, data);
}

auto Cartridge::power() -> void {
  bootromEnable = true;

  mbc0.power();
  mbc1.power();
  mbc1m.power();
  mbc2.power();
  mbc3.power();
  mbc5.power();
  mmm01.power();
  huc1.power();
  huc3.power();

  for(uint n = 0x0000; n <= 0x7fff; n++) bus.mmio[n] = this;
  for(uint n = 0xa000; n <= 0xbfff; n++) bus.mmio[n] = this;
  bus.mmio[0xff50] = this;
}

}
