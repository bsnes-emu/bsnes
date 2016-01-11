auto APU::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.integer(sequencer_base);
  s.integer(sequencer_step);

  square1.serialize(s);
  square2.serialize(s);
  wave.serialize(s);
  noise.serialize(s);
  master.serialize(s);
}
