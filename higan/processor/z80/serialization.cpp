auto Z80::serialize(serializer& s) -> void {
  s.integer(r.af.word);
  s.integer(r.bc.word);
  s.integer(r.de.word);
  s.integer(r.hl.word);
  s.integer(r.ix.word);
  s.integer(r.iy.word);
  s.integer(r.ir.word);
  s.integer(r.sp);
  s.integer(r.pc);
  s.integer(r.af_.word);
  s.integer(r.bc_.word);
  s.integer(r.de_.word);
  s.integer(r.hl_.word);
  s.integer(r.ei);
  s.integer(r.halt);
  s.integer(r.iff1);
  s.integer(r.iff2);
  s.integer(r.im);

  //todo: r.hlp is not serializable
}

auto Z80::Bus::serialize(serializer& s) -> void {
  s.integer(_requested);
  s.integer(_granted);
}
