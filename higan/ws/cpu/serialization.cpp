auto CPU::serialize(serializer& s) -> void {
  V30MZ::serialize(s);

  s.integer(r.dmaSource);
  s.integer(r.dmaTarget);
  s.integer(r.dmaLength);
  s.integer(r.dmaEnable);
  s.integer(r.dmaMode);
  s.integer(r.interruptBase);
  s.integer(r.interruptEnable);
  s.integer(r.interruptStatus);
  s.integer(r.ypadEnable);
  s.integer(r.xpadEnable);
  s.integer(r.buttonEnable);
}
