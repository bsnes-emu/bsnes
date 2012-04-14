void CPU::timer_step(unsigned clocks) {
  for(unsigned c = 0; c < clocks; c++) {
    for(unsigned n = 0; n < 4; n++) {
      auto &timer = regs.timer[n];
      if(timer.control.enable == false || timer.control.cascade == true) continue;

      static unsigned mask[] = { 0, 63, 255, 1023 };
      if((regs.clock & mask[timer.control.frequency]) == 0) {
        timer_increment(n);
      }
    }

    regs.clock++;
  }
}

void CPU::timer_increment(unsigned n) {
  auto &timer = regs.timer[n];
  if(++timer.period == 0) {
    timer.period = timer.reload;

    if(timer.control.irq) regs.irq.flag.timer[n] = 1;

    if(apu.fifo[0].timer == n) apu.fifo[0].read();
    if(apu.fifo[1].timer == n) apu.fifo[1].read();

    if(n < 3 && regs.timer[n + 1].control.enable && regs.timer[n + 1].control.cascade) {
      timer_increment(n + 1);
    }
  }
}
