auto V30MZ::serialize(serializer& s) -> void {
  s.integer(state.halt);
  s.integer(state.poll);
  s.integer(state.prefix);

  s.integer(opcode);
  if(s.mode() == serializer::Save) {
    uint8 _prefixes[7] = {0};
    uint8 _prefixCount = prefixes.size();
    for(auto n : range(prefixes)) _prefixes[n] = prefixes[n];
    s.integer(_prefixCount);
    s.array(_prefixes);
  } else {
    uint8 _prefixes[7] = {0};
    uint8 _prefixCount = 0;
    s.integer(_prefixCount);
    s.array(_prefixes);
    prefixes.resize(_prefixCount);
    for(auto n : range(prefixes)) prefixes[n] = _prefixes[n];
  }

  s.integer(modrm.mod);
  s.integer(modrm.reg);
  s.integer(modrm.mem);
  s.integer(modrm.segment);
  s.integer(modrm.address);

  s.integer(r.ax);
  s.integer(r.cx);
  s.integer(r.dx);
  s.integer(r.bx);
  s.integer(r.sp);
  s.integer(r.bp);
  s.integer(r.si);
  s.integer(r.di);
  s.integer(r.es);
  s.integer(r.cs);
  s.integer(r.ss);
  s.integer(r.ds);
  s.integer(r.ip);
  s.integer(r.f.data);
}
