#include <pce/pce.hpp>

namespace PCEngine {

#include "gamepad/gamepad.cpp"

Controller::Controller() {
  if(!handle()) create(Controller::Enter, 100);
}

Controller::~Controller() {
  scheduler.remove(*this);
}

auto Controller::Enter() -> void {
  while(true) {
    scheduler.synchronize();
    if(peripherals.controllerPort->active()) peripherals.controllerPort->main();
  }
}

auto Controller::main() -> void {
  step(1);
  synchronize(cpu);
}

}
