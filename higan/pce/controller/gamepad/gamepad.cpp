Gamepad::Gamepad() {
}

auto Gamepad::readData() -> uint4 {
  if(clr) return 0;

  uint4 data;

  if(sel) {
    bool up    = platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, Up);
    bool right = platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, Right);
    bool down  = platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, Down);
    bool left  = platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, Left);
    data.bit(0) = !(up & !down);
    data.bit(1) = !(right & !left);
    data.bit(2) = !(down & !up);
    data.bit(3) = !(left & !right);
  } else {
    bool one    = platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, One);
    bool two    = platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, Two);
    bool select = platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, Select);
    bool run    = platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, Run);
    data.bit(0) = !one;
    data.bit(1) = !two;
    data.bit(2) = !select;
    data.bit(3) = !run;
  }

  return data;
}

auto Gamepad::writeData(uint2 data) -> void {
  sel = data.bit(0);
  clr = data.bit(1);
}
