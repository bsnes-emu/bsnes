auto VDP::read(uint24 addr) -> uint16 {
  switch(addr & 0xc0001e) {

  //data port
  case 0xc00000: case 0xc00002: {
    return readDataPort();
  }

  //control port
  case 0xc00004: case 0xc00006: {
    return readControlPort();
  }

  //counter
  case 0xc00008: case 0xc0000a: case 0xc0000c: case 0xc0000e: {
    return state.y << 8 | (state.x >> 1) << 0;
  }

  }

  return 0x0000;
}

auto VDP::write(uint24 addr, uint16 data) -> void {
  switch(addr & 0xc0001e) {

  //data port
  case 0xc00000: case 0xc00002: {
    return writeDataPort(data);
  }

  //control port
  case 0xc00004: case 0xc00006: {
    return writeControlPort(data);
  }

  }
}

//

auto VDP::readDataPort() -> uint16 {
  io.commandPending = false;

  //VRAM read
  if(io.command.bits(0,3) == 0) {
    auto address = io.address.bits(1,15);
    auto data = vram[address];
    io.address += io.dataIncrement;
    return data;
  }

  //VSRAM read
  if(io.command.bits(0,3) == 4) {
    auto address = io.address.bits(1,6);
    if(address >= 40) return 0x0000;
    auto data = vsram[address];
    io.address += io.dataIncrement;
    return data;
  }

  //CRAM read
  if(io.command.bits(0,3) == 8) {
    auto address = io.address.bits(1,6);
    auto data = cram[address];
    io.address += io.dataIncrement;
    return data.bits(0,2) << 1 | data.bits(3,5) << 2 | data.bits(6,8) << 3;
  }

  return 0x0000;
}

auto VDP::writeDataPort(uint16 data) -> void {
  io.commandPending = false;

  //DMA VRAM fill
  if(io.dmaFillWait.lower()) {
    io.dmaFillByte = data >> 8;
  }

  //VRAM write
  if(io.command.bits(0,3) == 1) {
    auto address = io.address.bits(1,15);
    if(io.address.bit(0)) data = data >> 8 | data << 8;
    vram[address] = data;
    if(address >= sprite.io.attributeAddress && address < sprite.io.attributeAddress + 320) {
      sprite.write(address, data);
    }
    io.address += io.dataIncrement;
    return;
  }

  //VSRAM write
  if(io.command.bits(0,3) == 5) {
    auto address = io.address.bits(1,6);
    if(address >= 40) return;
    //data format: ---- --yy yyyy yyyy
    vsram[address] = data.bits(0,9);
    io.address += io.dataIncrement;
    return;
  }

  //CRAM write
  if(io.command.bits(0,3) == 3) {
    auto address = io.address.bits(1,6);
    //data format: ---- bbb- ggg- rrr-
    cram[address] = data.bits(1,3) << 0 | data.bits(5,7) << 3 | data.bits(9,11) << 6;
    io.address += io.dataIncrement;
    return;
  }
}

//

auto VDP::readControlPort() -> uint16 {
  io.commandPending = false;

  uint16 result = 0b0011'0100'0000'0000;
  result |= 1 << 9;  //FIFO empty
  result |= (state.y >= 240) << 3;  //vertical blank
  result |= (state.y >= 240 || state.x >= 320) << 2;  //horizontal blank
  result |= io.command.bit(5) << 1;  //DMA active
  return result;
}

auto VDP::writeControlPort(uint16 data) -> void {
//print("[VDPC] ", hex(data, 4L), "\n");

  //command write (lo)
  if(io.commandPending) {
    io.commandPending = false;

    io.command.bits(2,5) = data.bits(4,7);
    io.address.bits(14,15) = data.bits(0,1);
    io.dmaFillWait = io.dmaMode == 2 && io.command.bits(4,5) == 2;
    return;
  }

  //command write (hi)
  if(data.bits(14,15) != 2) {
    io.commandPending = true;

    io.command.bits(0,1) = data.bits(14,15);
    io.address.bits(0,13) = data.bits(0,13);
    return;
  }

  //register write (d13 is ignored)
  if(data.bits(14,15) == 2)
  switch(data.bits(8,12)) {

  //mode register 1
  case 0x00: {
    io.displayOverlayEnable = data.bit(0);
    io.counterLatch = data.bit(1);
    io.horizontalBlankInterruptEnable = data.bit(4);
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

    if(!io.dmaEnable) io.command.bit(5) = 0;

    return;
  }

  //plane A name table location
  case 0x02: {
    planeA.io.nametableAddress = data.bits(3,6) << 12;
    return;
  }

  //window name table location
  case 0x03: {
    window.io.nametableAddress = data.bits(1,6) << 10;
    return;
  }

  //plane B name table location
  case 0x04: {
    planeB.io.nametableAddress = data.bits(0,3) << 12;
    return;
  }

  //sprite attribute table location
  case 0x05: {
    sprite.io.attributeAddress = data.bits(0,7) << 8;
    return;
  }

  //sprite pattern base address
  case 0x06: {
    sprite.io.nametableAddressBase = data.bit(5);
    return;
  }

  //background color
  case 0x07: {
    io.backgroundColor = data.bits(0,5);
    return;
  }

  //horizontal interrupt counter
  case 0x0a: {
    io.horizontalInterruptCounter = data.bits(0,7);
    return;
  }

  //mode register 3
  case 0x0b: {
    planeA.io.horizontalScrollMode = data.bits(0,1);
    planeB.io.horizontalScrollMode = data.bits(0,1);
    planeA.io.verticalScrollMode = data.bit(2);
    planeB.io.verticalScrollMode = data.bit(2);
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
    planeA.io.horizontalScrollAddress = data.bits(0,6) << 9;
    planeB.io.horizontalScrollAddress = data.bits(0,6) << 9;
    return;
  }

  //nametable pattern base address
  case 0x0e: {
    io.nametableBasePatternA = data.bit(0);
    io.nametableBasePatternB = data.bit(1);
    return;
  }

  //data port auto-increment value
  case 0x0f: {
    io.dataIncrement = data.bits(0,7);
    return;
  }

  //plane size
  case 0x10: {
    planeA.io.nametableWidth = data.bits(0,1);
    planeB.io.nametableWidth = data.bits(0,1);
    planeA.io.nametableHeight = data.bits(4,5);
    planeB.io.nametableHeight = data.bits(4,5);
    return;
  }

  //window plane horizontal position
  case 0x11: {
    window.io.horizontalDirection = data.bit(7);
    window.io.horizontalOffset = data.bits(0,4) << 4;
    return;
  }

  //window plane vertical position
  case 0x12: {
    window.io.verticalDirection = data.bit(7);
    window.io.verticalOffset = data.bits(0,4) << 3;
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

  //unused
  default: {
    return;
  }

  }
}
