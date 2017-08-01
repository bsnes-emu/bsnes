ControlPad::ControlPad(uint port) : Controller(port) {
}

auto ControlPad::readData() -> uint8 {
  uint6 data;

  if(select == 0) {
    data.bit(0) = platform->inputPoll(port, ID::Device::ControlPad, Up);
    data.bit(1) = platform->inputPoll(port, ID::Device::ControlPad, Down);
    data.bits(2,3) = ~0;
    data.bit(4) = platform->inputPoll(port, ID::Device::ControlPad, A);
    data.bit(5) = platform->inputPoll(port, ID::Device::ControlPad, Start);
  } else {
    data.bit(0) = platform->inputPoll(port, ID::Device::ControlPad, Up);
    data.bit(1) = platform->inputPoll(port, ID::Device::ControlPad, Down);
    data.bit(2) = platform->inputPoll(port, ID::Device::ControlPad, Left);
    data.bit(3) = platform->inputPoll(port, ID::Device::ControlPad, Right);
    data.bit(4) = platform->inputPoll(port, ID::Device::ControlPad, B);
    data.bit(5) = platform->inputPoll(port, ID::Device::ControlPad, C);
  }

  data = ~data;
  return latch << 7 | select << 6 | data;
}

auto ControlPad::writeData(uint8 data) -> void {
  select = data.bit(6);
  latch = data.bit(7);
}
