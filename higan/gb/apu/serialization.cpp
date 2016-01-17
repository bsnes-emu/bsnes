auto APU::serialize(serializer& s) -> void {
  Thread::serialize(s);

  square1.serialize(s);
  square2.serialize(s);
  wave.serialize(s);
  noise.serialize(s);
  sequencer.serialize(s);

  s.integer(phase);
  s.integer(cycle);
}
