#include <pce/pce.hpp>

namespace PCEngine {

ControllerPort controllerPort;
#include "gamepad/gamepad.cpp"

Controller::Controller() {
  if(!handle()) create(Controller::Enter, 1);
}

Controller::~Controller() {
  scheduler.remove(*this);
}

auto Controller::Enter() -> void {
  while(true) {
    scheduler.synchronize();
    if(controllerPort.device->active()) controllerPort.device->main();
  }
}

auto Controller::main() -> void {
  step(1);
  synchronize(cpu);
}

//

auto ControllerPort::connect(uint deviceID) -> void {
  delete device;
  if(!system.loaded()) return;

  switch(deviceID) { default:
  case ID::Device::None: device = new Controller; break;
  case ID::Device::Gamepad: device = new Gamepad; break;
  }

  cpu.peripherals.reset();
  if(auto device = controllerPort.device) cpu.peripherals.append(device);
}

auto ControllerPort::power() -> void {
}

auto ControllerPort::unload() -> void {
  delete device;
  device = nullptr;
}

auto ControllerPort::serialize(serializer& s) -> void {
}

}
