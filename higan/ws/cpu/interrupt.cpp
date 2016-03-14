auto CPU::poll() -> void {
  if(!V30MZ::r.f.i || !state.poll) return;

  for(int n = 7; n >= 0; n--) {
    if(!r.interruptEnable.bit(n)) continue;
    if(!r.interruptStatus.bit(n)) continue;
    return interrupt(r.interruptBase + n);
  }
}

auto CPU::raise(Interrupt irq) -> void {
  r.interruptStatus.bit((uint)irq) = 1;
}

auto CPU::lower(Interrupt irq) -> void {
  r.interruptStatus.bit((uint)irq) = 0;
}
