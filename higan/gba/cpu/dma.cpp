auto CPU::DMA::run() -> bool {
  if(cpu.stopped() || !active || waiting) return false;

  transfer();
  if(irq) cpu.irq.flag |= CPU::Interrupt::DMA0 << id;
  if(drq && id == 3) cpu.irq.flag |= CPU::Interrupt::Cartridge;
  return true;
}

auto CPU::DMA::transfer() -> void {
  uint seek = size ? 4 : 2;
  uint mode = size ? Word : Half;
  mode |= latch.length == length ? Nonsequential : Sequential;

  if(mode & Nonsequential) {
    if((source & 0x0800'0000) && (target & 0x0800'0000)) {
      //ROM -> ROM transfer
    } else {
      cpu.idle();
      cpu.idle();
    }
  }

  if(latch.source < 0x0200'0000) {
    cpu.idle();  //cannot access BIOS
  } else {
    uint32 addr = latch.source;
    if(mode & Word) addr &= ~3;
    if(mode & Half) addr &= ~1;
    data = cpu._read(mode, addr);
  }

  if(latch.target < 0x0200'0000) {
    cpu.idle();  //cannot access BIOS
  } else {
    uint32 addr = latch.target;
    if(mode & Word) addr &= ~3;
    if(mode & Half) addr &= ~1;
    cpu._write(mode, addr, data);
  }

  switch(sourceMode) {
  case 0: latch.source += seek; break;
  case 1: latch.source -= seek; break;
  }

  switch(targetMode) {
  case 0: latch.target += seek; break;
  case 1: latch.target -= seek; break;
  case 3: latch.target += seek; break;
  }

  if(--latch.length == 0) {
    active = false;
    if(targetMode == 3) latch.target = target;
    if(repeat == 1) latch.length = length;
    if(repeat == 0) enable = false;
  }
}

auto CPU::dmaVblank() -> void {
  for(auto& dma : this->dma) {
    if(dma.enable && dma.timingMode == 1) dma.active = true;
  }
}

auto CPU::dmaHblank() -> void {
  for(auto& dma : this->dma) {
    if(dma.enable && dma.timingMode == 2) dma.active = true;
  }
}

auto CPU::dmaHDMA() -> void {
  auto& dma = this->dma[3];
  if(dma.enable && dma.timingMode == 3) dma.active = true;
}
