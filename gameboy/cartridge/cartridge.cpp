#include <gameboy.hpp>

#define CARTRIDGE_CPP
namespace GameBoy {

Cartridge cartridge;

void Cartridge::load(uint8_t *data, unsigned size) {
  bus.cartrom.copy(data, size);

  char name[17];
  memcpy(name, bus.cartrom.data + 0x0134, 16);
  name[16] = 0;
  info.name = name;
  info.name.rtrim();

  info.cgbflag = bus.cartrom[0x0143];
  info.sgbflag = bus.cartrom[0x0146];
  info.type    = bus.cartrom[0x0147];

  switch(bus.cartrom[0x0148]) { default:
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
  switch(bus.cartrom[0x0149]) { default:
    case 0x00: info.ramsize =  0 * 1024; break;
    case 0x01: info.ramsize =  2 * 1024; break;
    case 0x02: info.ramsize =  8 * 1024; break;
    case 0x03: info.ramsize = 32 * 1024; break;
  }

  loaded = true;
  system.interface->message({ "Loaded:\n", info.name });
}

void Cartridge::unload() {
  if(loaded == false) return;

  bus.cartrom.free();
  loaded = false;
}

Cartridge::Cartridge() {
  loaded = false;
}

}
