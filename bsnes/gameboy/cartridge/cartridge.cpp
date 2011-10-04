#include <gameboy/gameboy.hpp>

#include <nall/crc32.hpp>

#define CARTRIDGE_CPP
namespace GameBoy {

#include "mbc0/mbc0.cpp"
#include "mbc1/mbc1.cpp"
#include "mbc2/mbc2.cpp"
#include "mbc3/mbc3.cpp"
#include "mbc5/mbc5.cpp"
#include "mmm01/mmm01.cpp"
#include "huc1/huc1.cpp"
#include "huc3/huc3.cpp"
#include "serialization.cpp"
Cartridge cartridge;

void Cartridge::load(const string &markup, const uint8_t *data, unsigned size) {
  if(size == 0) size = 32768;
  romdata = allocate<uint8>(romsize = size, 0xff);
  if(data) memcpy(romdata, data, size);

//uint32_t crc = crc32_calculate(data, size);
//print("CRC32 = ", hex<4>(crc), "\n");

  info.mapper = Mapper::Unknown;
  info.ram = false;
  info.battery = false;
  info.rtc = false;
  info.rumble = false;

  info.romsize = 0;
  info.ramsize = 0;

  BML::Document document(markup);

  auto &mapperid = document["cartridge"]["mapper"].value;
  if(mapperid == "none" ) info.mapper = Mapper::MBC0;
  if(mapperid == "MBC1" ) info.mapper = Mapper::MBC1;
  if(mapperid == "MBC2" ) info.mapper = Mapper::MBC2;
  if(mapperid == "MBC3" ) info.mapper = Mapper::MBC3;
  if(mapperid == "MBC5" ) info.mapper = Mapper::MBC5;
  if(mapperid == "MMM01") info.mapper = Mapper::MMM01;
  if(mapperid == "HuC1" ) info.mapper = Mapper::HuC1;
  if(mapperid == "HuC3" ) info.mapper = Mapper::HuC3;

  info.rtc = document["cartridge"]["rtc"].exists();
  info.rumble = document["cartridge"]["rumble"].exists();

  info.romsize = hex(document["cartridge"]["rom"]["size"].value);
  info.ramsize = hex(document["cartridge"]["ram"]["size"].value);
  info.battery = document["cartridge"]["ram"]["non-volatile"].exists();

  switch(info.mapper) { default:
    case Mapper::MBC0:  mapper = &mbc0;  break;
    case Mapper::MBC1:  mapper = &mbc1;  break;
    case Mapper::MBC2:  mapper = &mbc2;  break;
    case Mapper::MBC3:  mapper = &mbc3;  break;
    case Mapper::MBC5:  mapper = &mbc5;  break;
    case Mapper::MMM01: mapper = &mmm01; break;
    case Mapper::HuC1:  mapper = &huc1;  break;
    case Mapper::HuC3:  mapper = &huc3;  break;
  }

  ramdata = new uint8_t[ramsize = info.ramsize]();
  system.load();

  loaded = true;
  sha256 = nall::sha256(romdata, romsize);
}

void Cartridge::unload() {
  if(loaded == false) return;

  if(romdata) { delete[] romdata; romdata = 0; }
  if(ramdata) { delete[] ramdata; ramdata = 0; }
  loaded = false;
}

uint8 Cartridge::rom_read(unsigned addr) {
  if(addr >= romsize) addr %= romsize;
  return romdata[addr];
}

void Cartridge::rom_write(unsigned addr, uint8 data) {
  if(addr >= romsize) addr %= romsize;
  romdata[addr] = data;
}

uint8 Cartridge::ram_read(unsigned addr) {
  if(ramsize == 0) return 0x00;
  if(addr >= ramsize) addr %= ramsize;
  return ramdata[addr];
}

void Cartridge::ram_write(unsigned addr, uint8 data) {
  if(ramsize == 0) return;
  if(addr >= ramsize) addr %= ramsize;
  ramdata[addr] = data;
}

uint8 Cartridge::mmio_read(uint16 addr) {
  if(bootrom_enable && within<0x0000, 0x00ff>(addr)) return System::BootROM::sgb[addr];
  return mapper->mmio_read(addr);
}

void Cartridge::mmio_write(uint16 addr, uint8 data) {
  if(bootrom_enable && addr == 0xff50) bootrom_enable = false;
  mapper->mmio_write(addr, data);
}

void Cartridge::power() {
  bootrom_enable = true;

  mbc0.power();
  mbc1.power();
  mbc2.power();
  mbc3.power();
  mbc5.power();
  mmm01.power();
  huc1.power();
  huc3.power();

  for(unsigned n = 0x0000; n <= 0x7fff; n++) bus.mmio[n] = this;
  for(unsigned n = 0xa000; n <= 0xbfff; n++) bus.mmio[n] = this;
  bus.mmio[0xff50] = this;
}

Cartridge::Cartridge() {
  loaded = false;
  romdata = 0;
  ramdata = 0;
}

Cartridge::~Cartridge() {
  unload();
}

}
