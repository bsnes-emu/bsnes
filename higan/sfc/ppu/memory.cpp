auto PPU::getVramAddress() -> uint16 {
  uint16 address = r.vramAddress;
  switch(r.vramMapping) {
  case 0: break;  //direct mapping
  case 1: address = (address & 0xff00) | ((address & 0x001f) << 3) | ((address >> 5) & 7); break;
  case 2: address = (address & 0xfe00) | ((address & 0x003f) << 3) | ((address >> 6) & 7); break;
  case 3: address = (address & 0xfc00) | ((address & 0x007f) << 3) | ((address >> 7) & 7); break;
  }
  return address << 1;
}

auto PPU::vramRead(uint addr) -> uint8 {
  uint8 data = 0x00;
  if(r.displayDisable || vcounter() >= vdisp()) {
    data = vram[addr];
    debugger.vramRead(addr, data);
  }
  return data;
}

auto PPU::vramWrite(uint addr, uint8 data) -> void {
  if(r.displayDisable || vcounter() >= vdisp()) {
    vram[addr] = data;
    debugger.vramWrite(addr, data);
  }
}

auto PPU::oamRead(uint addr) -> uint8 {
  uint8 data = oam[addr];
  debugger.oamRead(addr, data);
  return data;
}

auto PPU::oamWrite(uint addr, uint8 data) -> void {
  oam[addr] = data;
  obj.update(addr, data);
  debugger.oamWrite(addr, data);
}

auto PPU::cgramRead(uint addr) -> uint8 {
  uint8 data = cgram[addr];
  debugger.cgramRead(addr, data);
  return data;
}

auto PPU::cgramWrite(uint addr, uint8 data) -> void {
  cgram[addr] = data;
  debugger.cgramWrite(addr, data);
}
