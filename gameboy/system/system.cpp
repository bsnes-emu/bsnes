#include <gameboy/gameboy.hpp>

#define SYSTEM_CPP
namespace GameBoy {

System system;

void System::init(Interface *interface_) {
  interface = interface_;
}

void System::power() {
  bus.power();
  cartridge.power();
  cpu.power();
  lcd.power();
  scheduler.init();
}

void System::run() {
  scheduler.enter();
}

}
