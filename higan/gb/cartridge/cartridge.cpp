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

auto Cartridge::load(System::Revision revision) -> bool {
  information = Information();

  switch(revision) {
  case System::Revision::GameBoy:
    if(auto pathID = interface->load(ID::GameBoy, "Game Boy", "gb", true)) {
      information.pathID = pathID();
    } else return false;
    break;
  case System::Revision::SuperGameBoy:
    if(auto pathID = interface->load(ID::SuperGameBoy, "Game Boy", "gb", true)) {
      information.pathID = pathID();
    } else return false;
    break;
  case System::Revision::GameBoyColor:
    if(auto pathID = interface->load(ID::GameBoyColor, "Game Boy Color", "gbc", true)) {
      information.pathID = pathID();
    } else return false;
    break;
  }

  if(auto fp = interface->open(pathID(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
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

  auto rom = document["board/rom"];
  auto ram = document["board/ram"];

  romsize = max(32768u, rom["size"].natural());
  romdata = allocate<uint8>(romsize, 0xff);

  ramsize = ram["size"].natural();
  ramdata = allocate<uint8>(ramsize, 0xff);

  if(auto name = rom["name"].text()) {
    if(auto fp = interface->open(pathID(), name, File::Read, File::Required)) {
      fp->read(romdata, min(romsize, fp->size()));
    }
  }
  if(auto name = ram["name"].text()) {
    if(auto fp = interface->open(pathID(), name, File::Read, File::Optional)) {
      fp->read(ramdata, min(ramsize, fp->size()));
    }
  }

  information.romsize = romsize;
  information.ramsize = ramsize;
  information.battery = (bool)ram["name"];

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

  sha256 = Hash::SHA256(romdata, romsize).digest();
  return true;
}

auto Cartridge::save() -> void {
  auto document = BML::unserialize(information.manifest);

  if(auto name = document["board/ram/name"].text()) {
    if(auto fp = interface->open(pathID(), name, File::Write)) {
      fp->write(ramdata, ramsize);
    }
  }
}

auto Cartridge::unload() -> void {
  if(romdata) { delete[] romdata; romdata = nullptr; romsize = 0; }
  if(ramdata) { delete[] ramdata; ramdata = nullptr; ramsize = 0; }
}

auto Cartridge::rom_read(uint addr) -> uint8 {
  if(addr >= romsize) addr %= romsize;
  return romdata[addr];
}

auto Cartridge::rom_write(uint addr, uint8 data) -> void {
  if(addr >= romsize) addr %= romsize;
  romdata[addr] = data;
}

auto Cartridge::ram_read(uint addr) -> uint8 {
  if(ramsize == 0) return 0xff;
  if(addr >= ramsize) addr %= ramsize;
  return ramdata[addr];
}

auto Cartridge::ram_write(uint addr, uint8 data) -> void {
  if(ramsize == 0) return;
  if(addr >= ramsize) addr %= ramsize;
  ramdata[addr] = data;
}

auto Cartridge::mmio_read(uint16 addr) -> uint8 {
  if(addr == 0xff50) return 0xff;

  if(bootrom_enable) {
    const uint8* data = nullptr;
    switch(system.revision()) { default:
    case System::Revision::GameBoy: data = system.bootROM.dmg; break;
    case System::Revision::SuperGameBoy: data = system.bootROM.sgb; break;
    case System::Revision::GameBoyColor: data = system.bootROM.cgb; break;
    }
    if(addr >= 0x0000 && addr <= 0x00ff) return data[addr];
    if(addr >= 0x0200 && addr <= 0x08ff && system.cgb()) return data[addr - 256];
  }

  return mapper->mmio_read(addr);
}

auto Cartridge::mmio_write(uint16 addr, uint8 data) -> void {
  if(bootrom_enable && addr == 0xff50) {
    bootrom_enable = false;
    return;
  }

  mapper->mmio_write(addr, data);
}

auto Cartridge::power() -> void {
  bootrom_enable = true;

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
