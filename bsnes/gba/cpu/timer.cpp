void CPU::timer_step(unsigned clocks) {
  for(unsigned n = 0; n < 4; n++) {
    auto &timer = regs.timer[n];
    if(timer.control.enable == false || timer.control.cascade == true) continue;

    timer.counter -= clocks;
    while(timer.counter <= 0) {
      timer_increment(n);
      timer.counter = timer.period();
    }
  }
}

void CPU::timer_increment(unsigned n) {
  if(regs.timer[n].control.irq) regs.irq.flag.timer[n] = 1;

  if(apu.fifo[0].timer == n) apu.fifo[0].read();
  if(apu.fifo[1].timer == n) apu.fifo[1].read();

  if(n < 3 && regs.timer[n + 1].control.enable && regs.timer[n + 1].control.cascade) {
    timer_increment(n + 1);
  }
}
