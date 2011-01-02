#include <gameboy.hpp>

#define CARTRIDGE_CPP
namespace GameBoy {

#include "mbc0/mbc0.cpp"
#include "mbc1/mbc1.cpp"
Cartridge cartridge;

void Cartridge::load(uint8_t *data, unsigned size) {
  romdata = new uint8[romsize = size];
  memcpy(romdata, data, size);

  char name[17];
  memcpy(name, romdata + 0x0134, 16);
  name[16] = 0;
  info.name = name;
  info.name.rtrim();

  info.cgbflag = romdata[0x0143];
  info.sgbflag = romdata[0x0146];

  info.mapper = Mapper::Unknown;
  info.ram = false;
  info.battery = false;
  info.rtc = false;

  switch(romdata[0x0147]) { default:
    case 0x00: info.mapper = Mapper::MBC0; break;
    case 0x01: info.mapper = Mapper::MBC1; break;
    case 0x02: info.mapper = Mapper::MBC1; info.ram = true; break;
    case 0x03: info.mapper = Mapper::MBC1; info.ram = true; info.battery = true; break;
  }

  switch(romdata[0x0148]) { default:
    case 0x00: info.romsize =   2 * 16 * 1024; break;
    case 0x01: info.romsize =   4 * 16 * 1024; break;
    case 0x02: info.romsize =   8 * 16 * 1024; break;
    case 0x03: info.romsize =  16 * 16 * 1024; break;
    case 0x04: info.romsize =  32 * 16 * 1024; break;
    case 0x05: info.romsize =  64 * 16 * 1024; break;
    case 0x06: info.romsize = 128 * 16 * 1024; break;
    case 0x07: info.romsize = 256 * 16 * 1024; break;
    case 0x52: info.romsize =  72 * 16 * 1024; break;
    case 0x53: info.romsize =  80 * 16 * 1024; break;
    case 0x54: info.romsize =  96 * 16 * 1024; break;
  }

  //TODO: MBC2 always stores 0x00 here; yet it has 512x4-bits RAM
  switch(romdata[0x0149]) { default:
    case 0x00: info.ramsize =  0 * 1024; break;
    case 0x01: info.ramsize =  2 * 1024; break;
    case 0x02: info.ramsize =  8 * 1024; break;
    case 0x03: info.ramsize = 32 * 1024; break;
  }

  ramdata = new uint8_t[ramsize = info.ramsize]();

  loaded = true;
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

void Cartridge::power() {
  mbc0.power();
  mbc1.power();

  MMIO *mapper = 0;
  switch(info.mapper) {
    case Mapper::MBC0: mapper = &mbc0; break;
    case Mapper::MBC1: mapper = &mbc1; break;
  }
  if(mapper) {
    for(unsigned n = 0x0000; n <= 0x7fff; n++) bus.mmio[n] = mapper;
    for(unsigned n = 0xa000; n <= 0xbfff; n++) bus.mmio[n] = mapper;
  }

  reset();
}

void Cartridge::reset() {
  mbc1.reset();
  mbc1.reset();
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
