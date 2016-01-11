auto APU::Square1::dac_enable() const -> bool {
  return (envelope_volume || envelope_direction);
}

auto APU::Square1::run() -> void {
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

auto APU::Square1::sweep(bool update) -> void {
  if(sweep_enable == false) return;

  sweep_negate = sweep_direction;
  uint delta = frequency_shadow >> sweep_shift;
  int freq = frequency_shadow + (sweep_negate ? -delta : delta);

  if(freq > 2047) {
    enable = false;
  } else if(sweep_shift && update) {
    frequency_shadow = freq;
    frequency = freq & 2047;
    period = 2 * (2048 - frequency);
  }
}

auto APU::Square1::clock_length() -> void {
  if(counter) {
    if(++length == 0) enable = false;
  }
}

auto APU::Square1::clock_sweep() -> void {
  if(enable && sweep_frequency && --sweep_period == 0) {
    sweep_period = sweep_frequency;
    sweep(1);
    sweep(0);
  }
}

auto APU::Square1::clock_envelope() -> void {
  if(enable && envelope_frequency && --envelope_period == 0) {
    envelope_period = envelope_frequency;
    if(envelope_direction == 0 && volume >  0) volume--;
    if(envelope_direction == 1 && volume < 15) volume++;
  }
}

auto APU::Square1::read(uint16 addr) -> uint8 {
  if(addr == 0xff10) {  //NR10
    return 0x80 | sweep_frequency << 4 | sweep_direction << 3 | sweep_shift;
  }

  if(addr == 0xff11) {  //NR11
    return duty << 6 | 0x3f;
  }

  if(addr == 0xff12) {  //NR12
    return envelope_volume << 4 | envelope_direction << 3 | envelope_frequency;
  }

  if(addr == 0xff13) {  //NR13
    return 0xff;
  }

  if(addr == 0xff14) {  //NR14
    return 0x80 | counter << 6 | 0x3f;
  }

  return 0xff;
}

auto APU::Square1::write(uint16 addr, uint8 data) -> void {
  if(addr == 0xff10) {  //NR10
    if(sweep_negate && sweep_direction && !(data & 0x08)) enable = false;
    sweep_frequency = (data >> 4) & 7;
    sweep_direction = data & 0x08;
    sweep_shift = data & 0x07;
  }

  if(addr == 0xff11) {  //NR11
    duty = data >> 6;
    length = data & 0x3f;
  }

  if(addr == 0xff12) {  //NR12
    envelope_volume = data >> 4;
    envelope_direction = data & 0x08;
    envelope_frequency = data & 0x07;
    if(dac_enable() == false) enable = false;
  }

  if(addr == 0xff13) {  //NR13
    frequency = (frequency & 0x0700) | data;
  }

  if(addr == 0xff14) {  //NR14
    bool initialize = data & 0x80;
    counter = data & 0x40;
    frequency = ((data & 7) << 8) | (frequency & 0x00ff);

    if(initialize) {
      enable = dac_enable();
      period = 2 * (2048 - frequency);
      envelope_period = envelope_frequency;
      volume = envelope_volume;
      frequency_shadow = frequency;
      sweep_period = sweep_frequency;
      sweep_enable = sweep_period || sweep_shift;
      sweep_negate = false;
      if(sweep_shift) sweep(0);
    }
  }
}

auto APU::Square1::power() -> void {
  enable = 0;

  sweep_frequency = 0;
  sweep_direction = 0;
  sweep_shift = 0;
  sweep_negate = 0;
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
  sweep_period = 0;
  frequency_shadow = 0;
  sweep_enable = 0;
  volume = 0;
}

auto APU::Square1::serialize(serializer& s) -> void {
  s.integer(enable);

  s.integer(sweep_frequency);
  s.integer(sweep_direction);
  s.integer(sweep_shift);
  s.integer(sweep_negate);
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
  s.integer(sweep_period);
  s.integer(frequency_shadow);
  s.integer(sweep_enable);
  s.integer(volume);
}
