auto ICD::audioSample(const double* samples, uint channels) -> void {
  stream->write(samples);
}

auto ICD::inputPoll(uint port, uint device, uint id) -> int16 {
  uint8 data = 0x00;
  switch(joypID) {
  case 0: data = ~r6004; break;
  case 1: data = ~r6005; break;
  case 2: data = ~r6006; break;
  case 3: data = ~r6007; break;
  }

  switch((GameBoy::Input)id) {
  case GameBoy::Input::Right:  return data.bit(0);
  case GameBoy::Input::Left:   return data.bit(1);
  case GameBoy::Input::Up:     return data.bit(2);
  case GameBoy::Input::Down:   return data.bit(3);
  case GameBoy::Input::A:      return data.bit(4);
  case GameBoy::Input::B:      return data.bit(5);
  case GameBoy::Input::Select: return data.bit(6);
  case GameBoy::Input::Start:  return data.bit(7);
  }

  return 0;
}
