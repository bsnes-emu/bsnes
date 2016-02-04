auto CPU::portRead(uint16 addr) -> uint8 {
  //DMA_SRC
  if(addr == 0x0040) return s.dmaSource.b0;
  if(addr == 0x0041) return s.dmaSource.b1;
  if(addr == 0x0042) return s.dmaSource.b2;

  //DMA_DST
  if(addr == 0x0044) return s.dmaTarget.b0;
  if(addr == 0x0045) return s.dmaTarget.b1;

  //DMA_LEN
  if(addr == 0x0046) return s.dmaLength.b0;
  if(addr == 0x0047) return s.dmaLength.b1;

  //DMA_CTRL
  if(addr == 0x0048) return s.dmaEnable << 7 | s.dmaMode << 0;

  //WSC_SYSTEM
  if(addr == 0x0062) {
    return SC() << 7;
  }

  //HW_FLAGS
  if(addr == 0x00a0) {
    return 1 << 7      //1 = built-in self-test passed
         | 1 << 2      //0 = 8-bit bus width; 1 = 16-bit bus width
         | !WS() << 1  //0 = WonderSwan; 1 = WonderSwan Color or SwanCrystal
         | 1 << 0;     //0 = BIOS mapped; 1 = cartridge mapped
  }

  return 0x00;
}

auto CPU::portWrite(uint16 addr, uint8 data) -> void {
  //DMA_SRC
  if(addr == 0x0040) { s.dmaSource.b0 = data & ~1; return; }
  if(addr == 0x0041) { s.dmaSource.b1 = data; return; }
  if(addr == 0x0042) { s.dmaSource.b2 = data; return; }

  //DMA_DST
  if(addr == 0x0044) { s.dmaTarget.b0 = data & ~1; return; }
  if(addr == 0x0045) { s.dmaTarget.b1 = data; return; }

  //DMA_LEN
  if(addr == 0x0046) { s.dmaLength.b0 = data & ~1; return; }
  if(addr == 0x0047) { s.dmaLength.b1 = data; return; }

  //DMA_CTRL
  if(addr == 0x0048) {
    s.dmaEnable = (uint1)(data >> 7);
    s.dmaMode   = (uint1)(data >> 0);
    if(s.dmaEnable) dmaTransfer();
    return;
  }

  //WSC_SYSTEM
  if(addr == 0x0062) {
    //todo: d0 = 1 powers off system
  }

  //HW_FLAGS
  if(addr == 0x00a0) {
    //todo: d2 (bus width) bit is writable; but ... it will do very bad things
  }
}
