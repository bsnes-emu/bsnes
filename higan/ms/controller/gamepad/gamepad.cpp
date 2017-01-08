Gamepad::Gamepad(uint port) : Controller(port) {
}

auto Gamepad::readData() -> uint7 {
  uint7 data;

  data.bit(0) = !interface->inputPoll(port, ID::Device::Gamepad, Up);
  data.bit(1) = !interface->inputPoll(port, ID::Device::Gamepad, Down);
  data.bit(2) = !interface->inputPoll(port, ID::Device::Gamepad, Left);
  data.bit(3) = !interface->inputPoll(port, ID::Device::Gamepad, Right);
  data.bit(4) = !interface->inputPoll(port, ID::Device::Gamepad, One);
  data.bit(5) = !interface->inputPoll(port, ID::Device::Gamepad, Two);
  data.bit(6) = 1;

  return data;
}
