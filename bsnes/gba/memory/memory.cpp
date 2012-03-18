#include <gba/gba.hpp>

#define MEMORY_CPP
namespace GBA {

Bus bus;

uint8 Bus::read(unsigned addr) {
  if((addr & 0x0fffc000) == 0x00000000) {
    //00000000-00003fff  BIOS
    return system.bios.data[addr & 0x3fff];
  }

  if((addr & 0x0e000000) == 0x08000000) {
    //08000000-09ffffff  ROM (wait state 0)
    return cartridge.romdata[addr & 0x01ffffff];
  }

  if((addr & 0x0e000000) == 0x0a000000) {
    //0a000000-0bffffff  ROM (wait state 1)
    return cartridge.romdata[addr & 0x01ffffff];
  }

  if((addr & 0x0e000000) == 0x0c000000) {
    //0c000000-0dffffff  ROM (wait state 2)
    return cartridge.romdata[addr & 0x01ffffff];
  }

  //unmapped
  return 0u;
}

void Bus::write(unsigned addr, uint8 data) {
}

}
