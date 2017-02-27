auto YM2612::TimerA::run() -> void {
  if(!enable) return;
  if(++counter) return;

  counter = period;
  line |= irq;
}

auto YM2612::TimerA::power() -> void {
  enable = 0;
  irq = 0;
  line = 0;
  period = 0;
  counter = 0;
}

auto YM2612::TimerB::run() -> void {
  if(!enable) return;
  if(++divider) return;
  if(++counter) return;

  counter = period;
  line |= irq;
}

auto YM2612::TimerB::power() -> void {
  enable = 0;
  irq = 0;
  line = 0;
  period = 0;
  counter = 0;
  divider = 0;
}
