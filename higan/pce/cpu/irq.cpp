auto CPU::IRQ::pending() const -> bool {
  return pendingIRQ;
}

auto CPU::IRQ::vector() const -> uint16 {
  return pendingVector;
}

auto CPU::IRQ::poll() -> void {
  pendingIRQ = false;
  if(cpu.r.p.i) return;

  if(!disableExternal && pendingExternal) {
    pendingIRQ = true;
    pendingVector = 0xfff6;
  } else if(!disableVDC && pendingVDC) {
    pendingIRQ = true;
    pendingVector = 0xfff8;
  } else if(!disableTimer && pendingTimer) {
    pendingIRQ = true;
    pendingVector = 0xfffa;
  }
}

auto CPU::IRQ::level(Line line, bool level) -> void {
  if(line == Line::External) {
    pendingExternal = level;
  }

  if(line == Line::VDC) {
    pendingVDC = level;
  }

  if(line == Line::Timer) {
    pendingTimer = level;
  }
}
