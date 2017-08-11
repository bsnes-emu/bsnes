auto CPU::serialize(serializer& s) -> void {
  M68K::serialize(s);
  Thread::serialize(s);

  s.array(ram);

  s.boolean(io.version);
  s.boolean(io.romEnable);
  s.boolean(io.vdpEnable[0]);
  s.boolean(io.vdpEnable[1]);

  s.integer(state.interruptLine);
  s.integer(state.interruptPending);
}
