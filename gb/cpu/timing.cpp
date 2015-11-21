//70224 clocks/frame
//  456 clocks/scanline
//  154 scanlines/frame

auto CPU::add_clocks(uint clocks) -> void {
  if(oamdma.active) {
    for(uint n = 0; n < 4 * clocks; n++) {
      bus.write(0xfe00 + oamdma.offset, bus.read((oamdma.bank << 8) + oamdma.offset));
      if(++oamdma.offset == 160) {
        oamdma.active = false;
        break;
      }
    }
  }

  system.clocks_executed += clocks;
  if(system.sgb()) scheduler.exit(Scheduler::ExitReason::StepEvent);

  status.clock += clocks;
  if(status.clock >= 4 * 1024 * 1024) {
    status.clock -= 4 * 1024 * 1024;
    cartridge.mbc3.second();
  }

  //4MHz / N(hz) - 1 = mask
  if((status.clock &   15) == 0) timer_262144hz();
  if((status.clock &   63) == 0)  timer_65536hz();
  if((status.clock &  255) == 0)  timer_16384hz();
  if((status.clock &  511) == 0)   timer_8192hz();
  if((status.clock & 1023) == 0)   timer_4096hz();

  ppu.clock -= clocks * ppu.frequency;
  if(ppu.clock < 0) co_switch(scheduler.active_thread = ppu.thread);

  apu.clock -= clocks * apu.frequency;
  if(apu.clock < 0) co_switch(scheduler.active_thread = apu.thread);
}

auto CPU::timer_262144hz() -> void {
  if(status.timer_enable && status.timer_clock == 1) {
    if(++status.tima == 0) {
      status.tima = status.tma;
      interrupt_raise(Interrupt::Timer);
    }
  }
}

auto CPU::timer_65536hz() -> void {
  if(status.timer_enable && status.timer_clock == 2) {
    if(++status.tima == 0) {
      status.tima = status.tma;
      interrupt_raise(Interrupt::Timer);
    }
  }
}

auto CPU::timer_16384hz() -> void {
  if(status.timer_enable && status.timer_clock == 3) {
    if(++status.tima == 0) {
      status.tima = status.tma;
      interrupt_raise(Interrupt::Timer);
    }
  }

  status.div++;
}

auto CPU::timer_8192hz() -> void {
  if(status.serial_transfer && status.serial_clock) {
    if(--status.serial_bits == 0) {
      status.serial_transfer = 0;
      interrupt_raise(Interrupt::Serial);
    }
  }
}

auto CPU::timer_4096hz() -> void {
  if(status.timer_enable && status.timer_clock == 0) {
    if(++status.tima == 0) {
      status.tima = status.tma;
      interrupt_raise(Interrupt::Timer);
    }
  }
}

auto CPU::hblank() -> void {
  if(status.dma_mode == 1 && status.dma_length && ppu.status.ly < 144) {
    for(auto n : range(16)) {
      dma_write(status.dma_target++, dma_read(status.dma_source++));
    }
    add_clocks(8 << status.speed_double);
    status.dma_length -= 16;
  }
}
