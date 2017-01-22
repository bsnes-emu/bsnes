#include <ms/ms.hpp>

namespace MasterSystem {

#include "gamepad/gamepad.cpp"

Controller::Controller(uint port) : port(port) {
  if(!handle()) create(Controller::Enter, 100);
}

Controller::~Controller() {
  scheduler.remove(*this);
}

auto Controller::Enter() -> void {
  while(true) {
    scheduler.synchronize();
    if(auto device = peripherals.controllerPort1) if(device->active()) device->main();
    if(auto device = peripherals.controllerPort2) if(device->active()) device->main();
  }
}

auto Controller::main() -> void {
  step(1);
  synchronize(cpu);
}

}
