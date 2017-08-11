auto CPU::serialize(serializer& s) -> void {
  Z80::serialize(s);
  Z80::Bus::serialize(s);
  Thread::serialize(s);

  s.array(ram);

  s.integer(state.nmiLine);
  s.integer(state.intLine);
}
