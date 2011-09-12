#include <nes/nes.hpp>

namespace NES {

System system;

void System::run() {
  scheduler.enter();
}

void System::power() {
  cartridge.power();
  cpu.power();
  ppu.power();
  scheduler.power();
}

void System::reset() {
  cartridge.reset();
  cpu.reset();
  ppu.reset();
  scheduler.reset();
}

void System::init(Interface *interface) {
  this->interface = interface;
}

void System::term() {
}

}
