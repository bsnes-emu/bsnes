#ifdef APU_CPP

void APU::Wave::run() {
}

uint8 APU::Wave::read(unsigned r) {
  if(r == 0) {
    return (off << 7);
  }

  if(r == 2) {
    return (volume << 5);
  }

  if(r == 4) {
    return (counter << 6);
  }

  return 0x00;
}

void APU::Wave::write(unsigned r, uint8 data) {
  if(r == 0) {
    off = data & 0x80;
  }

  if(r == 1) {
    length = data;
  }

  if(r == 2) {
    volume = (data >> 5) & 3;
  }

  if(r == 3) {
    frequency = (frequency & 0x0700) | data;
  }

  if(r == 4) {
    bool initialize = data & 0x80;
    counter = data & 0x40;
    frequency = ((data & 7) << 8) | (frequency & 0x00ff);
  }
}

uint8 APU::Wave::read_pattern(unsigned p) {
  p <<= 1;
  return (pattern[p + 0] << 4) | (pattern[p + 1] << 0);
}

void APU::Wave::write_pattern(unsigned p, uint8 data) {
  p <<= 1;
  pattern[p + 0] = data >> 4;
  pattern[p + 1] = data >> 0;
}

void APU::Wave::power() {
  off = 0;
  length = 0;
  volume = 0;
  frequency = 0;
  counter = 0;
  foreach(n, pattern) n = 0;
}

void APU::Wave::serialize(serializer &s) {
  s.integer(off);
  s.integer(length);
  s.integer(volume);
  s.integer(frequency);
  s.integer(counter);
  s.array(pattern);
}

#endif
