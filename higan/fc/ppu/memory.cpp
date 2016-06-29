auto PPU::readCIRAM(uint11 addr) -> uint8 {
  return ciram[addr];
}

auto PPU::writeCIRAM(uint11 addr, uint8 data) -> void {
  ciram[addr] = data;
}

auto PPU::readCGRAM(uint5 addr) -> uint8 {
  if((addr & 0x13) == 0x10) addr &= ~0x10;
  uint8 data = cgram[addr];
  if(io.grayscale) data &= 0x30;
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
    result |= io.mdr.bits(0,4);
    result |= io.spriteOverflow << 5;
    result |= io.spriteZeroHit << 6;
    result |= io.nmiFlag << 7;
    io.v.latch = 0;
    io.nmiHold = 0;
    cpu.nmiLine(io.nmiFlag = 0);
    break;

  //OAMDATA
  case 4:
    result = oam[io.oamAddress];
    break;

  //PPUDATA
  case 7:
    if(enable() && (io.ly <= 240 || io.ly == 261)) return 0x00;

    addr = (uint14)io.v.address;
    if(addr <= 0x1fff) {
      result = io.busData;
      io.busData = cartridge.readCHR(addr);
    } else if(addr <= 0x3eff) {
      result = io.busData;
      io.busData = cartridge.readCHR(addr);
    } else if(addr <= 0x3fff) {
      result = readCGRAM(addr);
      io.busData = cartridge.readCHR(addr);
    }
    io.v.address += io.vramIncrement;
    break;

  }

  return result;
}

auto PPU::writeIO(uint16 addr, uint8 data) -> void {
  io.mdr = data;

  switch(addr.bits(0,2)) {

  //PPUCTRL
  case 0:
    io.t.nametable   = data.bits(0,1);
    io.vramIncrement = data.bit (2) ? 32 : 1;
    io.spriteAddress = data.bit (3) ? 0x1000 : 0x0000;
    io.bgAddress     = data.bit (4) ? 0x1000 : 0x0000;
    io.spriteHeight  = data.bit (5) ? 16 : 8;
    io.masterSelect  = data.bit (6);
    io.nmiEnable     = data.bit (7);
    cpu.nmiLine(io.nmiEnable && io.nmiHold && io.nmiFlag);
    break;

  //PPUMASK
  case 1:
    io.grayscale        = data.bit (0);
    io.bgEdgeEnable     = data.bit (1);
    io.spriteEdgeEnable = data.bit (2);
    io.bgEnable         = data.bit (3);
    io.spriteEnable     = data.bit (4);
    io.emphasis         = data.bits(5,7);
    break;

  //PPUSTATUS
  case 2:
    break;

  //OAMADDR
  case 3:
    io.oamAddress = data;
    break;

  //OAMDATA
  case 4:
    if(io.oamAddress.bits(0,1) == 2) data.bits(2,4) = 0;  //clear non-existent bits (always read back as 0)
    oam[io.oamAddress++] = data;
    break;

  //PPUSCROLL
  case 5:
    if(io.v.latch++ == 0) {
      io.v.fineX = data.bits(0,2);
      io.t.tileX = data.bits(3,7);
    } else {
      io.t.fineY = data.bits(0,2);
      io.t.tileY = data.bits(3,7);
    }
    break;

  //PPUADDR
  case 6:
    if(io.v.latch++ == 0) {
      io.t.addressHi = data.bits(0,5);
    } else {
      io.t.addressLo = data.bits(0,7);
      io.v.address = io.t.address;
    }
    break;

  //PPUDATA
  case 7:
    if(enable() && (io.ly <= 240 || io.ly == 261)) return;

    addr = (uint14)io.v.address;
    if(addr <= 0x1fff) {
      cartridge.writeCHR(addr, data);
    } else if(addr <= 0x3eff) {
      cartridge.writeCHR(addr, data);
    } else if(addr <= 0x3fff) {
      writeCGRAM(addr, data);
    }
    io.v.address += io.vramIncrement;
    break;

  }
}
