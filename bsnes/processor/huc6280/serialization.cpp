auto HuC6280::serialize(serializer& s) -> void {
  s.integer(r.a);
  s.integer(r.x);
  s.integer(r.y);
  s.integer(r.s);
  s.integer(r.pc);
  s.array(r.mpr);
  s.integer(r.mdr);
  s.integer(r.p.c);
  s.integer(r.p.z);
  s.integer(r.p.i);
  s.integer(r.p.d);
  s.integer(r.p.b);
  s.integer(r.p.t);
  s.integer(r.p.v);
  s.integer(r.p.n);
  s.integer(r.cs);
}
