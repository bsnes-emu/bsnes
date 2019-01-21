auto CPU::serialize(serializer& s) -> void {
  TLCS900H::serialize(s);
  Thread::serialize(s);
}
