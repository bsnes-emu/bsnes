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

//  cheat.reset();
//  cheat.append(CheatCode("GXXZZLVI"));
//  cheat[0].enable = true;
//  cheat.synchronize();
}

void System::reset() {
  cartridge.reset();
  cpu.reset();
  apu.reset();
  ppu.reset();
  scheduler.reset();
}

void System::init(Interface *interface) {
  this->interface = interface;
}

void System::term() {
}

}
