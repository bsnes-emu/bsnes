auto R6502::serialize(serializer& s) -> void {
  s.integer(regs.mdr);
  s.integer(regs.pc);
  s.integer(regs.a);
  s.integer(regs.x);
  s.integer(regs.y);
  s.integer(regs.s);
  s.integer(regs.p.data);

  s.integer(abs.w);
  s.integer(iabs.w);
  s.integer(rd);
  s.integer(zp);
  s.integer(aa);
}
