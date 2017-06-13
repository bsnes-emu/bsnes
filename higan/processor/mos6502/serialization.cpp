auto MOS6502::serialize(serializer& s) -> void {
  s.integer(BCD);
  s.integer(r.a);
  s.integer(r.x);
  s.integer(r.y);
  s.integer(r.s);
  s.integer(r.pc);
  s.integer(r.p.c);
  s.integer(r.p.z);
  s.integer(r.p.i);
  s.integer(r.p.d);
  s.integer(r.p.v);
  s.integer(r.p.n);
  s.integer(r.mdr);
}
