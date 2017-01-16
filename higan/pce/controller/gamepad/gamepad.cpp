Gamepad::Gamepad() {
}

auto Gamepad::readData() -> uint4 {
  if(clr) return 0;

  uint4 data;

  if(sel) {
    data.bit(0) = !platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, Up);
    data.bit(1) = !platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, Down);
    data.bit(2) = !platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, Right);
    data.bit(3) = !platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, Left);
    if(data.bits(0,1) == 0) data.bits(0,1) = 3;  //disallow up+down at the same time
    if(data.bits(2,3) == 0) data.bits(2,3) = 3;  //disallow left+right at the same time
  } else {
    data.bit(0) = !platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, One);
    data.bit(1) = !platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, Two);
    data.bit(2) = !platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, Select);
    data.bit(3) = !platform->inputPoll(ID::Port::Controller, ID::Device::Gamepad, Run);
  }

  return data;
}

auto Gamepad::writeData(uint2 data) -> void {
  sel = data.bit(0);
  clr = data.bit(1);
}
