/*
  $2121 : cgram write position

  takes an 8-bit value that indexes into color palette cgram data.
  multiply value by 2 to get actual offset into ppu.cgram
*/
void mmio_w2121(byte value) {
  ppu.cgram_write_pos = value << 1;
}

/*
  $2122 : cgram write

  writes to cgram using cgram_write_pos * 2 as an index
*/
void mmio_w2122(byte value) {
  ppu.cgram[ppu.cgram_write_pos] = value;

  debug_test_bp(BPSRC_CGRAM, BP_WRITE, ppu.cgram_write_pos, value);

  ppu.cgram_write_pos++;
  ppu.cgram_write_pos &= 0x01ff;
}

/*
  $213b : cgram read

  read from cgram using cgram_write_pos * 2 as an index
*/
byte mmio_r213b(void) {
byte r;
  r = ppu.cgram[ppu.cgram_write_pos];

  debug_test_bp(BPSRC_CGRAM, BP_READ, ppu.cgram_write_pos, r);

  ppu.cgram_write_pos++;
  ppu.cgram_write_pos &= 0x01ff;
  return r;
}
