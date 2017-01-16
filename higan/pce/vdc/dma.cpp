auto VDC::DMA::step(uint clocks) -> void {
  while(clocks--) {
    if(vramActive) {
      uint16 data = vdc.vramRead(source);
      vdc.vramWrite(target, data);
      sourceIncrementMode == 0 ? source++ : source--;
      targetIncrementMode == 0 ? target++ : target--;
      if(!--length) {
        vramActive = false;
        vdc.irq.raise(VDC::IRQ::Line::TransferVRAM);
      }
    }

    if(satbActive) {
      uint16 data = vdc.vramRead(satbSource + satbTarget);
      vdc.vramWrite(satbTarget++, data);
      if(satbTarget == 256) {
        satbActive = false;
        satbPending = satbRepeat;
        vdc.irq.raise(VDC::IRQ::Line::TransferSATB);
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
  satbTarget = 0;
}

auto VDC::DMA::satbQueue() -> void {
  satbPending = true;
}
