auto CPU::dmaTransfer() -> void {
  //length of 0 or SRAM source address cause immediate termination
  if(r.dmaLength == 0 || r.dmaSource.b2 == 1) {
    r.dmaEnable = false;
    return;
  }

  wait(5);
  while(r.dmaLength) {
    wait(2);
    uint16 data = 0;
    //once DMA is started; SRAM reads still incur time penalty, but do not transfer
    if(r.dmaSource.b2 != 1) {
      data |= read(r.dmaSource + 0) << 0;
      data |= read(r.dmaSource + 1) << 8;
      write(r.dmaTarget + 0, data >> 0);
      write(r.dmaTarget + 1, data >> 8);
    }
    if(r.dmaMode == 0) {
      r.dmaSource += 2;
      r.dmaTarget += 2;
    } else {
      r.dmaSource -= 2;
      r.dmaTarget -= 2;
    }
    r.dmaLength -= 2;
  };
  r.dmaEnable = false;
}
