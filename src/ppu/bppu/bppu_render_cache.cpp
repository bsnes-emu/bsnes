#define render_bg_tile_line_4(__m) \
  col = 0;              \
  if(d0 & __m)col += 1; \
  if(d1 & __m)col += 2; \
  *dest++ = col

#define render_bg_tile_line_16(__m) \
  col = 0;              \
  if(d0 & __m)col += 1; \
  if(d1 & __m)col += 2; \
  if(d2 & __m)col += 4; \
  if(d3 & __m)col += 8; \
  *dest++ = col

#define render_bg_tile_line_256(__m) \
  col = 0;                \
  if(d0 & __m)col +=   1; \
  if(d1 & __m)col +=   2; \
  if(d2 & __m)col +=   4; \
  if(d3 & __m)col +=   8; \
  if(d4 & __m)col +=  16; \
  if(d5 & __m)col +=  32; \
  if(d6 & __m)col +=  64; \
  if(d7 & __m)col += 128; \
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
      d0 = vram[pos    ];
      d1 = vram[pos + 1];
      render_bg_tile_line_4(0x80);
      render_bg_tile_line_4(0x40);
      render_bg_tile_line_4(0x20);
      render_bg_tile_line_4(0x10);
      render_bg_tile_line_4(0x08);
      render_bg_tile_line_4(0x04);
      render_bg_tile_line_4(0x02);
      render_bg_tile_line_4(0x01);
      pos += 2;
    }
    bg_tiledata_state[TILE_2BIT][tile_num] = 0;
    break;
  case COLORDEPTH_16:
    dest = (uint8*)bg_tiledata[TILE_4BIT] + tile_num * 64;
    pos  = tile_num * 32;
    y    = 8;
    while(y--) {
      d0 = vram[pos     ];
      d1 = vram[pos +  1];
      d2 = vram[pos + 16];
      d3 = vram[pos + 17];
      render_bg_tile_line_16(0x80);
      render_bg_tile_line_16(0x40);
      render_bg_tile_line_16(0x20);
      render_bg_tile_line_16(0x10);
      render_bg_tile_line_16(0x08);
      render_bg_tile_line_16(0x04);
      render_bg_tile_line_16(0x02);
      render_bg_tile_line_16(0x01);
      pos += 2;
    }
    bg_tiledata_state[TILE_4BIT][tile_num] = 0;
    break;
  case COLORDEPTH_256:
    dest = (uint8*)bg_tiledata[TILE_8BIT] + tile_num * 64;
    pos  = tile_num * 64;
    y    = 8;
    while(y--) {
      d0 = vram[pos     ];
      d1 = vram[pos +  1];
      d2 = vram[pos + 16];
      d3 = vram[pos + 17];
      d4 = vram[pos + 32];
      d5 = vram[pos + 33];
      d6 = vram[pos + 48];
      d7 = vram[pos + 49];
      render_bg_tile_line_256(0x80);
      render_bg_tile_line_256(0x40);
      render_bg_tile_line_256(0x20);
      render_bg_tile_line_256(0x10);
      render_bg_tile_line_256(0x08);
      render_bg_tile_line_256(0x04);
      render_bg_tile_line_256(0x02);
      render_bg_tile_line_256(0x01);
      pos += 2;
    }
    bg_tiledata_state[TILE_8BIT][tile_num] = 0;
    break;
  }
}

#undef render_bg_tile_line_4
#undef render_bg_tile_line_16
#undef render_bg_tile_line_256

inline void bPPU::clear_pixel_cache() {
uint16 main = get_palette(0);
uint16 sub  = (regs.pseudo_hires || regs.hires) ? main : regs.color_rgb;
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
