auto VDC::DMA::step(uint clocks) -> void {
  while(clocks--) {
    if(vramActive) {
      uint16 data = vdc->vram.read(source);
      vdc->vram.write(target, data);
      sourceIncrementMode == 0 ? source++ : source--;
      targetIncrementMode == 0 ? target++ : target--;
      if(!--length) {
        vramActive = false;
        vdc->irq.raise(VDC::IRQ::Line::TransferVRAM);
      }
    }

    if(satbActive) {
      uint16 data = vdc->vram.read(satbSource + satbOffset);
      vdc->satb.write(satbOffset, data);
      if(++satbOffset == 256) {
        satbActive = false;
        satbOffset = 0;
        satbPending = satbRepeat;
        vdc->irq.raise(VDC::IRQ::Line::TransferSATB);
      }
    }
  }
}

auto VDC::DMA::vramStart() -> void {
  vramActive = true;
}

auto VDC::DMA::satbStart() -> void {
  if(!satbPending) return;
  satbActive = true;
  satbOffset = 0;
}

auto VDC::DMA::satbQueue() -> void {
  satbPending = true;
}
