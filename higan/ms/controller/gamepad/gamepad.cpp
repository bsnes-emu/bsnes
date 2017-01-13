Gamepad::Gamepad(uint port) : Controller(port) {
}

auto Gamepad::readData() -> uint7 {
  uint7 data = 0x7f;
  data.bit(0) = !platform->inputPoll(port, ID::Device::Gamepad, Up);
  data.bit(1) = !platform->inputPoll(port, ID::Device::Gamepad, Down);
  data.bit(2) = !platform->inputPoll(port, ID::Device::Gamepad, Left);
  data.bit(3) = !platform->inputPoll(port, ID::Device::Gamepad, Right);
  data.bit(4) = !platform->inputPoll(port, ID::Device::Gamepad, One);
  data.bit(5) = !platform->inputPoll(port, ID::Device::Gamepad, Two);
  return data;
}
