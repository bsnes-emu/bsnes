#ifdef APU_CPP

void APU::Noise::run() {
  if(period && --period == 0) {
    period = divisor << frequency;
    if(frequency < 14) {
      bool bit = (lfsr ^ (lfsr >> 1)) & 1;
      lfsr = (lfsr >> 1) ^ (bit << (narrow_lfsr ? 6 : 14));
    }
  }

  uint4 sample = (lfsr & 1) ? 0 : volume;
  if(counter && length == 0) sample = 0;

  output = (sample * 4369) - 32768;
}

void APU::Noise::clock_length() {
  if(counter && length) length--;
}

void APU::Noise::clock_envelope() {
  if(envelope_period && --envelope_period == 0) {
    envelope_period = envelope_frequency;
    if(envelope_direction == 0 && volume >  0) volume--;
    if(envelope_direction == 1 && volume < 15) volume++;
  }
}

void APU::Noise::write(unsigned r, uint8 data) {
  if(r == 1) {
    initial_length = 64 - (data & 0x3f);

    length = initial_length;
  }

  if(r == 2) {
    envelope_volume = data >> 4;
    envelope_direction = data & 0x08;
    envelope_frequency = data & 0x07;
  }

  if(r == 3) {
    frequency = data >> 4;
    narrow_lfsr = data & 0x08;
    divisor = (data & 0x07) << 4;
    if(divisor == 0) divisor = 8;

    period = divisor << frequency;
  }

  if(r == 4) {
    bool initialize = data & 0x80;
    counter = data & 0x40;

    if(initialize) {
      lfsr = ~0U;
      length = initial_length;
      envelope_period = envelope_frequency;
      volume = envelope_volume;
    }
  }
}

void APU::Noise::power() {
  initial_length = 0;
  envelope_volume = 0;
  envelope_direction = 0;
  envelope_frequency = 0;
  frequency = 0;
  narrow_lfsr = 0;
  divisor = 0;
  counter = 0;

  output = 0;
  length = 0;
  envelope_period = 0;
  volume = 0;
  period = 0;
  lfsr = 0;
}

void APU::Noise::serialize(serializer &s) {
  s.integer(initial_length);
  s.integer(envelope_volume);
  s.integer(envelope_direction);
  s.integer(envelope_frequency);
  s.integer(frequency);
  s.integer(narrow_lfsr);
  s.integer(divisor);
  s.integer(counter);

  s.integer(output);
  s.integer(length);
  s.integer(envelope_period);
  s.integer(volume);
  s.integer(period);
  s.integer(lfsr);
}

#endif
