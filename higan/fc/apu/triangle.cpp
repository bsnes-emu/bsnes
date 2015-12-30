auto APU::Triangle::clock_length() -> void {
  if(halt_length_counter == 0) {
    if(length_counter > 0) length_counter--;
  }
}

auto APU::Triangle::clock_linear_length() -> void {
  if(reload_linear) {
    linear_length_counter = linear_length;
  } else if(linear_length_counter) {
    linear_length_counter--;
  }

  if(halt_length_counter == 0) reload_linear = false;
}

auto APU::Triangle::clock() -> uint8 {
  uint8 result = step_counter & 0x0f;
  if((step_counter & 0x10) == 0) result ^= 0x0f;
  if(length_counter == 0 || linear_length_counter == 0) return result;

  if(--period_counter == 0) {
    step_counter++;
    period_counter = period + 1;
  }

  return result;
}

auto APU::Triangle::power() -> void {
  reset();
}

auto APU::Triangle::reset() -> void {
  length_counter = 0;

  linear_length = 0;
  halt_length_counter = 0;
  period = 0;
  period_counter = 1;
  step_counter = 0;
  linear_length_counter = 0;
  reload_linear = 0;
}

auto APU::Triangle::serialize(serializer& s) -> void {
  s.integer(length_counter);

  s.integer(linear_length);
  s.integer(halt_length_counter);

  s.integer(period);
  s.integer(period_counter);

  s.integer(step_counter);
  s.integer(linear_length_counter);
  s.integer(reload_linear);
}
