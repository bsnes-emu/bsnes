/*
  $2180 : wram read/write

  write byte to wram write pointer ($2181-$2183), then increment pointer.
  always stays within 7e0000-7fffff, high 7 bits of 24-bit offset ignored.
*/
byte mmio_r2180(void) {
byte r;
  r = gx816->mem_read(MEMMODE_NONE, MEMSIZE_BYTE, 0x7e0000 | ppu.wram_write_pos);
  ppu.wram_write_pos++;
  ppu.wram_write_pos &= 0x01ffff;
  return r;
}

void mmio_w2180(byte value) {
  gx816->mem_write(MEMMODE_NONE, MEMSIZE_BYTE, 0x7e0000 | ppu.wram_write_pos, value);
  ppu.wram_write_pos++;
  ppu.wram_write_pos &= 0x01ffff;
}

/*
  $2181-$2183: wram write pointer set
*/
void mmio_w2181(byte value) {
  ppu.wram_write_pos = ((ppu.wram_write_pos & 0xffff00) | value) & 0x01ffff;
}
void mmio_w2182(byte value) {
  ppu.wram_write_pos = ((ppu.wram_write_pos & 0xff00ff) | (value << 8)) & 0x01ffff;
}
void mmio_w2183(byte value) {
  ppu.wram_write_pos = ((ppu.wram_write_pos & 0x00ffff) | (value << 16)) & 0x01ffff;
}
