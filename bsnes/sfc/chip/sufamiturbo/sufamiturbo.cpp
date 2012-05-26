#include <sfc/sfc.hpp>

#define SUFAMITURBO_CPP
namespace SuperFamicom {

#include "serialization.cpp"
SufamiTurbo sufamiturbo;

void SufamiTurbo::load() {
  slotA.ram.map(allocate<uint8>(128 * 1024, 0xff), 128 * 1024);
  slotB.ram.map(allocate<uint8>(128 * 1024, 0xff), 128 * 1024);

  if(slotA.rom.data() == nullptr) {
    slotA.rom.map(allocate<uint8>(128 * 1024, 0xff), 128 * 1024);
  }

  if(slotB.rom.data() == nullptr) {
    slotB.rom.map(allocate<uint8>(128 * 1024, 0xff), 128 * 1024);
  }
}

void SufamiTurbo::unload() {
  slotA.rom.reset();
  slotA.ram.reset();
  slotB.rom.reset();
  slotB.ram.reset();
}

}
