uint32 PPU::read(uint32 addr, uint32 size) {
  if(size == Word) {
    addr &= ~3;
    uint32 word = 0;
    word |= read(addr + 0, Byte) <<  0;
    word |= read(addr + 1, Byte) <<  8;
    word |= read(addr + 2, Byte) << 16;
    word |= read(addr + 3, Byte) << 24;
    return word;
  }

  if(size == Half) {
    addr &= ~3;
    uint32 half = 0;
    half |= read(addr + 0, Byte) <<  0;
    half |= read(addr + 0, Byte) <<  8;
    return half;
  }

  switch(addr & 0x0fffffff) {

  //VCOUNT
  case 0x04000006: return regs.scanline >> 0;
  case 0x04000007: return regs.scanline >> 8;

  }

  return 0u;
}

void PPU::write(uint32 addr, uint32 size, uint32 word) {
  if(size == Word) {
    addr &= ~3;
    write(addr + 0, Byte, word >>  0);
    write(addr + 1, Byte, word >>  8);
    write(addr + 2, Byte, word >> 16);
    write(addr + 3, Byte, word >> 24);
    return;
  }

  if(size == Half) {
    addr &= ~1;
    write(addr + 0, Byte, word >>  0);
    write(addr + 1, Byte, word >>  8);
    return;
  }

  uint8 byte = word;

  switch(addr & 0x0fffffff) {

  //DISPCNT
  case 0x04000000: regs.control = (regs.control & 0xff00) | (byte << 0); return;
  case 0x04000001: regs.control = (regs.control & 0x00ff) | (byte << 8); return;

  }
}
