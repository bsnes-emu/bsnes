auto CPU::dma_run() -> void {
  while(true) {
    bool transferred = false;
    for(auto n : range(4)) {
      auto& dma = regs.dma[n];
      if(dma.pending) {
        dma_exec(dma);
        if(dma.control.irq) regs.irq.flag.dma[n] = 1;
        if(dma.control.drq && n == 3) regs.irq.flag.cartridge = 1;
        transferred = true;
        break;
      }
    }
    if(!transferred) break;
  }
}

auto CPU::dma_exec(Registers::DMA& dma) -> void {
  unsigned size = dma.control.size ? Word : Half;
  unsigned mode = dma.run.length == dma.length ? Nonsequential : Sequential;
  unsigned seek = dma.control.size ? 4 : 2;

  if(mode == Nonsequential) {
    idle();
    idle();
  }

  uint32 word = bus_read(dma.run.source, size, mode);
  bus_write(dma.run.target, size, mode, word);

  switch(dma.control.sourcemode) {
  case 0: dma.run.source += seek; break;
  case 1: dma.run.source -= seek; break;
  }

  switch(dma.control.targetmode) {
  case 0: dma.run.target += seek; break;
  case 1: dma.run.target -= seek; break;
  case 3: dma.run.target += seek; break;
  }

  if(--dma.run.length == 0) {
    dma.pending = false;
    if(dma.control.targetmode == 3) dma.run.target = dma.target;
    if(dma.control.repeat == 1) dma.run.length = dma.length;
    if(dma.control.repeat == 0) dma.control.enable = false;
  }
}

auto CPU::dma_vblank() -> void {
  for(auto& dma : regs.dma) {
    if(dma.control.enable && dma.control.timingmode == 1) dma.pending = true;
  }
}

auto CPU::dma_hblank() -> void {
  for(auto& dma : regs.dma) {
    if(dma.control.enable && dma.control.timingmode == 2) dma.pending = true;
  }
}

auto CPU::dma_hdma() -> void {
  auto& dma = regs.dma[3];
  if(dma.control.enable && dma.control.timingmode == 3) dma.pending = true;
}
