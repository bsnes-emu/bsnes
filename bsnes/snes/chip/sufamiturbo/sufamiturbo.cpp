#include <snes/snes.hpp>

#define SUFAMITURBO_CPP
namespace SNES {

#include "serialization.cpp"
SufamiTurbo sufamiturbo;

void SufamiTurbo::enable() {
  if(slotA.rom.data()) {
    slotA.ram.map(allocate<uint8>(128 * 1024, 0xff), 128 * 1024);
    cartridge.nvram.append({ "srm", slotA.ram.data(), slotA.ram.size(), 1 });
  }

  if(slotB.rom.data()) {
    slotB.ram.map(allocate<uint8>(128 * 1024, 0xff), 128 * 1024);
    cartridge.nvram.append({ "srm", slotB.ram.data(), slotB.ram.size(), 2 });
  }
}

}
