auto Bus::serialize(serializer& s) -> void {
  Processor::Z80::Bus::serialize(s);

  s.array(ram);
}
