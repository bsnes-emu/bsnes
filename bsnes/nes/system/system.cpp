#include <nes/nes.hpp>

namespace NES {

System system;

void System::run() {
  scheduler.enter();
}

void System::power() {
  cpu.power();
  ppu.power();
  scheduler.power();
}

void System::reset() {
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
