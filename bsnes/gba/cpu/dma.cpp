void CPU::dma_run() {
  for(unsigned n = 0; n < 4; n++) {
    auto &dma = regs.dma[n];

    if(dma.control.enable == false) continue;

    switch(dma.control.timingmode) {
    case 0: break;
    case 1: if(pending.dma.vblank == false) continue; break;
    case 2: if(pending.dma.hblank == false) continue; break;
    case 3:
      if(n == 0) {
        continue;
      }
      if(n == 1 || n == 2) {
        if(apu.fifo[n - 1].size > 16) continue;
        dma.control.targetmode = 2;
        dma.control.size = 1;
        dma.run.length = 4;
      }
      if(n == 3) {
        if(pending.dma.hdma == false) continue;
      }
    }

    dma_transfer(dma);
    if(dma.control.irq) regs.irq.flag.dma[n] = 1;
  }

  pending.dma.vblank = false;
  pending.dma.hblank = false;
  pending.dma.hdma   = false;
}

void CPU::dma_transfer(Registers::DMA &dma) {
  unsigned size = dma.control.size ? Word : Half;
  unsigned seek = dma.control.size ? 4 : 2;

  do {
    uint32 word = bus.read(dma.run.source, size);
    bus.write(dma.run.target, size, word);
    step(2);

    switch(dma.control.sourcemode) {
    case 0: dma.run.source += seek; break;
    case 1: dma.run.source -= seek; break;
    }

    switch(dma.control.targetmode) {
    case 0: dma.run.target += seek; break;
    case 1: dma.run.target -= seek; break;
    case 3: dma.run.target += seek; break;
    }
  } while(--dma.run.length);

  if(dma.control.targetmode == 3) dma.run.target = dma.target;
  if(dma.control.repeat == 1) dma.run.length = dma.length;
  if(dma.control.repeat == 0) dma.control.enable = false;
}
