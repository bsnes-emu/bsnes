auto APU::serialize(serializer& s) -> void {
  Thread::serialize(s);

  filter.serialize(s);

  pulse[0].serialize(s);
  pulse[1].serialize(s);
  triangle.serialize(s);
  dmc.serialize(s);
  frame.serialize(s);

  s.integer(enabled_channels);
  s.integer(cartridge_sample);
}

auto APU::Filter::serialize(serializer& s) -> void {
  s.integer(hipass_strong);
  s.integer(hipass_weak);
  s.integer(lopass);
}

auto APU::FrameCounter::serialize(serializer& s) -> void {
  s.integer(irq_pending);

  s.integer(mode);
  s.integer(counter);
  s.integer(divider);
}
