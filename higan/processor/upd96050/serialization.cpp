auto uPD96050::serialize(serializer& s) -> void {
  s.array(dataRAM);

  s.array(regs.stack);
  s.integer(regs.pc);
  s.integer(regs.rp);
  s.integer(regs.dp);
  s.integer(regs.sp);

  s.integer(regs.k);
  s.integer(regs.l);
  s.integer(regs.m);
  s.integer(regs.n);
  s.integer(regs.a);
  s.integer(regs.b);

  s.integer(regs.flaga.data);
  s.integer(regs.flagb.data);

  s.integer(regs.tr);
  s.integer(regs.trb);

  s.integer(regs.sr.data);
  s.integer(regs.dr);
  s.integer(regs.si);
  s.integer(regs.so);
}
