auto PPU::getVramAddress() -> uint16 {
  uint16 address = r.vramAddress;
  switch(r.vramMapping) {
  case 0: return (address);
  case 1: return (address & 0xff00) | ((address & 0x001f) << 3) | ((address >> 5) & 7);
  case 2: return (address & 0xfe00) | ((address & 0x003f) << 3) | ((address >> 6) & 7);
  case 3: return (address & 0xfc00) | ((address & 0x007f) << 3) | ((address >> 7) & 7);
  }
  unreachable;
}

auto PPU::vramRead(bool chip, uint15 addr) -> uint8 {
  uint8 data = 0x00;
  if(r.displayDisable || vcounter() >= vdisp()) {
    data = vram[addr].byte(chip);
    debugger.vramRead(addr << 1 | chip, data);
  }
  return data;
}

auto PPU::vramWrite(bool chip, uint15 addr, uint8 data) -> void {
  if(r.displayDisable || vcounter() >= vdisp()) {
    vram[addr].byte(chip) = data;
    debugger.vramWrite(addr << 1 | chip, data);
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
