void CPU::timer_tick() {
  for(unsigned n = 0; n < 4; n++) {
    if(regs.timer[n].control.cascade) continue;

    static unsigned mask[] = { 0, 63, 255, 1023 };
    if((regs.clock & mask[regs.timer[n].control.frequency]) == 0) {
      timer_increment(n);
    }
  }

  regs.clock++;
}

void CPU::timer_increment(unsigned n) {
  if(regs.timer[n].control.enable == false) return;

  if(++regs.timer[n].counter == 0) {
    if(regs.timer[n].control.irq) regs.irq.flag.timer[n] = 1;

    if(apu.fifo[0].timer == n) apu.fifo[0].read();
    if(apu.fifo[1].timer == n) apu.fifo[1].read();

    regs.timer[n].counter = regs.timer[n].reload;

    if(n < 3 && regs.timer[n + 1].control.cascade) {
      timer_increment(n + 1);
    }
  }
}
