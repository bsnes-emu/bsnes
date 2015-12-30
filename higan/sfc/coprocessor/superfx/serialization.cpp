auto SuperFX::serialize(serializer& s) -> void {
  GSU::serialize(s);
  Thread::serialize(s);

  s.array(ram.data(), ram.size());
  s.integer(r15_modified);
}
