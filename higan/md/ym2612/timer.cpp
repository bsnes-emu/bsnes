auto YM2612::TimerA::run() -> void {
  if(!enable) return;
  if(++counter) return;

  counter = period;
  line |= irq;
}

auto YM2612::TimerB::run() -> void {
  if(!enable) return;
  if(++divider) return;
  if(++counter) return;

  counter = period;
  line |= irq;
}
