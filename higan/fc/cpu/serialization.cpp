auto CPU::serialize(serializer& s) -> void {
  MOS6502::serialize(s);
  Thread::serialize(s);

  s.array(ram);

  s.integer(io.interruptPending);
  s.integer(io.nmiPending);
  s.integer(io.nmiLine);
  s.integer(io.irqLine);
  s.integer(io.apuLine);

  s.integer(io.rdyLine);
  s.integer(io.rdyAddrValid);
  s.integer(io.rdyAddrValue);

  s.integer(io.oamdmaPending);
  s.integer(io.oamdmaPage);
}
