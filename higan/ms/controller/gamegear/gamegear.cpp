GameGearControls::GameGearControls(uint port) : Controller(port) {
}

auto GameGearControls::readData() -> uint8 {
  uint8 data = 0xff;
  data.bit(0) = !platform->inputPoll(port, ID::Device::GameGearControls, Up);
  data.bit(1) = !platform->inputPoll(port, ID::Device::GameGearControls, Down);
  data.bit(2) = !platform->inputPoll(port, ID::Device::GameGearControls, Left);
  data.bit(3) = !platform->inputPoll(port, ID::Device::GameGearControls, Right);
  data.bit(4) = !platform->inputPoll(port, ID::Device::GameGearControls, One);
  data.bit(5) = !platform->inputPoll(port, ID::Device::GameGearControls, Two);
  data.bit(6) = !platform->inputPoll(port, ID::Device::GameGearControls, Start);
  return data;
}
