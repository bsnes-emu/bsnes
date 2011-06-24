#include <snes/snes.hpp>

#define CONTROLLER_CPP
namespace SNES {

#include "gamepad/gamepad.cpp"
#include "multitap/multitap.cpp"
#include "mouse/mouse.cpp"
#include "superscope/superscope.cpp"
#include "justifier/justifier.cpp"
Controllers controllers;

bool Controller::iobit() {
  switch(port) {
  case Controller::Port1: return cpu.pio() & 0x40;
  case Controller::Port2: return cpu.pio() & 0x80;
  }
}

void Controller::iobit(bool data) {
  switch(port) {
  case Controller::Port1: break;
  case Controller::Port2: break;
  }
}

Controller::Controller(bool port) : port(port) {
}

void Controllers::connect(bool port, Input::Device id) {
  Controller *&controller = (port == Controller::Port1 ? port1 : port2);
  if(controller) {
    delete controller;
    controller = 0;
  }

  switch(id) { default:
  case Input::Device::None: controller = new Controller(port); break;
  case Input::Device::Joypad: controller = new Gamepad(port); break;
  case Input::Device::Multitap: controller = new Multitap(port); break;
  case Input::Device::Mouse: controller = new Mouse(port); break;
  case Input::Device::SuperScope: controller = new SuperScope(port); break;
  case Input::Device::Justifier: controller = new Justifier(port, false); break;
  case Input::Device::Justifiers: controller = new Justifier(port, true); break;
  }

  switch(port) {
  case Controller::Port1: config.controller_port1 = id; break;
  case Controller::Port2: config.controller_port2 = id; break;
  }
}

Controllers::Controllers() {
  connect(Controller::Port1, Input::Device::Joypad);
  connect(Controller::Port2, Input::Device::Joypad);
}

Controllers::~Controllers() {
  if(port1) delete port1;
  if(port2) delete port2;
}

}
