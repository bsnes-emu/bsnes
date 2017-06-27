#include <md/md.hpp>

namespace MegaDrive {

ControllerPort controllerPort1;
ControllerPort controllerPort2;
ControllerPort extensionPort;
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
    if(controllerPort1.controller->active()) controllerPort1.controller->main();
    if(controllerPort2.controller->active()) controllerPort2.controller->main();
    if(extensionPort.controller->active())   extensionPort.controller->main();
  }
}

auto Controller::main() -> void {
  step(1);
  synchronize(cpu);
}

//

auto ControllerPort::connect(uint device) -> void {
  if(!system.loaded()) return;
  delete controller;

  switch(device) { default:
  case ID::Device::None: controller = new Controller(port); break;
  case ID::Device::Gamepad: controller = new Gamepad(port); break;
  }

  cpu.peripherals.reset();
  cpu.peripherals.append(controllerPort1.controller);
  cpu.peripherals.append(controllerPort2.controller);
  cpu.peripherals.append(extensionPort.controller);
}

auto ControllerPort::readData() -> uint8 {
  return controller->readData();
}

auto ControllerPort::writeData(uint8 data) -> void {
  return controller->writeData(data);
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
  delete controller;
  controller = nullptr;
}

auto ControllerPort::serialize(serializer& s) -> void {
  s.integer(control);
}

}
