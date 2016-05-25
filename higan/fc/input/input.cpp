#include <fc/fc.hpp>

namespace Famicom {

#include "serialization.cpp"
Input input;

auto Input::latch(bool data) -> void {
  latchdata = data;

  if(latchdata == 1) {
    counter1 = 0;
    counter2 = 0;
  }
}

auto Input::data(bool port) -> bool {
  //table to convert native button ordering to Emulator::Interface ordering
  static const uint lookup[] = {5, 4, 6, 7, 0, 1, 2, 3};

  bool result = 0;

  if(port == 0) {
    if(port1 == Device::Joypad) {
      if(counter1 >= 8) return 1;
      result = interface->inputPoll(0, 0u, lookup[counter1]);
      if(latchdata == 0) counter1++;
    }
  }

  if(port == 1) {
    if(port2 == Device::Joypad) {
      if(counter2 >= 8) return 1;
      result = interface->inputPoll(1, 0u, lookup[counter2]);
      if(latchdata == 0) counter2++;
    }
  }

  return result;
}

auto Input::connect(bool port, Device device) -> void {
  if(port == 0) port1 = device, counter1 = 0;
  if(port == 1) port2 = device, counter2 = 0;
}

auto Input::power() -> void {
}

auto Input::reset() -> void {
  latchdata = 0;
  counter1 = 0;
  counter2 = 0;
}

}
