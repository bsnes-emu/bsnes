auto CPU::poll() -> void {
  if(!state.poll) return;

  for(int n = 7; n >= 0; n--) {
    if(!r.interruptEnable.bit(n)) continue;
    if(!r.interruptStatus.bit(n)) continue;
    state.halt = false;
    if(V30MZ::r.f.i) interrupt(r.interruptBase + n);
    return;
  }
}

auto CPU::raise(Interrupt irq) -> void {
  if(!r.interruptEnable.bit((uint)irq)) return;
  r.interruptStatus.bit((uint)irq) = 1;
}

auto CPU::lower(Interrupt irq) -> void {
  r.interruptStatus.bit((uint)irq) = 0;
}
