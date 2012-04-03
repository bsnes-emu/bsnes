void CPU::dma_run() {
  for(unsigned n = 0; n < 4; n++) {
    if(regs.dma[n].control.enable == false) continue;
    switch(regs.dma[n].control.timingmode) {
    case 0: break;
    case 1: if(pending.dma.vblank == false) continue; break;
    case 2: if(pending.dma.hblank == false) continue; break;
    case 3: if(pending.dma.hdma == false || n != 3) continue; break;
    }
    dma_transfer(n);
  }

  pending.dma.vblank = false;
  pending.dma.hblank = false;
  pending.dma.hdma   = false;
}

void CPU::dma_transfer(uint2 n) {
  auto &channel = regs.dma[n];

  unsigned size = channel.control.size ? Word : Half;
  unsigned seek = channel.control.size ? 4 : 2;
  uint16 length = channel.length;

  channel.basetarget = channel.target;
  do {
    uint32 word = bus.read(channel.source, size);
    bus.write(channel.target, size, word);

    switch(channel.control.sourcemode) {
    case 0: channel.source += seek; break;
    case 1: channel.source -= seek; break;
    }

    switch(channel.control.targetmode) {
    case 0: channel.target += seek; break;
    case 1: channel.target -= seek; break;
    case 3: channel.target += seek; break;
    }
  } while(--length);
  if(channel.control.targetmode == 3) channel.target = channel.basetarget;

  channel.control.enable = false;
  if(channel.control.irq) regs.irq.flag.dma[n] = 1;
}
