auto APU::Noise::dacEnable() const -> bool {
  return (envelopeVolume || envelopeDirection);
}

auto APU::Noise::getPeriod() const -> uint {
  static const uint table[] = {4, 8, 16, 24, 32, 40, 48, 56};
  return table[divisor] << frequency;
}

auto APU::Noise::run() -> void {
  if(period && --period == 0) {
    period = getPeriod();
    if(frequency < 14) {
      bool bit = (lfsr ^ (lfsr >> 1)) & 1;
      lfsr = (lfsr >> 1) ^ (bit << (narrow ? 6 : 14));
    }
  }

  uint4 sample = lfsr & 1 ? 0 : (uint)volume;
  if(!enable) sample = 0;

  output = sample;
}

auto APU::Noise::clockLength() -> void {
  if(counter) {
    if(length && --length == 0) enable = false;
  }
}

auto APU::Noise::clockEnvelope() -> void {
  if(enable && envelopeFrequency && --envelopePeriod == 0) {
    envelopePeriod = envelopeFrequency ? (uint)envelopeFrequency : 8;
    if(envelopeDirection == 0 && volume >  0) volume--;
    if(envelopeDirection == 1 && volume < 15) volume++;
  }
}

auto APU::Noise::read(uint16 addr) -> uint8 {
  if(addr == 0xff1f) {  //NR40
    return 0xff;
  }

  if(addr == 0xff20) {  //NR41
    return 0xff;
  }

  if(addr == 0xff21) {  //NR42
    return envelopeVolume << 4 | envelopeDirection << 3 | envelopeFrequency;
  }

  if(addr == 0xff22) {  //NR43
    return frequency << 4 | narrow << 3 | divisor;
  }

  if(addr == 0xff23) {  //NR44
    return 0x80 | counter << 6 | 0x3f;
  }

  return 0xff;
}

auto APU::Noise::write(uint16 addr, uint8 data) -> void {
  if(addr == 0xff20) {  //NR41
    length = 64 - bits(data,0-5);
  }

  if(addr == 0xff21) {  //NR42
    envelopeVolume    = bits(data,4-7);
    envelopeDirection = bit1(data,3);
    envelopeFrequency = bits(data,0-2);
    if(!dacEnable()) enable = false;
  }

  if(addr == 0xff22) {  //NR43
    frequency = bits(data,4-7);
    narrow    = bit1(data,3);
    divisor   = bits(data,0-2);
    period = getPeriod();
  }

  if(addr == 0xff23) {  //NR44
    if(bit1(apu.phase,0) && !counter && bit1(data,6)) {
      if(length && --length == 0) enable = false;
    }

    counter = bit1(data,6);

    if(bit1(data,7)) {
      enable = dacEnable();
      lfsr = -1;
      envelopePeriod = envelopeFrequency ? (uint)envelopeFrequency : 8;
      volume = envelopeVolume;

      if(!length) {
        length = 64;
        if(bit1(apu.phase,0) && counter) length--;
      }
    }
  }
}

auto APU::Noise::power(bool initializeLength) -> void {
  enable = 0;

  envelopeVolume = 0;
  envelopeDirection = 0;
  envelopeFrequency = 0;
  frequency = 0;
  narrow = 0;
  divisor = 0;
  counter = 0;

  output = 0;
  envelopePeriod = 0;
  volume = 0;
  period = 0;
  lfsr = 0;

  if(initializeLength) length = 64;
}

auto APU::Noise::serialize(serializer& s) -> void {
  s.integer(enable);

  s.integer(envelopeVolume);
  s.integer(envelopeDirection);
  s.integer(envelopeFrequency);
  s.integer(frequency);
  s.integer(narrow);
  s.integer(divisor);
  s.integer(counter);

  s.integer(output);
  s.integer(length);
  s.integer(envelopePeriod);
  s.integer(volume);
  s.integer(period);
  s.integer(lfsr);
}
