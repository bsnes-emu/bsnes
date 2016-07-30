#include <sfc/sfc.hpp>

namespace SuperFamicom {

#include "gamepad/gamepad.cpp"
#include "mouse/mouse.cpp"
#include "super-multitap/super-multitap.cpp"
#include "super-scope/super-scope.cpp"
#include "justifier/justifier.cpp"

Controller::Controller(bool port) : port(port) {
  if(!handle()) create(Controller::Enter, 1);
}

Controller::~Controller() {
  scheduler.remove(*this);
}

auto Controller::Enter() -> void {
  while(true) {
    scheduler.synchronize();
    if(scheduler.active(*peripherals.controllerPort1)) peripherals.controllerPort1->main();
    if(scheduler.active(*peripherals.controllerPort2)) peripherals.controllerPort2->main();
  }
}

auto Controller::main() -> void {
  step(1);
  synchronize(cpu);
}

auto Controller::iobit() -> bool {
  switch(port) {
  case Controller::Port1: return cpu.pio() & 0x40;
  case Controller::Port2: return cpu.pio() & 0x80;
  }
}

auto Controller::iobit(bool data) -> void {
  switch(port) {
  case Controller::Port1: bus.write(0x4201, (cpu.pio() & ~0x40) | (data << 6)); break;
  case Controller::Port2: bus.write(0x4201, (cpu.pio() & ~0x80) | (data << 7)); break;
  }
}

}
