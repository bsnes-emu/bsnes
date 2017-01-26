auto ICD2::audioSample(const double* samples, uint channels) -> void {
  stream->write(samples);
}

auto ICD2::inputPoll(uint port, uint device, uint id) -> int16 {
  GameBoy::cpu.status.mltReq = joypID & mltReq;

  uint data = 0x00;
  switch(joypID & mltReq) {
  case 0: data = ~r6004; break;
  case 1: data = ~r6005; break;
  case 2: data = ~r6006; break;
  case 3: data = ~r6007; break;
  }

  switch((GameBoy::Input)id) {
  case GameBoy::Input::Start:  return (bool)(data & 0x80);
  case GameBoy::Input::Select: return (bool)(data & 0x40);
  case GameBoy::Input::B:      return (bool)(data & 0x20);
  case GameBoy::Input::A:      return (bool)(data & 0x10);
  case GameBoy::Input::Down:   return (bool)(data & 0x08);
  case GameBoy::Input::Up:     return (bool)(data & 0x04);
  case GameBoy::Input::Left:   return (bool)(data & 0x02);
  case GameBoy::Input::Right:  return (bool)(data & 0x01);
  }

  return 0;
}
