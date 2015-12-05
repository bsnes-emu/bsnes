auto APU::Noise::clock_length() -> void {
  if(envelope.loop_mode == 0) {
    if(length_counter > 0) length_counter--;
  }
}

auto APU::Noise::clock() -> uint8 {
  if(length_counter == 0) return 0;

  uint8 result = (lfsr & 1) ? envelope.volume() : 0;

  if(--period_counter == 0) {
    uint feedback;

    if(short_mode) {
      feedback = ((lfsr >> 0) & 1) ^ ((lfsr >> 6) & 1);
    } else {
      feedback = ((lfsr >> 0) & 1) ^ ((lfsr >> 1) & 1);
    }

    lfsr = (lfsr >> 1) | (feedback << 14);
    period_counter = apu.ntsc_noise_period_table[period];
  }

  return result;
}

auto APU::Noise::power() -> void {
}

auto APU::Noise::reset() -> void {
  length_counter = 0;

  envelope.speed = 0;
  envelope.use_speed_as_volume = 0;
  envelope.loop_mode = 0;
  envelope.reload_decay = 0;
  envelope.decay_counter = 0;
  envelope.decay_volume = 0;

  period = 0;
  period_counter = 1;
  short_mode = 0;
  lfsr = 1;
}

auto APU::Noise::serialize(serializer& s) -> void {
  s.integer(length_counter);

  envelope.serialize(s);

  s.integer(period);
  s.integer(period_counter);

  s.integer(short_mode);
  s.integer(lfsr);
}
