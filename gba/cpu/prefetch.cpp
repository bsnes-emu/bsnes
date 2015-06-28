auto CPU::prefetch_stall() -> void {
  prefetch.stalled = true;
}

auto CPU::prefetch_start(uint32 addr) -> void {
  prefetch.stalled = false;
  prefetch.slots   = 0;
  prefetch.input   = 0;
  prefetch.output  = 0;
  prefetch.addr    = addr;
  prefetch.wait    = bus.wait(addr, Half, Nonsequential);
}

auto CPU::prefetch_step(unsigned clocks) -> void {
  if(!regs.wait.control.prefetch || prefetch.stalled) return;

  prefetch.wait -= clocks;
  while(prefetch.wait <= 0) {
    if(prefetch.slots < 8) {
      prefetch.slot[prefetch.output++] = cartridge.read(prefetch.addr, Half);
      prefetch.slots++;
      prefetch.addr += 2;
    }
    prefetch.wait += bus.wait(prefetch.addr, Half, Sequential);
  }
}

auto CPU::prefetch_wait() -> void {
  step(prefetch.wait);
  prefetch_step(prefetch.wait);
}

auto CPU::prefetch_take() -> uint16 {
  if(prefetch.slots) {
    step(1);
    prefetch_step(1);
  } else {
    prefetch_wait();
  }

  prefetch.slots--;
  return prefetch.slot[prefetch.input++];
}
