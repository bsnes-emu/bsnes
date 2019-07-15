auto ICD::audioSample(const float* samples, uint channels) -> void {
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
  case GameBoy::Input::Right:  return bit1(data,0);
  case GameBoy::Input::Left:   return bit1(data,1);
  case GameBoy::Input::Up:     return bit1(data,2);
  case GameBoy::Input::Down:   return bit1(data,3);
  case GameBoy::Input::A:      return bit1(data,4);
  case GameBoy::Input::B:      return bit1(data,5);
  case GameBoy::Input::Select: return bit1(data,6);
  case GameBoy::Input::Start:  return bit1(data,7);
  }

  return 0;
}
