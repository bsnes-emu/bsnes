auto VCE::serialize(serializer& s) -> void {
  Thread::serialize(s);

  s.array(cram.data);
  s.integer(cram.address);

  s.integer(timing.hclock);
  s.integer(timing.vclock);

  s.integer(io.clock);
  s.integer(io.extraLine);
  s.integer(io.grayscale);
}
