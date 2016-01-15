auto APU::Wave::getPattern(uint5 offset) const -> uint4 {
  return pattern[offset >> 1] >> (offset & 1 ? 0 : 4);
}

auto APU::Wave::run() -> void {
  if(period && --period == 0) {
    period = 1 * (2048 - frequency);
    patternSample = getPattern(++patternOffset);
  }

  static const uint shift[] = {4, 0, 1, 2};  //0%, 100%, 50%, 25%
  uint4 sample = patternSample >> shift[volume];
  if(!enable) sample = 0;

  output = sample;
}

auto APU::Wave::clockLength() -> void {
  if(counter) {
    if(length && --length == 0) enable = false;
  }
}

auto APU::Wave::read(uint16 addr) -> uint8 {
  if(addr == 0xff1a) {  //NR30
    return dacEnable << 7 | 0x7f;
  }

  if(addr == 0xff1b) {  //NR31
    return 0xff;
  }

  if(addr == 0xff1c) {  //NR32
    return 0x80 | volume << 5 | 0x1f;
  }

  if(addr == 0xff1d) {  //NR33
    return 0xff;
  }

  if(addr == 0xff1e) {  //NR34
    return 0x80 | counter << 6 | 0x3f;
  }

  if(addr >= 0xff30 && addr <= 0xff3f) {
    return pattern[addr & 15];
  }

  return 0xff;
}

auto APU::Wave::write(uint16 addr, uint8 data) -> void {
  if(addr == 0xff1a) {  //NR30
    dacEnable = data & 0x80;
    if(!dacEnable) enable = false;
  }

  if(addr == 0xff1b) {  //NR31
    length = 256 - data;
  }

  if(addr == 0xff1c) {  //NR32
    volume = data >> 5;
  }

  if(addr == 0xff1d) {  //NR33
    frequency = (frequency & 0x0700) | data;
  }

  if(addr == 0xff1e) {  //NR34
    if((apu.phase & 1) && !counter && (data & 0x40)) {
      if(length && --length == 0) enable = false;
    }

    bool initialize = data & 0x80;
    counter = data & 0x40;
    frequency = ((data & 7) << 8) | (frequency & 0x00ff);

    if(initialize) {
      enable = dacEnable;
      period = 1 * (2048 - frequency);
      patternOffset = 0;

      if(!length) {
        length = 256;
        if((apu.phase & 1) && counter) length--;
      }
    }
  }

  if(addr >= 0xff30 && addr <= 0xff3f) {
    pattern[addr & 15] = data;
  }
}

auto APU::Wave::power(bool initializeLength) -> void {
  enable = 0;

  dacEnable = 0;
  volume = 0;
  frequency = 0;
  counter = 0;

  output = 0;
  period = 0;
  patternOffset = 0;
  patternSample = 0;

  if(initializeLength) length = 256;
}

auto APU::Wave::serialize(serializer& s) -> void {
  s.integer(enable);

  s.integer(dacEnable);
  s.integer(volume);
  s.integer(frequency);
  s.integer(counter);
  s.array(pattern);

  s.integer(output);
  s.integer(length);
  s.integer(period);
  s.integer(patternOffset);
  s.integer(patternSample);
}
