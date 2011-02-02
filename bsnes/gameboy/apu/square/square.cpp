#ifdef APU_CPP

void APU::Square::run() {
}

uint8 APU::Square::read(unsigned r) {
  if(r == 0) {
    return (sweep_period << 4) | (sweep_direction << 3) | (sweep_shift << 0);
  }

  if(r == 1) {
    return (duty << 6);
  }

  if(r == 2) {
    return (envelope_volume << 4) | (envelope_direction << 3) | (envelope_period << 0);
  }

  if(r == 4) {
    return (counter << 6);
  }

  return 0x00;
}

void APU::Square::write(unsigned r, uint8 data) {
  if(r == 0) {
    sweep_period = (data >> 4) & 7;
    sweep_direction = data & 0x08;
    sweep_shift = data & 0x07;
  }

  if(r == 1) {
    duty = data >> 6;
    length = data & 0x3f;
  }

  if(r == 2) {
    envelope_volume = data >> 4;
    envelope_direction = data & 0x08;
    envelope_period = data & 0x07;
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

void APU::Square::power() {
  sweep_period = 0;
  sweep_direction = 0;
  sweep_shift = 0;

  duty = 0;
  length = 0;

  envelope_volume = 0;
  envelope_direction = 0;
  envelope_period = 0;

  frequency = 0;

  counter = 0;
}

void APU::Square::serialize(serializer &s) {
  s.integer(sweep_period);
  s.integer(sweep_direction);
  s.integer(sweep_shift);
  s.integer(duty);
  s.integer(length);
  s.integer(envelope_volume);
  s.integer(envelope_direction);
  s.integer(envelope_period);
  s.integer(frequency);
  s.integer(counter);
}

#endif
