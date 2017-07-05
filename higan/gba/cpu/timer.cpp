auto CPU::Timer::run() -> void {
  if(cpu.stopped()) return;

  if(pending) {
    pending = false;
    if(enable) period = reload;
    return;
  }

  if(!enable || cascade) return;

  static const uint mask[] = {0, 63, 255, 1023};
  if((cpu.clock() & mask[frequency]) == 0) step();
}

auto CPU::Timer::step() -> void {
  if(++period == 0) {
    period = reload;

    if(irq) cpu.irq.flag |= CPU::Interrupt::Timer0 << id;

    if(apu.fifo[0].timer == id) cpu.runFIFO(0);
    if(apu.fifo[1].timer == id) cpu.runFIFO(1);

    if(id < 3 && cpu.timer[id + 1].enable && cpu.timer[id + 1].cascade) {
      cpu.timer[id + 1].step();
    }
  }
}

auto CPU::runFIFO(uint n) -> void {
  apu.fifo[n].read();
  if(apu.fifo[n].size > 16) return;

  auto& dma = this->dma[1 + n];
  if(dma.enable && dma.timingMode == 3) {
    dma.active = true;
    dma.waiting = 2;
    dma.targetMode = 2;
    dma.size = 1;
    dma.latch.length = 4;
  }
}
