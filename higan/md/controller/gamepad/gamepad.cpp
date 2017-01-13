Gamepad::Gamepad(uint port) : Controller(port) {
}

auto Gamepad::readData() -> uint8 {
  uint6 data;

  if(select == 0) {
    data.bit(0) = platform->inputPoll(port, ID::Device::Gamepad, Up);
    data.bit(1) = platform->inputPoll(port, ID::Device::Gamepad, Down);
    data.bit(2) = 1;
    data.bit(3) = 1;
    data.bit(4) = platform->inputPoll(port, ID::Device::Gamepad, A);
    data.bit(5) = platform->inputPoll(port, ID::Device::Gamepad, Start);
  } else {
    data.bit(0) = platform->inputPoll(port, ID::Device::Gamepad, Up);
    data.bit(1) = platform->inputPoll(port, ID::Device::Gamepad, Down);
    data.bit(2) = platform->inputPoll(port, ID::Device::Gamepad, Left);
    data.bit(3) = platform->inputPoll(port, ID::Device::Gamepad, Right);
    data.bit(4) = platform->inputPoll(port, ID::Device::Gamepad, B);
    data.bit(5) = platform->inputPoll(port, ID::Device::Gamepad, C);
  }

  data = ~data;
  return latch << 7 | select << 6 | data;
}

auto Gamepad::writeData(uint8 data) -> void {
  select = data.bit(6);
  latch  = data.bit(7);
}
