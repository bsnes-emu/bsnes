auto APU::Noise::divider() const -> uint {
  if(divisor == 0) return 4;
  return divisor * 8;
}

auto APU::Noise::run() -> void {
  if(period && --period == 0) {
    period = divider() << frequency;
    if(frequency < 14) {
      bool bit = (lfsr ^ (lfsr >> 1)) & 1;
      lfsr = (lfsr >> 1) ^ (bit << (narrowlfsr ? 6 : 14));
    }
  }

  output = volume;
  if(enable == false || (lfsr & 1)) output = 0;
}

auto APU::Noise::clocklength() -> void {
  if(enable && counter) {
    if(++length == 0) enable = false;
  }
}

auto APU::Noise::clockenvelope() -> void {
  if(enable && envelope.frequency && --envelope.period == 0) {
    envelope.period = envelope.frequency;
    if(envelope.direction == 0 && volume >  0) volume--;
    if(envelope.direction == 1 && volume < 15) volume++;
  }
}

auto APU::Noise::read(uint addr) const -> uint8 {
  switch(addr) {
  case 1: return 0;
  case 2: return (envelope.frequency << 0) | (envelope.direction << 3) | (envelope.volume << 4);
  case 3: return (divisor << 0) | (narrowlfsr << 3) | (frequency << 4);
  case 4: return (counter << 6);
  }
  return 0;
}

auto APU::Noise::write(uint addr, uint8 byte) -> void {
  switch(addr) {
  case 1:  //NR41
    length = byte >> 0;
    break;

  case 2:  //NR42
    envelope.frequency = byte >> 0;
    envelope.direction = byte >> 3;
    envelope.volume    = byte >> 4;
    if(!envelope.dacEnable()) enable = false;
    break;

  case 3:  //NR43
    divisor    = byte >> 0;
    narrowlfsr = byte >> 3;
    frequency  = byte >> 4;
    period = divider() << frequency;
    break;

  case 4:  //NR44
    counter    = byte >> 6;
    initialize = byte >> 7;

    if(initialize) {
      enable = envelope.dacEnable();
      lfsr = -1;
      envelope.period = envelope.frequency;
      volume = envelope.volume;
    }

    break;
  }
}

auto APU::Noise::power() -> void {
  envelope.frequency = 0;
  envelope.direction = 0;
  envelope.volume = 0;
  envelope.period = 0;
  length = 0;
  divisor = 0;
  narrowlfsr = 0;
  frequency = 0;
  counter = 0;
  initialize = 0;
  enable = 0;
  lfsr = 0;
  output = 0;
  period = 0;
  volume = 0;
}
