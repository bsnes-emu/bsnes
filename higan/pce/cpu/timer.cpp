auto CPU::Timer::start() -> void {
  value = latch;
  clock = 0;
}

auto CPU::Timer::step(uint clocks) -> void {
  if(!enable) return;
  clock += clocks;
  while(clock >= 1024) {
    clock -= 1024;
    if(!value--) {
      value = latch;
      line = 1;
    }
  }
}
