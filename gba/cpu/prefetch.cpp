auto CPU::prefetch_run() -> void {
  if(prefetch.slots == 8) return;
  prefetch.slots++;
}

auto CPU::prefetch_read(uint32 addr, uint32 size) -> maybe<uint32> {
  if(prefetch.slots >= (size == Word ? 2 : 1)) {
    prefetch.slots -= (size == Word ? 2 : 1);
    return cartridge.read(addr, size);
  }
  prefetch.slots = 0;
  return nothing;
}

auto CPU::prefetch_take() -> uint16 {
  return 0;
}
