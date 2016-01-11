auto APU::Square2::dac_enable() const -> bool {
  return (envelope_volume || envelope_direction);
}

auto APU::Square2::run() -> void {
  if(period && --period == 0) {
    period = 2 * (2048 - frequency);
    phase++;
    switch(duty) {
    case 0: duty_output = (phase == 6); break;  //______-_
    case 1: duty_output = (phase >= 6); break;  //______--
    case 2: duty_output = (phase >= 4); break;  //____----
    case 3: duty_output = (phase <= 5); break;  //------__
    }
  }

  uint4 sample = (duty_output ? volume : (uint4)0);
  if(enable == false) sample = 0;

  output = sample;
}

auto APU::Square2::clock_length() -> void {
  if(counter) {
    if(++length == 0) enable = false;
  }
}

auto APU::Square2::clock_envelope() -> void {
  if(enable && envelope_frequency && --envelope_period == 0) {
    envelope_period = envelope_frequency;
    if(envelope_direction == 0 && volume >  0) volume--;
    if(envelope_direction == 1 && volume < 15) volume++;
  }
}

auto APU::Square2::read(uint16 addr) -> uint8 {
  if(addr == 0xff15) {  //NR20
    return 0xff;
  }

  if(addr == 0xff16) {  //NR21
    return duty << 6 | 0x3f;
  }

  if(addr == 0xff17) {  //NR22
    return envelope_volume << 4 | envelope_direction << 3 | envelope_frequency;
  }

  if(addr == 0xff18) {  //NR23
    return 0xff;
  }

  if(addr == 0xff19) {  //NR24
    return 0x80 | counter << 6 | 0x3f;
  }

  return 0xff;
}

auto APU::Square2::write(uint16 addr, uint8 data) -> void {
  if(addr == 0xff16) {  //NR21
    duty = data >> 6;
    length = (data & 0x3f);
  }

  if(addr == 0xff17) {  //NR22
    envelope_volume = data >> 4;
    envelope_direction = data & 0x08;
    envelope_frequency = data & 0x07;
    if(dac_enable() == false) enable = false;
  }

  if(addr == 0xff18) {  //NR23
    frequency = (frequency & 0x0700) | data;
  }

  if(addr == 0xff19) {  //NR24
    bool initialize = data & 0x80;
    counter = data & 0x40;
    frequency = ((data & 7) << 8) | (frequency & 0x00ff);

    if(initialize) {
      enable = dac_enable();
      period = 2 * (2048 - frequency);
      envelope_period = envelope_frequency;
      volume = envelope_volume;
    }
  }
}

auto APU::Square2::power() -> void {
  enable = 0;

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

auto APU::Square2::serialize(serializer& s) -> void {
  s.integer(enable);

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
