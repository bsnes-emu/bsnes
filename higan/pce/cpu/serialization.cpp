auto CPU::serialize(serializer& s) -> void {
  HuC6280::serialize(s);
  Thread::serialize(s);

  s.array(ram, Model::PCEngine() ? 0x2000 : 0x8000);
  s.array(bram);

  s.integer(irq.disableExternal);
  s.integer(irq.disableVDC);
  s.integer(irq.disableTimer);
  s.integer(irq.pendingIRQ);
  s.integer(irq.pendingVector);

  s.integer(timer.enable);
  s.integer(timer.latch);
  s.integer(timer.value);
  s.integer(timer.clock);
  s.integer(timer.line);

  s.integer(io.mdr);
}
