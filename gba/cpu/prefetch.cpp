auto CPU::prefetch_sync(uint32 addr) -> void {
  if(addr == prefetch.addr) return;

  prefetch.addr = addr;
  prefetch.load = addr;
  prefetch.wait = bus_wait(Half | Nonsequential, prefetch.load);
}

auto CPU::prefetch_step(uint clocks) -> void {
  step(clocks);
  if(!regs.wait.control.prefetch || active.dma) return;

  while(!prefetch.full() && clocks--) {
    if(--prefetch.wait) continue;
    prefetch.slot[prefetch.load >> 1 & 7] = cartridge.read(Half, prefetch.load);
    prefetch.load += 2;
    prefetch.wait = bus_wait(Half | Sequential, prefetch.load);
  }
}

auto CPU::prefetch_wait() -> void {
  if(!regs.wait.control.prefetch || active.dma || prefetch.full()) return;

  prefetch_step(prefetch.wait);
  prefetch.wait = bus_wait(Half | Nonsequential, prefetch.load);
}

auto CPU::prefetch_read() -> uint16 {
  if(prefetch.empty()) prefetch_step(prefetch.wait);
  else prefetch_step(1);

  if(prefetch.full()) prefetch.wait = bus_wait(Half | Sequential, prefetch.load);

  uint16 half = prefetch.slot[prefetch.addr >> 1 & 7];
  prefetch.addr += 2;
  return half;
}
