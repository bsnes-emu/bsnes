auto APU::Noise::dac_enable() const -> bool {
  return (envelope_volume || envelope_direction);
}

auto APU::Noise::get_period() const -> uint {
  static const uint table[] = {4, 8, 16, 24, 32, 40, 48, 56};
  return table[divisor] << frequency;
}

auto APU::Noise::run() -> void {
  if(period && --period == 0) {
    period = get_period();
    if(frequency < 14) {
      bool bit = (lfsr ^ (lfsr >> 1)) & 1;
      lfsr = (lfsr >> 1) ^ (bit << (narrow_lfsr ? 6 : 14));
    }
  }

  uint4 sample = (lfsr & 1) ? (uint4)0 : volume;
  if(enable == false) sample = 0;

  output = sample;
}

auto APU::Noise::clock_length() -> void {
  if(counter) {
    if(++length == 0) enable = false;
  }
}

auto APU::Noise::clock_envelope() -> void {
  if(enable && envelope_frequency && --envelope_period == 0) {
    envelope_period = envelope_frequency;
    if(envelope_direction == 0 && volume >  0) volume--;
    if(envelope_direction == 1 && volume < 15) volume++;
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
    return envelope_volume << 4 | envelope_direction << 3 | envelope_frequency;
  }

  if(addr == 0xff22) {  //NR43
    return frequency << 4 | narrow_lfsr << 3 | divisor;
  }

  if(addr == 0xff23) {  //NR44
    return 0x80 | counter << 6 | 0x3f;
  }

  return 0xff;
}

auto APU::Noise::write(uint16 addr, uint8 data) -> void {
  if(addr == 0xff20) {  //NR41
    length = data & 0x3f;
  }

  if(addr == 0xff21) {  //NR42
    envelope_volume = data >> 4;
    envelope_direction = data & 0x08;
    envelope_frequency = data & 0x07;
    if(dac_enable() == false) enable = false;
  }

  if(addr == 0xff22) {  //NR43
    frequency = data >> 4;
    narrow_lfsr = data & 0x08;
    divisor = data & 0x07;
    period = get_period();
  }

  if(addr == 0xff23) {  //NR44
    bool initialize = data & 0x80;
    counter = data & 0x40;

    if(initialize) {
      enable = dac_enable();
      lfsr = -1;
      envelope_period = envelope_frequency;
      volume = envelope_volume;
    }
  }
}

auto APU::Noise::power() -> void {
  enable = 0;

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

auto APU::Noise::serialize(serializer& s) -> void {
  s.integer(enable);

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
