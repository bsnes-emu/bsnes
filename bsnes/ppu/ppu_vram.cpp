/*
  $2107-$210a : bg1-4 tilemap location
  bbbbbbss

  b: location of bg tilemap - highest bit is ignored
  s: tilemap size (00 = 32x32, 01 = 64x32, 10 = 32x64, 11 = 64x64)
*/
void mmio_w2107(byte value) {
  ppu.bg_tilemap_loc[BG1]  = (value & 0x7c) << 9;
  ppu.bg_tilemap_size[BG1] = value & 3;
}
void mmio_w2108(byte value) {
  ppu.bg_tilemap_loc[BG2]  = (value & 0x7c) << 9;
  ppu.bg_tilemap_size[BG2] = value & 3;
}
void mmio_w2109(byte value) {
  ppu.bg_tilemap_loc[BG3]  = (value & 0x7c) << 9;
  ppu.bg_tilemap_size[BG3] = value & 3;
}
void mmio_w210a(byte value) {
  ppu.bg_tilemap_loc[BG4]  = (value & 0x7c) << 9;
  ppu.bg_tilemap_size[BG4] = value & 3;
}

/*
  $210b/$210c: bg1-4 tiledata location
  bbbbaaaa

  a: bg1/3 tiledata location (210b/210c)
  b: bg2/4 tiledata location (210b/210c)
*/
void mmio_w210b(byte value) {
  ppu.bg_tiledata_loc[BG1] = (value & 0x07) << 13;
  ppu.bg_tiledata_loc[BG2] = (value & 0x70) <<  9;
}
void mmio_w210c(byte value) {
  ppu.bg_tiledata_loc[BG3] = (value & 0x07) << 13;
  ppu.bg_tiledata_loc[BG4] = (value & 0x70) <<  9;
}

/*
  $2115 : vram write counter
  i---mmrr

  i: 0 = increment on $2118/$2139
     1 = increment on $2119/$213a
  m: address remapping
     00 = no remapping
     01 = aaaaaaaaBBBccccc -> aaaaaaaacccccBBB
     10 = aaaaaaaBBBcccccc -> aaaaaaaccccccBBB
     11 = aaaaaaBBBccccccc -> aaaaaacccccccBBB
  r: increment rate
     00 = increment by 1
     01 = increment by 32
     10 = increment by 128
     11 = increment by 128
*/
void mmio_w2115(byte value) {
  ppu.vram_inc_reg    = (value & 0x80)?1:0;
  ppu.vram_remap_mode = (value >> 2) & 3;
  switch(value & 3) {
  case 0x00:ppu.vram_inc_size =   1;break;
  case 0x01:ppu.vram_inc_size =  32;break;
  case 0x02:ppu.vram_inc_size = 128;break;
  case 0x03:ppu.vram_inc_size = 128;break;
  }
}

/*
  $2116/$2117 : vram write position

  15-bit value ($2116/$2117) determining position in vram to write to using $2118
  this value is doubled to get true write position (0000-ffff)
*/
void mmio_w2116(byte value) {
  ppu.vram_write_pos = ((ppu.vram_write_pos & 0xff00) | value);
}
void mmio_w2117(byte value) {
  ppu.vram_write_pos = ((value << 8) | (ppu.vram_write_pos & 0xff));
}

word mmio_vram_remap(void) {
word addr, t;
  addr = (ppu.vram_write_pos << 1);
  switch(ppu.vram_remap_mode) {
  case 0:
    break;
  case 1:
    t = (addr >> 5) & 7;
    addr = (addr & 0xff00) | ((addr & 0x001f) << 3) | t;
    break;
  case 2:
    t = (addr >> 6) & 7;
    addr = (addr & 0xfe00) | ((addr & 0x003f) << 3) | t;
    break;
  case 3:
    t = (addr >> 7) & 7;
    addr = (addr & 0xfc00) | ((addr & 0x007f) << 3) | t;
  }
  return addr;
}

/*
  $2118/$2119 : vram write

  $2118/$2119 write to vram using vram_write_pos, this is then incremented based on
  the settings of $2115 (vram_inc_size / vram_inc_reg)
*/
void mmio_w2118(byte value) {
word w;
  w = mmio_vram_remap();
  ppu.vram[w] = value;
  if(ppu.vram_inc_reg == 0) {
    ppu.vram_write_pos += ppu.vram_inc_size;
  }
  ppu_bg_tiledata_state[TILE_2BIT][(w >> 4)] = 1;
  ppu_bg_tiledata_state[TILE_4BIT][(w >> 5)] = 1;
  ppu_bg_tiledata_state[TILE_8BIT][(w >> 6)] = 1;

  debug_test_bp(BPSRC_VRAM, BP_WRITE, w, value);
}

void mmio_w2119(byte value) {
word w;
  w = mmio_vram_remap() + 1;
  ppu.vram[w] = value;
  if(ppu.vram_inc_reg == 1) {
    ppu.vram_write_pos += ppu.vram_inc_size;
  }
  ppu_bg_tiledata_state[TILE_2BIT][(w >> 4)] = 1;
  ppu_bg_tiledata_state[TILE_4BIT][(w >> 5)] = 1;
  ppu_bg_tiledata_state[TILE_8BIT][(w >> 6)] = 1;

  debug_test_bp(BPSRC_VRAM, BP_WRITE, w, value);
}

/*
  $2139/$213a : vram read
*/
byte mmio_r2139(void) {
byte r;
word w;
  w = mmio_vram_remap();
  r = ppu.vram_read_buffer;
  if(ppu.vram_inc_reg == 0) {
    ppu.vram_read_buffer = *((word*)ppu.vram + (w >> 1));
    ppu.vram_write_pos += ppu.vram_inc_size;
  }

  debug_test_bp(BPSRC_VRAM, BP_READ, w, r);
  return r;
}

byte mmio_r213a(void) {
byte r;
word w;
  w = mmio_vram_remap() + 1;
  r = ppu.vram_read_buffer >> 8;
  if(ppu.vram_inc_reg == 1) {
    ppu.vram_read_buffer = *((word*)ppu.vram + (w >> 1));
    ppu.vram_write_pos += ppu.vram_inc_size;
  }

  debug_test_bp(BPSRC_VRAM, BP_READ, w, r);
  return r;
}
