#ifdef BPPU_CPP

#define render_bg_tile_line_2bpp(mask) \
  col  = !!(d0 & mask) << 0; \
  col += !!(d1 & mask) << 1; \
  *dest++ = col

#define render_bg_tile_line_4bpp(mask) \
  col  = !!(d0 & mask) << 0; \
  col += !!(d1 & mask) << 1; \
  col += !!(d2 & mask) << 2; \
  col += !!(d3 & mask) << 3; \
  *dest++ = col

#define render_bg_tile_line_8bpp(mask) \
  col  = !!(d0 & mask) << 0; \
  col += !!(d1 & mask) << 1; \
  col += !!(d2 & mask) << 2; \
  col += !!(d3 & mask) << 3; \
  col += !!(d4 & mask) << 4; \
  col += !!(d5 & mask) << 5; \
  col += !!(d6 & mask) << 6; \
  col += !!(d7 & mask) << 7; \
  *dest++ = col

void bPPU::render_bg_tile(uint8 color_depth, uint16 tile_num) {
uint8  mask, d0, d1, d2, d3, d4, d5, d6, d7, col;
int    x, y;
uint32 pos;
uint8 *dest;
  switch(color_depth) {
  case COLORDEPTH_4:
    dest = (uint8*)bg_tiledata[TILE_2BIT] + tile_num * 64;
    pos  = tile_num * 16;
    y    = 8;
    while(y--) {
      d0 = memory::vram[pos    ];
      d1 = memory::vram[pos + 1];
      render_bg_tile_line_2bpp(0x80);
      render_bg_tile_line_2bpp(0x40);
      render_bg_tile_line_2bpp(0x20);
      render_bg_tile_line_2bpp(0x10);
      render_bg_tile_line_2bpp(0x08);
      render_bg_tile_line_2bpp(0x04);
      render_bg_tile_line_2bpp(0x02);
      render_bg_tile_line_2bpp(0x01);
      pos += 2;
    }
    bg_tiledata_state[TILE_2BIT][tile_num] = 0;
    break;
  case COLORDEPTH_16:
    dest = (uint8*)bg_tiledata[TILE_4BIT] + tile_num * 64;
    pos  = tile_num * 32;
    y    = 8;
    while(y--) {
      d0 = memory::vram[pos     ];
      d1 = memory::vram[pos +  1];
      d2 = memory::vram[pos + 16];
      d3 = memory::vram[pos + 17];
      render_bg_tile_line_4bpp(0x80);
      render_bg_tile_line_4bpp(0x40);
      render_bg_tile_line_4bpp(0x20);
      render_bg_tile_line_4bpp(0x10);
      render_bg_tile_line_4bpp(0x08);
      render_bg_tile_line_4bpp(0x04);
      render_bg_tile_line_4bpp(0x02);
      render_bg_tile_line_4bpp(0x01);
      pos += 2;
    }
    bg_tiledata_state[TILE_4BIT][tile_num] = 0;
    break;
  case COLORDEPTH_256:
    dest = (uint8*)bg_tiledata[TILE_8BIT] + tile_num * 64;
    pos  = tile_num * 64;
    y    = 8;
    while(y--) {
      d0 = memory::vram[pos     ];
      d1 = memory::vram[pos +  1];
      d2 = memory::vram[pos + 16];
      d3 = memory::vram[pos + 17];
      d4 = memory::vram[pos + 32];
      d5 = memory::vram[pos + 33];
      d6 = memory::vram[pos + 48];
      d7 = memory::vram[pos + 49];
      render_bg_tile_line_8bpp(0x80);
      render_bg_tile_line_8bpp(0x40);
      render_bg_tile_line_8bpp(0x20);
      render_bg_tile_line_8bpp(0x10);
      render_bg_tile_line_8bpp(0x08);
      render_bg_tile_line_8bpp(0x04);
      render_bg_tile_line_8bpp(0x02);
      render_bg_tile_line_8bpp(0x01);
      pos += 2;
    }
    bg_tiledata_state[TILE_8BIT][tile_num] = 0;
    break;
  }
}

#undef render_bg_tile_line_2bpp
#undef render_bg_tile_line_4bpp
#undef render_bg_tile_line_8bpp

void bPPU::clear_pixel_cache() {
uint16 main = get_palette(0);
uint16 sub  = (regs.pseudo_hires || regs.bg_mode == 5 || regs.bg_mode == 6) ? main : regs.color_rgb;
uint32 i    = 255;
  do {
    pixel_cache[i].src_main = main;
    pixel_cache[i].src_sub  = sub;
    pixel_cache[i].bg_main  = BACK;
    pixel_cache[i].bg_sub   = BACK;
    pixel_cache[i].ce_main  = false;
    pixel_cache[i].ce_sub   = false;
    pixel_cache[i].pri_main = 0;
    pixel_cache[i].pri_sub  = 0;
  } while(i--);
}

void bPPU::init_tiledata_cache() {
  bg_tiledata[TILE_2BIT]       = (uint8*)malloc(262144);
  bg_tiledata[TILE_4BIT]       = (uint8*)malloc(131072);
  bg_tiledata[TILE_8BIT]       = (uint8*)malloc( 65536);
  bg_tiledata_state[TILE_2BIT] = (uint8*)malloc(  4096);
  bg_tiledata_state[TILE_4BIT] = (uint8*)malloc(  2048);
  bg_tiledata_state[TILE_8BIT] = (uint8*)malloc(  1024);
}

void bPPU::clear_tiledata_cache() {
  memset(bg_tiledata[TILE_2BIT],       0, 262144);
  memset(bg_tiledata[TILE_4BIT],       0, 131072);
  memset(bg_tiledata[TILE_8BIT],       0,  65536);
  memset(bg_tiledata_state[TILE_2BIT], 0,   4096);
  memset(bg_tiledata_state[TILE_4BIT], 0,   2048);
  memset(bg_tiledata_state[TILE_8BIT], 0,   1024);
}

#endif //ifdef BPPU_CPP
