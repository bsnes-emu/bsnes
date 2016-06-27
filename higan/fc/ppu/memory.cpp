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

  switch(addr.bits(0,2)) {

  //PPUSTATUS
  case 2:
    result |= r.mdr.bits(0,4);
    result |= r.spriteOverflow << 5;
    result |= r.spriteZeroHit << 6;
    result |= r.nmiFlag << 7;
    r.v.latch = 0;
    r.nmiHold = 0;
    cpu.nmiLine(r.nmiFlag = 0);
    break;

  //OAMDATA
  case 4:
    result = oam[r.oamAddress];
    break;

  //PPUDATA
  case 7:
    if(enable() && (r.ly <= 240 || r.ly == 261)) return 0x00;

    addr = (uint14)r.v.address;
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
    r.v.address += r.vramIncrement;
    break;

  }

  return result;
}

auto PPU::writeIO(uint16 addr, uint8 data) -> void {
  r.mdr = data;

  switch(addr.bits(0,2)) {

  //PPUCTRL
  case 0:
    r.t.nametable   = data.bits(0,1);
    r.vramIncrement = data.bit (2) ? 32 : 1;
    r.spriteAddress = data.bit (3) ? 0x1000 : 0x0000;
    r.bgAddress     = data.bit (4) ? 0x1000 : 0x0000;
    r.spriteHeight  = data.bit (5) ? 16 : 8;
    r.masterSelect  = data.bit (6);
    r.nmiEnable     = data.bit (7);
    cpu.nmiLine(r.nmiEnable && r.nmiHold && r.nmiFlag);
    break;

  //PPUMASK
  case 1:
    r.grayscale        = data.bit (0);
    r.bgEdgeEnable     = data.bit (1);
    r.spriteEdgeEnable = data.bit (2);
    r.bgEnable         = data.bit (3);
    r.spriteEnable     = data.bit (4);
    r.emphasis         = data.bits(5,7);
    break;

  //PPUSTATUS
  case 2:
    break;

  //OAMADDR
  case 3:
    r.oamAddress = data;
    break;

  //OAMDATA
  case 4:
    if(r.oamAddress.bits(0,1) == 2) data.bits(2,4) = 0;  //clear non-existent bits (always read back as 0)
    oam[r.oamAddress++] = data;
    break;

  //PPUSCROLL
  case 5:
    if(!r.v.latch) {
      r.v.fineX = data.bits(0,2);
      r.t.tileX = data.bits(3,7);
    } else {
      r.t.fineY = data.bits(0,2);
      r.t.tileY = data.bits(3,7);
    }
    r.v.latch ^= 1;
    break;

  //PPUADDR
  case 6:
    if(!r.v.latch) {
      r.t.addressHi = data.bits(0,5);
    } else {
      r.t.addressLo = data.bits(0,7);
      r.v.address = r.t.address;
    }
    r.v.latch ^= 1;
    break;

  //PPUDATA
  case 7:
    if(enable() && (r.ly <= 240 || r.ly == 261)) return;

    addr = (uint14)r.v.address;
    if(addr <= 0x1fff) {
      cartridge.writeCHR(addr, data);
    } else if(addr <= 0x3eff) {
      cartridge.writeCHR(addr, data);
    } else if(addr <= 0x3fff) {
      writeCGRAM(addr, data);
    }
    r.v.address += r.vramIncrement;
    break;

  }
}
