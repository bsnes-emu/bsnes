#include <gameboy.hpp>

#define SYSTEM_CPP
namespace GameBoy {

System system;

void System::init(Interface *interface_) {
  interface = interface_;
}

void System::power() {
  cpu.power();
  scheduler.init();
}

void System::reset() {
  cpu.reset();
  scheduler.init();
}

void System::run() {
  scheduler.enter();
}

}
