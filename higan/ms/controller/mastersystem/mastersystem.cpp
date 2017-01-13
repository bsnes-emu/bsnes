MasterSystemControls::MasterSystemControls(uint port) : Controller(port) {
}

auto MasterSystemControls::readData() -> uint8 {
  uint8 data = 0xff;
  data.bit(0) = !platform->inputPoll(port, ID::Device::MasterSystemControls, Reset);
  return data;
}
