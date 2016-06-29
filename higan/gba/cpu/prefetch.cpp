auto CPU::prefetchSync(uint32 addr) -> void {
  if(addr == prefetch.addr) return;

  prefetch.addr = addr;
  prefetch.load = addr;
  prefetch.wait = wait(Half | Nonsequential, prefetch.load);
}

auto CPU::prefetchStep(uint clocks) -> void {
  step(clocks);
  if(!regs.wait.control.prefetch || active.dma) return;

  while(!prefetch.full() && clocks--) {
    if(--prefetch.wait) continue;
    prefetch.slot[prefetch.load >> 1 & 7] = cartridge.read(Half, prefetch.load);
    prefetch.load += 2;
    prefetch.wait = wait(Half | Sequential, prefetch.load);
  }
}

auto CPU::prefetchWait() -> void {
  if(!regs.wait.control.prefetch || active.dma || prefetch.full()) return;

  prefetchStep(prefetch.wait);
  prefetch.wait = wait(Half | Nonsequential, prefetch.load);
}

auto CPU::prefetchRead() -> uint16 {
  if(prefetch.empty()) prefetchStep(prefetch.wait);
  else prefetchStep(1);

  if(prefetch.full()) prefetch.wait = wait(Half | Sequential, prefetch.load);

  uint16 half = prefetch.slot[prefetch.addr >> 1 & 7];
  prefetch.addr += 2;
  return half;
}
