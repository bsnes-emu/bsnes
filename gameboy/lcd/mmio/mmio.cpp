#ifdef LCD_CPP

uint8 LCD::mmio_read(uint16 addr) {
  if(addr >= 0xa000 && addr <= 0xbfff) return vram[addr & 0x1fff];
  if(addr >= 0xfe00 && addr <= 0xfe9f) return oam[addr & 0xff];

  //LY
  if(addr == 0xff44) {
    return status.ly;
  }

  return 0x00;
}

void LCD::mmio_write(uint16 addr, uint8 data) {
  if(addr >= 0xa000 && addr <= 0xbfff) { vram[addr & 0x1fff] = data; return; }
  if(addr >= 0xfe00 && addr <= 0xfe9f) { oam[addr & 0xff] = data; return; }

  //LY
  if(addr == 0xff44) {
    status.ly = 0;
    return;
  }
}

#endif
