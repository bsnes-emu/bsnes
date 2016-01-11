//70224 clocks/frame
//  456 clocks/scanline
//  154 scanlines/frame

auto CPU::add_clocks(uint clocks) -> void {
  if(system.sgb()) system.clocks_executed += clocks;

  while(clocks--) {
    if(oamdma.active) {
      uint offset = oamdma.clock++;
      if((offset & 3) == 0) {
        offset >>= 2;
        if(offset == 0) {
          //warm-up
        } else if(offset == 161) {
          //cool-down; disable
          oamdma.active = false;
        } else {
          bus.write(0xfe00 + offset - 1, bus.read((oamdma.bank << 8) + offset - 1));
        }
      }
    }

    if(++status.clock == 0) {
      cartridge.mbc3.second();
    }

    //4MHz / N(hz) - 1 = mask
    status.div++;
    if((status.div &   15) == 0) timer_262144hz();
    if((status.div &   63) == 0)  timer_65536hz();
    if((status.div &  255) == 0)  timer_16384hz();
    if((status.div &  511) == 0)   timer_8192hz();
    if((status.div & 1023) == 0)   timer_4096hz();

    ppu.clock -= ppu.frequency;
    if(ppu.clock < 0) co_switch(scheduler.active_thread = ppu.thread);

    apu.clock -= apu.frequency;
    if(apu.clock < 0) co_switch(scheduler.active_thread = apu.thread);
  }

  if(system.sgb()) scheduler.exit(Scheduler::ExitReason::StepEvent);
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
