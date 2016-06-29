auto CPU::timerStep(uint clocks) -> void {
  for(auto c : range(clocks)) {
    for(auto n : range(4)) {
      auto& timer = regs.timer[n];

      if(timer.pending) {
        timer.pending = false;
        if(timer.control.enable == 1) {
          timer.period = timer.reload;
        }
        continue;
      }

      if(!timer.control.enable || timer.control.cascade) continue;

      static uint mask[] = {0, 63, 255, 1023};
      if((regs.clock & mask[timer.control.frequency]) == 0) {
        timerIncrement(n);
      }
    }

    regs.clock++;
  }
}

auto CPU::timerIncrement(uint n) -> void {
  auto& timer = regs.timer[n];
  if(++timer.period == 0) {
    timer.period = timer.reload;

    if(timer.control.irq) regs.irq.flag |= Interrupt::Timer0 << n;

    if(apu.fifo[0].timer == n) timerRunFIFO(0);
    if(apu.fifo[1].timer == n) timerRunFIFO(1);

    if(n < 3 && regs.timer[n + 1].control.enable && regs.timer[n + 1].control.cascade) {
      timerIncrement(n + 1);
    }
  }
}

auto CPU::timerRunFIFO(uint n) -> void {
  apu.fifo[n].read();
  if(apu.fifo[n].size > 16) return;

  auto& dma = regs.dma[1 + n];
  if(dma.control.enable && dma.control.timingmode == 3) {
    dma.pending = true;
    dma.control.targetmode = 2;
    dma.control.size = 1;
    dma.run.length = 4;
  }
}
