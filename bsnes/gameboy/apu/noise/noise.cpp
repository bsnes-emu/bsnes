#ifdef APU_CPP

void APU::Noise::run() {
}

uint8 APU::Noise::read(unsigned r) {
  if(r == 2) {
    return (envelope_volume << 4) | (envelope_direction << 3) | (envelope_period << 0);
  }

  if(r == 3) {
    return (period << 4) | (step << 3) | (divisor);
  }

  if(r == 4) {
    return (counter << 6);
  }

  return 0x00;
}

void APU::Noise::write(unsigned r, uint8 data) {
  if(r == 1) {
    length = data & 0x3f;
  }

  if(r == 2) {
    envelope_volume = data >> 4;
    envelope_direction = data & 0x08;
    envelope_period = data & 0x07;
  }

  if(r == 3) {
    period = data >> 4;
    step = data & 0x08;
    divisor = data & 0x07;
  }

  if(r == 4) {
    counter = data & 0x40;
  }
}

void APU::Noise::power() {
  length = 0;
  envelope_volume = 0;
  envelope_direction = 0;
  envelope_period = 0;
  period = 0;
  step = 0;
  divisor = 0;
  counter = 0;
}

void APU::Noise::serialize(serializer &s) {
  s.integer(length);
  s.integer(envelope_volume);
  s.integer(envelope_direction);
  s.integer(envelope_period);
  s.integer(period);
  s.integer(step);
  s.integer(divisor);
  s.integer(counter);
}

#endif
