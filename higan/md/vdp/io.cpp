auto VDP::readByte(uint24 addr) -> uint8 {
  return 0x00;
}

auto VDP::readWord(uint24 addr) -> uint16 {
  switch(addr & 0xc0001f) {

  case 0xc00004: case 0xc00006: {
    return readControlPort();
  }

  }

  return 0x0000;
}

auto VDP::writeByte(uint24 addr, uint8 data) -> void {
//print("[VDP] ", hex(addr, 6L), "=", hex(data, 2L), "\n");
}

auto VDP::writeWord(uint24 addr, uint16 data) -> void {
//print("[VDP] ", hex(addr, 6L), "=", hex(data, 4L), "\n");

  switch(addr & 0xc0001f) {

  //control port
  case 0xc00004: case 0xc00006: {
    if(!data.bit(15)) return;
    return writeControlPort(data.bits(8,14), data.bits(0,7));
  }

  }
}

//

auto VDP::readControlPort() -> uint16 {
  uint16 result = 0b0011'0100'0000'0000;
  return result;
}

auto VDP::writeControlPort(uint7 addr, uint8 data) -> void {
//print("[VDPC] ", hex(addr, 2L), "=", hex(data, 2L), "\n");

  switch(addr) {

  //mode register 1
  case 0x00: {
    io.displayOverlayEnable = data.bit(0);
    io.counterLatch = data.bit(1);
    io.horizontalInterruptEnable = data.bit(4);
    io.leftColumnBlank = data.bit(5);
    return;
  }

  //mode register 2
  case 0x01: {
    io.videoMode = data.bit(2);
    io.overscan = data.bit(3);
    io.dmaEnable = data.bit(4);
    io.verticalBlankInterruptEnable = data.bit(5);
    io.displayEnable = data.bit(6);
    io.externalVRAM = data.bit(7);
    return;
  }

  //plane A name table location
  case 0x02: {
    io.nametablePlaneA = data.bits(3,6);
    return;
  }

  //window name table location
  case 0x03: {
    io.nametableWindow = data.bits(1,6);
    return;
  }

  //plane B name table location
  case 0x04: {
    io.nametablePlaneB = data.bits(0,3);
    return;
  }

  //sprite attribute table location
  case 0x05: {
    io.attrtableSprite = data.bits(0,7);
    return;
  }

  //sprite pattern base address
  case 0x06: {
    io.nametableBaseSprite = data.bit(5);
    return;
  }

  //background color
  case 0x07: {
    io.backgroundIndex = data.bits(0,3);
    io.backgroundPalette = data.bits(4,5);
    return;
  }

  //unused
  case 0x08: {
    return;
  }

  //unused
  case 0x09: {
    return;
  }

  //horizontal interrupt counter
  case 0x0a: {
    io.horizontalInterruptCounter = data.bits(0,7);
    return;
  }

  //mode register 3
  case 0x0b: {
    io.horizontalScrollMode = data.bits(0,1);
    io.verticalScrollMode = data.bit(2);
    io.externalInterruptEnable = data.bit(3);
    return;
  }

  //mode register 4
  case 0x0c: {
    io.tileWidth = data.bit(0) | data.bit(7) << 1;
    io.interlaceMode = data.bits(1,2);
    io.shadowHighlightEnable = data.bit(3);
    io.externalColorEnable = data.bit(4);
    io.horizontalSync = data.bit(5);
    io.verticalSync = data.bit(6);
    return;
  }

  //horizontal scroll data location
  case 0x0d: {
    io.horizontalScrollTable = data.bits(1,6);
    return;
  }

  //nametable pattern base address
  case 0x0e: {
    io.nametableBasePatternA = data.bit(0);
    io.nametableBasePatternB = data.bit(1);
    return;
  }

  //VRAM auto-increment value
  case 0x0f: {
    io.vramAutoIncrement = data.bits(0,7);
    return;
  }

  //plane size
  case 0x10: {
    io.horizontalPlaneSize = data.bits(0,1);
    io.verticalPlaneSize = data.bits(4,5);
    return;
  }

  //window plane horizontal position
  case 0x11: {
    io.horizontalWindowPlanePosition = data.bits(0,4);
    io.horizontalWindowPlaneRight = data.bit(7);
    return;
  }

  //window plane vertical position
  case 0x12: {
    io.verticalWindowPlanePosition = data.bits(0,4);
    io.verticalWindowPlaneDown = data.bit(7);
    return;
  }

  //DMA length
  case 0x13: {
    io.dmaLength.bits(0,7) = data.bits(0,7);
    return;
  }

  //DMA length
  case 0x14: {
    io.dmaLength.bits(8,15) = data.bits(0,7);
    return;
  }

  //DMA source
  case 0x15: {
    io.dmaSource.bits(0,7) = data.bits(0,7);
    return;
  }

  //DMA source
  case 0x16: {
    io.dmaSource.bits(8,15) = data.bits(0,7);
    return;
  }

  //DMA source
  case 0x17: {
    io.dmaSource.bits(16,21) = data.bits(0,5);
    io.dmaMode = data.bits(6,7);
    return;
  }

  }
}
