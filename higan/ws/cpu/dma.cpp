auto CPU::dmaTransfer() -> void {
  //length of 0 or SRAM source address cause immediate termination
  if(s.dmaLength == 0 || s.dmaSource.b2 == 1) {
    s.dmaEnable = false;
    return;
  }

  wait(5);
  while(s.dmaLength) {
    wait(2);
    uint16 data = 0;
    //once DMA is started; SRAM reads still incur time penalty, but do not transfer
    if(s.dmaSource.b2 != 1) {
      data |= read(s.dmaSource + 0) << 0;
      data |= read(s.dmaSource + 1) << 8;
      write(s.dmaTarget + 0, data >> 0);
      write(s.dmaTarget + 1, data >> 8);
    }
    if(s.dmaMode == 0) {
      s.dmaSource += 2;
      s.dmaTarget += 2;
    } else {
      s.dmaSource -= 2;
      s.dmaTarget -= 2;
    }
    s.dmaLength -= 2;
  };
  s.dmaEnable = false;
}
