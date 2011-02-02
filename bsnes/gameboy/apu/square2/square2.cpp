#ifdef APU_CPP

void APU::Square2::run() {
  if(period && --period == 0) {
    period = 4 * (2048 - frequency);
    phase = (phase + 1) & 7;
    switch(duty) {
      case 0: duty_output = (phase == 6); break;  //______-_
      case 1: duty_output = (phase >= 6); break;  //______--
      case 2: duty_output = (phase >= 4); break;  //____----
      case 3: duty_output = (phase <= 5); break;  //------__
    }
  }

  uint4 sample = (duty_output ? volume : 0);
  if(counter && length == 0) sample = 0;

  output = (sample * 4369) - 32768;
}

void APU::Square2::clock_length() {
  if(counter && length) length--;
}

void APU::Square2::clock_envelope() {
  if(envelope_period && --envelope_period == 0) {
    envelope_period = envelope_frequency;
    if(envelope_direction == 0 && volume >  0) volume--;
    if(envelope_direction == 1 && volume < 15) volume++;
  }
}

void APU::Square2::write(unsigned r, uint8 data) {
  if(r == 1) {
    duty = data >> 6;
    length = data & 0x3f;
  }

  if(r == 2) {
    envelope_volume = data >> 4;
    envelope_direction = data & 0x08;
    envelope_frequency = data & 0x07;
  }

  if(r == 3) {
    frequency = (frequency & 0x0700) | data;
  }

  if(r == 4) {
    bool initialize = data & 0x80;
    counter = data & 0x40;
    frequency = ((data & 7) << 8) | (frequency & 0x00ff);

    if(initialize) {
      envelope_period = envelope_frequency;
      volume = envelope_volume;
    }
  }

  period = 4 * (2048 - frequency);
}

void APU::Square2::power() {
  duty = 0;
  length = 0;
  envelope_volume = 0;
  envelope_direction = 0;
  envelope_frequency = 0;
  frequency = 0;
  counter = 0;

  output = 0;
  duty_output = 0;
  phase = 0;
  period = 0;
  envelope_period = 0;
  volume = 0;
}

void APU::Square2::serialize(serializer &s) {
  s.integer(duty);
  s.integer(length);
  s.integer(envelope_volume);
  s.integer(envelope_direction);
  s.integer(envelope_frequency);
  s.integer(frequency);
  s.integer(counter);

  s.integer(output);
  s.integer(duty_output);
  s.integer(phase);
  s.integer(period);
  s.integer(envelope_period);
  s.integer(volume);
}

#endif
