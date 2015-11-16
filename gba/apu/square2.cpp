auto APU::Square2::read(uint addr) const -> uint8 {
  switch(addr) {
  case 1: return (duty << 6);
  case 2: return (envelope.frequency << 0) | (envelope.direction << 3) | (envelope.volume << 4);
  case 3: return 0;
  case 4: return (counter << 6);
  }
}

auto APU::Square2::write(uint addr, uint8 byte) -> void {
  switch(addr) {
  case 1:  //NR21
    length = byte >> 0;
    duty   = byte >> 6;
    break;

  case 2:  //NR22
    envelope.frequency = byte >> 0;
    envelope.direction = byte >> 3;
    envelope.volume    = byte >> 4;
    if(!envelope.dacEnable()) enable = false;
    break;

  case 3:  //NR23
    frequency = (frequency & 0xff00) | (byte << 0);
    break;

  case 4:  //NR24
    frequency = (frequency & 0x00ff) | (byte << 8);
    counter    = byte >> 6;
    initialize = byte >> 7;

    if(initialize) {
      enable = envelope.dacEnable();
      period = 2 * (2048 - frequency);
      envelope.period = envelope.frequency;
      volume = envelope.volume;
    }

    break;
  }
}

auto APU::Square2::power() -> void {
  envelope.frequency = 0;
  envelope.direction = 0;
  envelope.direction = 0;
  envelope.period = 0;
  enable = 0;
  length = 0;
  duty = 0;
  frequency = 0;
  counter = 0;
  initialize = 0;
  shadowfrequency = 0;
  signal = 0;
  output = 0;
  period = 0;
  phase = 0;
  volume = 0;
}
