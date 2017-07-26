//70224 clocks/frame
//  456 clocks/scanline
//  154 scanlines/frame

auto CPU::step(uint clocks) -> void {
  for(auto n : range(clocks)) {
    if(++status.clock == 0) {
      cartridge.second();
    }

    //4MHz / N(hz) - 1 = mask
    status.div++;
    if((status.div &   15) == 0) timer262144hz();
    if((status.div &   63) == 0)  timer65536hz();
    if((status.div &  255) == 0)  timer16384hz();
    if((status.div &  511) == 0)   timer8192hz();
    if((status.div & 1023) == 0)   timer4096hz();

    Thread::step(1);
    synchronize(ppu);
    synchronize(apu);
  }

  if(Model::SuperGameBoy()) {
    system._clocksExecuted += clocks;
    scheduler.exit(Scheduler::Event::Step);
  }
}

auto CPU::timer262144hz() -> void {
  if(status.timerEnable && status.timerClock == 1) {
    if(++status.tima == 0) {
      status.tima = status.tma;
      raise(Interrupt::Timer);
    }
  }
}

auto CPU::timer65536hz() -> void {
  if(status.timerEnable && status.timerClock == 2) {
    if(++status.tima == 0) {
      status.tima = status.tma;
      raise(Interrupt::Timer);
    }
  }
}

auto CPU::timer16384hz() -> void {
  if(status.timerEnable && status.timerClock == 3) {
    if(++status.tima == 0) {
      status.tima = status.tma;
      raise(Interrupt::Timer);
    }
  }
}

auto CPU::timer8192hz() -> void {
  if(status.serialTransfer && status.serialClock) {
    if(--status.serialBits == 0) {
      status.serialTransfer = 0;
      raise(Interrupt::Serial);
    }
  }
}

auto CPU::timer4096hz() -> void {
  if(status.timerEnable && status.timerClock == 0) {
    if(++status.tima == 0) {
      status.tima = status.tma;
      raise(Interrupt::Timer);
    }
  }
}

auto CPU::hblank() -> void {
  if(status.dmaMode == 1 && status.dmaLength && ppu.status.ly < 144) {
    for(auto n : range(16)) {
      writeDMA(status.dmaTarget++, readDMA(status.dmaSource++));
    }
    step(8 << status.speedDouble);
    status.dmaLength -= 16;
  }
}
