#include <md/md.hpp>

namespace MegaDrive {

ControllerPort controllerPort1;
ControllerPort controllerPort2;
ControllerPort extensionPort;
#include "gamepad/gamepad.cpp"

Controller::Controller(uint port) : port(port) {
  if(!handle()) create(Controller::Enter, 1);
}

Controller::~Controller() {
  scheduler.remove(*this);
}

auto Controller::Enter() -> void {
  while(true) {
    scheduler.synchronize();
    if(controllerPort1.device->active()) controllerPort1.device->main();
    if(controllerPort2.device->active()) controllerPort2.device->main();
    if(extensionPort.device->active()) extensionPort.device->main();
  }
}

auto Controller::main() -> void {
  step(1);
  synchronize(cpu);
}

//

auto ControllerPort::connect(uint deviceID) -> void {
  if(!system.loaded()) return;
  delete device;

  switch(deviceID) { default:
  case ID::Device::None: device = new Controller(port); break;
  case ID::Device::Gamepad: device = new Gamepad(port); break;
  }

  cpu.peripherals.reset();
  if(auto device = controllerPort1.device) cpu.peripherals.append(device);
  if(auto device = controllerPort2.device) cpu.peripherals.append(device);
  if(auto device = extensionPort.device) cpu.peripherals.append(device);
}

auto ControllerPort::readControl() -> uint8 {
  return control;
}

auto ControllerPort::writeControl(uint8 data) -> void {
  control = data;
}

auto ControllerPort::power(uint port) -> void {
  this->port = port;
  control = 0x00;
}

auto ControllerPort::unload() -> void {
  delete device;
  device = nullptr;
}

auto ControllerPort::serialize(serializer& s) -> void {
  s.integer(control);
}

}
