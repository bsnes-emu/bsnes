#include <nes/nes.hpp>

namespace NES {

System system;

void System::run() {
  scheduler.enter();
}

void System::power() {
  cartridge.power();
  cpu.power();
  apu.power();
  ppu.power();
  scheduler.power();
}

void System::reset() {
  cartridge.reset();
  cpu.reset();
  apu.reset();
  ppu.reset();
  scheduler.reset();
}

void System::init() {
  assert(interface != 0);
}

void System::term() {
}

}
