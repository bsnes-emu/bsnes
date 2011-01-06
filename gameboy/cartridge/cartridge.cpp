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
Cartridge cartridge;

void Cartridge::load(uint8_t *data, unsigned size) {
//uint32_t crc = crc32_calculate(data, size);
//print("CRC32 = ", hex<4>(crc), "\n");

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

  switch(romdata[0x0147]) {
    case 0x00: info.mapper = Mapper::MBC0; break;
    case 0x01: info.mapper = Mapper::MBC1; break;
    case 0x02: info.mapper = Mapper::MBC1; info.ram = true; break;
    case 0x03: info.mapper = Mapper::MBC1; info.ram = true; info.battery = true; break;
    case 0x05: info.mapper = Mapper::MBC2; info.ram = true; break;
    case 0x06: info.mapper = Mapper::MBC2; info.ram = true; info.battery = true; break;
    case 0x08: info.mapper = Mapper::MBC0; info.ram = true; break;
    case 0x09: info.mapper = Mapper::MBC0; info.ram = true; info.battery = true; break;
    case 0x0b: info.mapper = Mapper::MMM01; break;
    case 0x0c: info.mapper = Mapper::MMM01; info.ram = true; break;
    case 0x0d: info.mapper = Mapper::MMM01; info.ram = true; info.battery = true; break;
    case 0x0f: info.mapper = Mapper::MBC3; info.rtc = true; info.battery = true; break;
    case 0x10: info.mapper = Mapper::MBC3; info.rtc = true; info.ram = true; info.battery = true; break;
    case 0x11: info.mapper = Mapper::MBC3; break;
    case 0x12: info.mapper = Mapper::MBC3; info.ram = true; break;
    case 0x13: info.mapper = Mapper::MBC3; info.ram = true; info.battery = true; break;
    case 0x19: info.mapper = Mapper::MBC5; break;
    case 0x1a: info.mapper = Mapper::MBC5; info.ram = true; break;
    case 0x1b: info.mapper = Mapper::MBC5; info.ram = true; info.battery = true; break;
    case 0x1c: info.mapper = Mapper::MBC5; info.rumble = true; break;
    case 0x1d: info.mapper = Mapper::MBC5; info.rumble = true; info.ram = true; break;
    case 0x1e: info.mapper = Mapper::MBC5; info.rumble = true; info.ram = true; info.battery = true; break;
    case 0xfc: break;  //Pocket Camera
    case 0xfd: break;  //Bandai TAMA5
    case 0xfe: info.mapper = Mapper::HuC3; break;
    case 0xff: info.mapper = Mapper::HuC1; info.ram = true; info.battery = true; break;
  }
//print("Mapper: ", hex<2>(romdata[0x0147]), "\n");

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

  switch(romdata[0x0149]) { default:
    case 0x00: info.ramsize =  0 * 1024; break;
    case 0x01: info.ramsize =  2 * 1024; break;
    case 0x02: info.ramsize =  8 * 1024; break;
    case 0x03: info.ramsize = 32 * 1024; break;
  }

  if(info.mapper == Mapper::MBC2) info.ramsize = 512;  //512 x 4-bit

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
  mbc2.power();
  mbc3.power();
  mbc5.power();
  mmm01.power();
  huc1.power();
  huc3.power();
  map();
}

void Cartridge::map() {
  MMIO *mapper = 0;
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

  if(mapper) {
    for(unsigned n = 0x0000; n <= 0x7fff; n++) bus.mmio[n] = mapper;
    for(unsigned n = 0xa000; n <= 0xbfff; n++) bus.mmio[n] = mapper;
  }
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
