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
  i---ggrr

  i: 0 = increment on $2118/$2139
     1 = increment on $2119/$213a
  g: graphic increment
  r: increment rate
     00 = increment by 2
     01 = increment by 64
     10 = increment by 128
     11 = increment by 256
*/
void mmio_w2115(byte value) {
if(value & 0x0c)dprintf("$2115 = %0.2x", value);
  ppu.vram_inc_reg  = (value & 0x80)?1:0;
  switch(value & 3) {
  case 0x00:ppu.vram_inc_size =   1;break;
  case 0x01:ppu.vram_inc_size =  32;break;
  case 0x02:ppu.vram_inc_size =  64;break;
  case 0x03:ppu.vram_inc_size = 128;break;
  }
}

/*
  $2116/$2117 : vram write position

  15-bit value ($2116/$2117) determining position in vram to write to using $2118
  this value is doubled to get true write position (0000-ffff)
*/
void mmio_w2116(byte value) {
  ppu.vram_write_pos = ((ppu.vram_write_pos & 0xff00) | value) & 0x7fff;
}
void mmio_w2117(byte value) {
  ppu.vram_write_pos = ((value << 8) | (ppu.vram_write_pos & 0xff)) & 0x7fff;
}

/*
  $2118/$2119 : vram write

  $2118/$2119 write to vram using vram_write_pos, this is then incremented based on
  the settings of $2115 (vram_inc_size / vram_inc_reg)
*/
void mmio_w2118(byte value) {
word w = ppu.vram_write_pos * 2;
  ppu.vram[w] = value;
  if(ppu.vram_inc_reg == 0) {
    ppu.vram_write_pos += ppu.vram_inc_size;
    ppu.vram_write_pos &= 0x7fff;
  }
  ppu_bg_tiledata_state[TILE_2BIT][(w >> 4)] = 1;
  ppu_bg_tiledata_state[TILE_4BIT][(w >> 5)] = 1;
  ppu_bg_tiledata_state[TILE_8BIT][(w >> 6)] = 1;
}

void mmio_w2119(byte value) {
word w = ppu.vram_write_pos * 2 + 1;
  ppu.vram[w] = value;
  if(ppu.vram_inc_reg == 1) {
    ppu.vram_write_pos += ppu.vram_inc_size;
    ppu.vram_write_pos &= 0x7fff;
  }
  ppu_bg_tiledata_state[TILE_2BIT][(w >> 4)] = 1;
  ppu_bg_tiledata_state[TILE_4BIT][(w >> 5)] = 1;
  ppu_bg_tiledata_state[TILE_8BIT][(w >> 6)] = 1;
}

/*
  $2139/$213a : vram read
*/
byte mmio_r2139(void) {
word w = ppu.vram_write_pos * 2;
byte r;
  r = ppu.vram[w];
  if(ppu.vram_inc_reg == 0) {
    ppu.vram_write_pos += ppu.vram_inc_size;
    ppu.vram_write_pos &= 0x7fff;
  }
  return r;
}

byte mmio_r213a(void) {
word w = ppu.vram_write_pos * 2 + 1;
byte r;
  r = ppu.vram[w];
  if(ppu.vram_inc_reg == 1) {
    ppu.vram_write_pos += ppu.vram_inc_size;
    ppu.vram_write_pos &= 0x7fff;
  }
  return r;
}
