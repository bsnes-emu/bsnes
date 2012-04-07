void CPU::dma_run() {
  for(unsigned n = 0; n < 4; n++) {
    auto &dma = regs.dma[n];

    if(dma.control.enable == false) {
      dma.active = false;
      continue;
    }

    if(dma.active == false) {
      dma.active = true;
      dma.run.target = dma.target;
      dma.run.source = dma.source;
      dma.run.length = dma.length;
      step(2);
    }

    switch(dma.control.timingmode) {
    case 0: break;
    case 1: if(pending.dma.vblank == false) continue; break;
    case 2: if(pending.dma.hblank == false) continue; break;
    case 3: if(pending.dma.hdma == false || n != 3) continue; break;
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
  if(dma.control.repeat == 0) dma.active = false, dma.control.enable = false;
}
