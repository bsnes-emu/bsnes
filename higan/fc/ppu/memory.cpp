auto PPU::readCIRAM(uint11 addr) -> uint8 {
  return ciram[addr];
}

auto PPU::writeCIRAM(uint11 addr, uint8 data) -> void {
  ciram[addr] = data;
}

auto PPU::readCGRAM(uint5 addr) -> uint8 {
  if((addr & 0x13) == 0x10) addr &= ~0x10;
  uint8 data = cgram[addr];
  if(r.grayscale) data &= 0x30;
  return data;
}

auto PPU::writeCGRAM(uint5 addr, uint8 data) -> void {
  if((addr & 0x13) == 0x10) addr &= ~0x10;
  cgram[addr] = data;
}

auto PPU::readIO(uint16 addr) -> uint8 {
  uint8 result = 0x00;

  switch(addr & 7) {
  case 2:  //PPUSTATUS
    result |= r.nmiFlag << 7;
    result |= r.spriteZeroHit << 6;
    result |= r.spriteOverflow << 5;
    result |= r.mdr & 0x1f;
    r.addressLatch = 0;
    r.nmiHold = 0;
    cpu.nmiLine(r.nmiFlag = 0);
    break;
  case 4:  //OAMDATA
    result = oam[r.oamAddress];
    break;
  case 7:  //PPUDATA
    if(enable() && (r.ly <= 240 || r.ly == 261)) return 0x00;

    addr = r.vaddr & 0x3fff;
    if(addr <= 0x1fff) {
      result = r.busData;
      r.busData = cartridge.readCHR(addr);
    } else if(addr <= 0x3eff) {
      result = r.busData;
      r.busData = cartridge.readCHR(addr);
    } else if(addr <= 0x3fff) {
      result = readCGRAM(addr);
      r.busData = cartridge.readCHR(addr);
    }
    r.vaddr += r.vramIncrement;
    break;
  }

  return result;
}

auto PPU::writeIO(uint16 addr, uint8 data) -> void {
  r.mdr = data;

  switch(addr & 7) {
  case 0:  //PPUCTRL
    r.nmiEnable = data & 0x80;
    r.masterSelect = data & 0x40;
    r.spriteHeight = data & 0x20 ? 16 : 8;
    r.bgAddress = (data & 0x10) ? 0x1000 : 0x0000;
    r.spriteAddress = (data & 0x08) ? 0x1000 : 0x0000;
    r.vramIncrement = (data & 0x04) ? 32 : 1;
    r.taddr = (r.taddr & 0x73ff) | ((data & 0x03) << 10);
    cpu.nmiLine(r.nmiEnable && r.nmiHold && r.nmiFlag);
    return;
  case 1:  //PPUMASK
    r.emphasis = data >> 5;
    r.spriteEnable = data & 0x10;
    r.bgEnable = data & 0x08;
    r.spriteEdgeEnable = data & 0x04;
    r.bgEdgeEnable = data & 0x02;
    r.grayscale = data & 0x01;
    return;
  case 2:  //PPUSTATUS
    return;
  case 3:  //OAMADDR
    r.oamAddress = data;
    return;
  case 4:  //OAMDATA
    if(r.oamAddress.bits(0,1) == 2) data.bits(2,4) = 0;  //clear non-existent bits (always read back as 0)
    oam[r.oamAddress++] = data;
    return;
  case 5:  //PPUSCROLL
    if(r.addressLatch == 0) {
      r.xaddr = data & 0x07;
      r.taddr = (r.taddr & 0x7fe0) | (data >> 3);
    } else {
      r.taddr = (r.taddr & 0x0c1f) | ((data & 0x07) << 12) | ((data >> 3) << 5);
    }
    r.addressLatch ^= 1;
    return;
  case 6:  //PPUADDR
    if(r.addressLatch == 0) {
      r.taddr = (r.taddr & 0x00ff) | ((data & 0x3f) << 8);
    } else {
      r.taddr = (r.taddr & 0x7f00) | data;
      r.vaddr = r.taddr;
    }
    r.addressLatch ^= 1;
    return;
  case 7:  //PPUDATA
    if(enable() && (r.ly <= 240 || r.ly == 261)) return;

    addr = r.vaddr & 0x3fff;
    if(addr <= 0x1fff) {
      cartridge.writeCHR(addr, data);
    } else if(addr <= 0x3eff) {
      cartridge.writeCHR(addr, data);
    } else if(addr <= 0x3fff) {
      writeCGRAM(addr, data);
    }
    r.vaddr += r.vramIncrement;
    return;
  }
}
