auto PPU::getVramAddress() -> uint16 {
  uint16 addr = regs.vram_addr;
  switch(regs.vram_mapping) {
  case 0: break;  //direct mapping
  case 1: addr = (addr & 0xff00) | ((addr & 0x001f) << 3) | ((addr >> 5) & 7); break;
  case 2: addr = (addr & 0xfe00) | ((addr & 0x003f) << 3) | ((addr >> 6) & 7); break;
  case 3: addr = (addr & 0xfc00) | ((addr & 0x007f) << 3) | ((addr >> 7) & 7); break;
  }
  return (addr << 1);
}

auto PPU::vramRead(uint addr) -> uint8 {
  uint8 data = 0x00;
  if(regs.display_disable || vcounter() >= vdisp()) {
    data = vram[addr];
    debugger.vram_read(addr, data);
  }
  return data;
}

auto PPU::vramWrite(uint addr, uint8 data) -> void {
  if(regs.display_disable || vcounter() >= vdisp()) {
    vram[addr] = data;
    debugger.vram_write(addr, data);
  }
}

auto PPU::oamRead(uint addr) -> uint8 {
  uint8 data = oam[addr];
  debugger.oam_read(addr, data);
  return data;
}

auto PPU::oamWrite(uint addr, uint8 data) -> void {
  oam[addr] = data;
  sprite.update(addr, data);
  debugger.oam_write(addr, data);
}

auto PPU::cgramRead(uint addr) -> uint8 {
  uint8 data = cgram[addr];
  debugger.cgram_read(addr, data);
  return data;
}

auto PPU::cgramWrite(uint addr, uint8 data) -> void {
  cgram[addr] = data;
  debugger.cgram_write(addr, data);
}
