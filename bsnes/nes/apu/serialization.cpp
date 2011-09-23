void APU::serialize(serializer &s) {
  Processor::serialize(s);

  rectangle[0].serialize(s);
  rectangle[1].serialize(s);
  triangle.serialize(s);
  dmc.serialize(s);
  frame.serialize(s);

  s.integer(enabled_channels);
}

void APU::Envelope::serialize(serializer &s) {
  s.integer(speed);
  s.integer(use_speed_as_volume);
  s.integer(loop_mode);

  s.integer(reload_decay);
  s.integer(decay_counter);
  s.integer(decay_volume);
}

void APU::Sweep::serialize(serializer &s) {
  s.integer(shift);
  s.integer(decrement);
  s.integer(period);
  s.integer(counter);
  s.integer(enable);
  s.integer(reload);
  s.integer(rectangle_period);
}

void APU::Rectangle::serialize(serializer &s) {
  s.integer(length_counter);

  envelope.serialize(s);
  sweep.serialize(s);

  s.integer(duty);
  s.integer(duty_counter);

  s.integer(period);
  s.integer(period_counter);
}

void APU::Triangle::serialize(serializer &s) {
  s.integer(length_counter);

  s.integer(linear_length);
  s.integer(halt_length_counter);

  s.integer(period);
  s.integer(period_counter);

  s.integer(step_counter);
  s.integer(linear_length_counter);
  s.integer(reload_linear);
}

void APU::Noise::serialize(serializer &s) {
  s.integer(length_counter);

  envelope.serialize(s);

  s.integer(period);
  s.integer(period_counter);

  s.integer(short_mode);
  s.integer(lfsr);
}

void APU::DMC::serialize(serializer &s) {
  s.integer(length_counter);
  s.integer(irq_pending);

  s.integer(period);
  s.integer(period_counter);

  s.integer(irq_enable);
  s.integer(loop_mode);

  s.integer(dac_latch);
  s.integer(addr_latch);
  s.integer(length_latch);

  s.integer(read_addr);
  s.integer(dma_delay_counter);

  s.integer(bit_counter);
  s.integer(have_dma_buffer);
  s.integer(dma_buffer);

  s.integer(have_sample);
  s.integer(sample);
}

void APU::FrameCounter::serialize(serializer &s) {
  s.integer(irq_pending);

  s.integer(mode);
  s.integer(counter);
  s.integer(divider);
}
