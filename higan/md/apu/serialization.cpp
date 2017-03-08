auto APU::serialize(serializer& s) -> void {
  Z80::serialize(s);
  Thread::serialize(s);

  s.integer(state.enabled);
  s.integer(state.nmiLine);
  s.integer(state.intLine);
}
