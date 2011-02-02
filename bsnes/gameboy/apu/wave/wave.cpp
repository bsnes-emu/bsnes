#ifdef APU_CPP

void APU::Wave::run() {
  if(period && --period == 0) {
    period = 2 * (2048 - frequency);
    pattern_offset = (pattern_offset + 1) & 31;
    pattern_sample = pattern[pattern_offset];
  }

  uint4 sample = pattern_sample;
  if(counter && length == 0) sample = 0;
  if(enable == false) sample = 0;

  output = (sample * 4369) - 32768;
  switch(volume) {
    case 0: output *= 0.00; break;
    case 1: output *= 1.00; break;
    case 2: output *= 0.50; break;
    case 3: output *= 0.25; break;
  }
}

void APU::Wave::clock_length() {
  if(counter && length) length--;
}

void APU::Wave::write(unsigned r, uint8 data) {
  if(r == 0) {
    dac_enable = data & 0x80;

    if(dac_enable == false) enable = false;
  }

  if(r == 1) {
    initial_length = 256 - data;

    length = initial_length;
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

    if(initialize && dac_enable) {
      enable = true;
      pattern_offset = 0;
      length = initial_length;
    }
  }

  period = 2 * (2048 - frequency);
}

void APU::Wave::write_pattern(unsigned p, uint8 data) {
  p <<= 1;
  pattern[p + 0] = (data >> 4) & 15;
  pattern[p + 1] = (data >> 0) & 15;
}

void APU::Wave::power() {
  dac_enable = 0;
  initial_length = 0;
  volume = 0;
  frequency = 0;
  counter = 0;

  random_cyclic r;
  foreach(n, pattern) n = r() & 15;

  output = 0;
  enable = 0;
  length = 0;
  period = 0;
  pattern_offset = 0;
  pattern_sample = 0;
}

void APU::Wave::serialize(serializer &s) {
  s.integer(dac_enable);
  s.integer(initial_length);
  s.integer(volume);
  s.integer(frequency);
  s.integer(counter);
  s.array(pattern);

  s.integer(output);
  s.integer(enable);
  s.integer(length);
  s.integer(period);
  s.integer(pattern_offset);
  s.integer(pattern_sample);
}

#endif
