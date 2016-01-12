auto APU::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.integer(stage);
  s.integer(phase);

  square1.serialize(s);
  square2.serialize(s);
  wave.serialize(s);
  noise.serialize(s);
  master.serialize(s);
}
