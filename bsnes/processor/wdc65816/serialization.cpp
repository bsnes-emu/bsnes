auto WDC65816::serialize(serializer& s) -> void {
  s.integer(r.pc);

  s.integer(r.a);
  s.integer(r.x);
  s.integer(r.y);
  s.integer(r.z);
  s.integer(r.s);
  s.integer(r.d);
  s.integer(r.b);

  s.integer(r.p.c);
  s.integer(r.p.z);
  s.integer(r.p.i);
  s.integer(r.p.d);
  s.integer(r.p.x);
  s.integer(r.p.m);
  s.integer(r.p.v);
  s.integer(r.p.n);
  s.integer(r.e);

  s.integer(r.irq);
  s.integer(r.wai);
  s.integer(r.stp);
  s.integer(r.mar);
  s.integer(r.mdr);
  s.integer(r.vector);
}
