auto CPU::dma_run() -> void {
  active.dma = true;

  while(true) {
    bool transferred = false;
    for(auto n : range(4)) {
      auto& dma = regs.dma[n];
      if(dma.pending) {
        dma_exec(dma);
        if(dma.control.irq) regs.irq.flag |= Interrupt::DMA0 << n;
        if(dma.control.drq && n == 3) regs.irq.flag |= Interrupt::Cartridge;
        transferred = true;
        break;
      }
    }
    if(!transferred) break;
  }

  active.dma = false;
}

auto CPU::dma_exec(Registers::DMA& dma) -> void {
  uint seek = dma.control.size ? 4 : 2;
  uint mode = dma.control.size ? Word : Half;
  mode |= dma.run.length == dma.length ? Nonsequential : Sequential;

  if(mode & Nonsequential) {
    if((dma.source & 0x0800'0000) && (dma.target & 0x0800'0000)) {
      //ROM -> ROM transfer
    } else {
      idle();
      idle();
    }
  }

  if(dma.run.source < 0x0200'0000) {
    idle();  //cannot access BIOS
  } else {
    uint32 addr = dma.run.source;
    if(mode & Word) addr &= ~3;
    if(mode & Half) addr &= ~1;
    dma.data = bus_read(mode, addr);
  }

  if(dma.run.target < 0x0200'0000) {
    idle();  //cannot access BIOS
  } else {
    uint32 addr = dma.run.target;
    if(mode & Word) addr &= ~3;
    if(mode & Half) addr &= ~1;
    bus_write(mode, addr, dma.data);
  }

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
