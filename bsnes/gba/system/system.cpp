#include <gba/gba.hpp>

namespace GBA {

#include "bios.cpp"
BIOS bios;
System system;

void System::init() {
}

void System::term() {
}

void System::power() {
  bus.power();
  cpu.power();
  ppu.power();
  apu.power();
  cartridge.power();
  scheduler.power();
}

void System::run() {
  scheduler.enter();
}

}
