auto PSG::serialize(serializer& s) -> void {
  Thread::serialize(s);

  tone0.serialize(s);
  tone1.serialize(s);
  tone2.serialize(s);
  noise.serialize(s);

  s.integer(select);
  s.array(levels);
}

auto PSG::Tone::serialize(serializer& s) -> void {
  s.integer(volume);
  s.integer(counter);
  s.integer(pitch);
  s.integer(output);
}

auto PSG::Noise::serialize(serializer& s) -> void {
  s.integer(volume);
  s.integer(counter);
  s.integer(pitch);
  s.integer(enable);
  s.integer(rate);
  s.integer(lfsr);
  s.integer(clock);
  s.integer(output);
}
