auto CPU::serialize(serializer& s) -> void {
  M68K::serialize(s);
  Thread::serialize(s);

  s.integer(state.interruptLine);
  s.integer(state.interruptPending);
}
