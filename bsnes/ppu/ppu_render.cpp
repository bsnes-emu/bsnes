byte ppu_addsub_adjust_buffer[96] = {
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31
};

#define ppu_pal_pixel(__i) \
  (*((word*)ppu.cgram + __i))

word ppu_addsub_pixels(byte x, byte cdest_index, byte cdest_bg, byte csrc_index, byte csrc_bg) {
int r, g, b;
byte hd = 0, hs = 0;
word cdest = ppu_pal_pixel(cdest_index);
word csrc  = ppu_pal_pixel(csrc_index);
//oam palettes 0-3 are not affected by color add/sub
  if(cdest_bg == OAM) {
    if(cdest_index < 192) {
      return cdest;
    }
  }
  if(ppu.bg_color_enabled[cdest_bg] == true) {
    hd = hs = ppu.color_halve;
  }
  switch(ppu.color_mode) {
  case COLORMODE_ADD:
    r = *(ppu_addsub_adjust_buffer + 32 + (( ((cdest      ) & 31) >> hd) + ( ((csrc      ) & 31) >> hs) ));
    g = *(ppu_addsub_adjust_buffer + 32 + (( ((cdest >>  5) & 31) >> hd) + ( ((csrc >>  5) & 31) >> hs) ));
    b = *(ppu_addsub_adjust_buffer + 32 + (( ((cdest >> 10) & 31) >> hd) + ( ((csrc >> 10) & 31) >> hs) ));
    break;
  case COLORMODE_SUB:
    r = *(ppu_addsub_adjust_buffer + 32 + (( ((cdest      ) & 31) >> hd) - ( ((csrc      ) & 31) >> hs) ));
    g = *(ppu_addsub_adjust_buffer + 32 + (( ((cdest >>  5) & 31) >> hd) - ( ((csrc >>  5) & 31) >> hs) ));
    b = *(ppu_addsub_adjust_buffer + 32 + (( ((cdest >> 10) & 31) >> hd) - ( ((csrc >> 10) & 31) >> hs) ));
    break;
  }
  return ((r) | (g << 5) | (b << 10));
}

word ppu_addsub_pixel(byte x, byte cdest_index, byte cdest_bg) {
int r, g, b;
byte hd = 0;
word cdest = ppu_pal_pixel(cdest_index);
//only oam palettes 4-7 are affected by color add/sub
  if(cdest_bg == OAM) {
    if(cdest_index < 192) {
      return cdest;
    }
  }
  if(ppu.bg_color_enabled[cdest_bg] == true) {
    hd = ppu.color_halve;
  }
  switch(ppu.color_mode) {
  case COLORMODE_ADD:
    r = *(ppu_addsub_adjust_buffer + 32 + (( ((cdest      ) & 31) >> hd) + (ppu.color_r >> hd) ));
    g = *(ppu_addsub_adjust_buffer + 32 + (( ((cdest >>  5) & 31) >> hd) + (ppu.color_g >> hd) ));
    b = *(ppu_addsub_adjust_buffer + 32 + (( ((cdest >> 10) & 31) >> hd) + (ppu.color_b >> hd) ));
    break;
  case COLORMODE_SUB:
    r = *(ppu_addsub_adjust_buffer + 32 + (( ((cdest      ) & 31) >> hd) - (ppu.color_r >> hd) ));
    g = *(ppu_addsub_adjust_buffer + 32 + (( ((cdest >>  5) & 31) >> hd) - (ppu.color_g >> hd) ));
    b = *(ppu_addsub_adjust_buffer + 32 + (( ((cdest >> 10) & 31) >> hd) - (ppu.color_b >> hd) ));
    break;
  }
  return ((r) | (g << 5) | (b << 10));
}

#define ppu_render_bg_tile_line_4(__m) \
  col = 0;              \
  if(d0 & __m)col += 1; \
  if(d1 & __m)col += 2; \
  *dest++ = col
#define ppu_render_bg_tile_line_16(__m) \
  col = 0;              \
  if(d0 & __m)col += 1; \
  if(d1 & __m)col += 2; \
  if(d2 & __m)col += 4; \
  if(d3 & __m)col += 8; \
  *dest++ = col
#define ppu_render_bg_tile_line_256(__m) \
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

void ppu_render_bg_tile(byte color_depth, byte bg, word tile_num) {
byte  mask, d0, d1, d2, d3, d4, d5, d6, d7, col;
int   x, y;
ulong pos;
byte *dest;
  switch(color_depth) {
  case COLORDEPTH_4:
    dest = (byte*)ppu_bg_tiledata[TILE_2BIT] + tile_num * 64;
    pos  = tile_num * 16;
    y    = 8;
    while(y--) {
      d0 = ppu.vram[pos    ];
      d1 = ppu.vram[pos + 1];
      ppu_render_bg_tile_line_4(0x80);
      ppu_render_bg_tile_line_4(0x40);
      ppu_render_bg_tile_line_4(0x20);
      ppu_render_bg_tile_line_4(0x10);
      ppu_render_bg_tile_line_4(0x08);
      ppu_render_bg_tile_line_4(0x04);
      ppu_render_bg_tile_line_4(0x02);
      ppu_render_bg_tile_line_4(0x01);
      pos += 2;
    }
    ppu_bg_tiledata_state[TILE_2BIT][tile_num] = 0;
    break;
  case COLORDEPTH_16:
    dest = (byte*)ppu_bg_tiledata[TILE_4BIT] + tile_num * 64;
    pos  = tile_num * 32;
    y    = 8;
    while(y--) {
      d0 = ppu.vram[pos     ];
      d1 = ppu.vram[pos +  1];
      d2 = ppu.vram[pos + 16];
      d3 = ppu.vram[pos + 17];
      ppu_render_bg_tile_line_16(0x80);
      ppu_render_bg_tile_line_16(0x40);
      ppu_render_bg_tile_line_16(0x20);
      ppu_render_bg_tile_line_16(0x10);
      ppu_render_bg_tile_line_16(0x08);
      ppu_render_bg_tile_line_16(0x04);
      ppu_render_bg_tile_line_16(0x02);
      ppu_render_bg_tile_line_16(0x01);
      pos += 2;
    }
    ppu_bg_tiledata_state[TILE_4BIT][tile_num] = 0;
    break;
  case COLORDEPTH_256:
    dest = (byte*)ppu_bg_tiledata[TILE_8BIT] + tile_num * 64;
    pos  = tile_num * 64;
    y    = 8;
    while(y--) {
      d0 = ppu.vram[pos     ];
      d1 = ppu.vram[pos +  1];
      d2 = ppu.vram[pos + 16];
      d3 = ppu.vram[pos + 17];
      d4 = ppu.vram[pos + 32];
      d5 = ppu.vram[pos + 33];
      d6 = ppu.vram[pos + 48];
      d7 = ppu.vram[pos + 49];
      ppu_render_bg_tile_line_256(0x80);
      ppu_render_bg_tile_line_256(0x40);
      ppu_render_bg_tile_line_256(0x20);
      ppu_render_bg_tile_line_256(0x10);
      ppu_render_bg_tile_line_256(0x08);
      ppu_render_bg_tile_line_256(0x04);
      ppu_render_bg_tile_line_256(0x02);
      ppu_render_bg_tile_line_256(0x01);
      pos += 2;
    }
    ppu_bg_tiledata_state[TILE_8BIT][tile_num] = 0;
    break;
  }
}

#define PPU_MAIN 0
#define PPU_SUB  1

bool windows_not_obstructing(byte layer, byte bg, byte x);
bool color_windows_not_obstructing(byte x, byte color_mask_type);

//light table is mirrored twice so that the high bit (bit 15) in the color
//is ignored, and does not cause color to reach into next light table.
#define ppu_write_pixel() \
  *(ptr + x) = *(light_table + cx)

void ppu_render_line_to_screen(void) {
int x;
word *ptr, *light_table;
word c, cx, cy;
word screen_width = render.snes_width;
  ptr         = (word*)ppu.screen;
  light_table = (word*)ppu.light_table + (ppu.display_brightness * 65536);
  if(ppu.interlace == true) {
    ptr += ((ppu.vline_pos * 2) + ppu.interlace_frame) * 512;
  } else {
    ptr += ppu.vline_pos * 512;
  }
  for(x=0;x<screen_width;x++) {
    switch(ppu_pixel_cache[x].blend_type) {
    case BLENDTYPE_BACK:
      if(ppu.bg_color_enabled[BACK] == true && color_windows_not_obstructing(x, PPU_SUB) == false) {
        cx = ppu_addsub_pixel(x, 0, BACK);
      } else {
        cx = ppu_pal_pixel(0);
      }
      ppu_write_pixel();
      break;
    case BLENDTYPE_MAIN:
      if(ppu.bg_color_enabled[ppu_pixel_cache[x].src_main] == true && color_windows_not_obstructing(x, PPU_SUB) == false) {
        cx = ppu_addsub_pixel(x, ppu_pixel_cache[x].color_main, ppu_pixel_cache[x].src_main);
      } else {
        cx = ppu_pal_pixel(ppu_pixel_cache[x].color_main);
      }
      ppu_write_pixel();
      break;
    case BLENDTYPE_SUB:
      if(ppu.bg_color_enabled[BACK] && color_windows_not_obstructing(x, PPU_SUB) == false) {
        cx = ppu_addsub_pixels(x, 0, BACK, ppu_pixel_cache[x].color_sub, ppu_pixel_cache[x].src_sub);
      } else {
        cx = ppu_pal_pixel(ppu_pixel_cache[x].color_sub);
      }
      ppu_write_pixel();
      break;
    case BLENDTYPE_COMBINE:
      if(color_windows_not_obstructing(x, PPU_SUB) == false) {
        if(ppu_pixel_cache[x].src_sub == BACK) {
          cx = ppu_addsub_pixels(x, ppu_pixel_cache[x].color_main, ppu_pixel_cache[x].src_main, 0, BACK);
        } else {
          cx = ppu_addsub_pixels(x, ppu_pixel_cache[x].color_main, ppu_pixel_cache[x].src_main,
                                    ppu_pixel_cache[x].color_sub,  ppu_pixel_cache[x].src_sub);
        }
      } else {
        cx = ppu_pal_pixel(ppu_pixel_cache[x].color_main);
      }
      ppu_write_pixel();
      break;
    }
  }
}

void ppu_set_pixel(byte bg, word x, byte pal_index) {
  if(ppu.bg_enabled[bg] == true && ppu.ss_bg_enabled[bg] == true) {
    if(windows_not_obstructing(PPU_MAIN, bg, x) == false)return;
    ppu_pixel_cache[x].blend_type = BLENDTYPE_MAIN;
    ppu_pixel_cache[x].src_main   = bg;
    ppu_pixel_cache[x].color_main = pal_index;
    if(color_windows_not_obstructing(x, PPU_SUB) == false) {
      ppu_pixel_cache[x].src_sub    = bg;
      ppu_pixel_cache[x].color_sub  = pal_index;
    }
  } else if(ppu.bg_enabled[bg] == true && bg == OAM && pal_index < 192) {
    if(windows_not_obstructing(PPU_MAIN, bg, x) == false)return;
    ppu_pixel_cache[x].blend_type = BLENDTYPE_MAIN;
    ppu_pixel_cache[x].src_main   = bg;
    ppu_pixel_cache[x].color_main = pal_index;
  } else if(ppu.bg_enabled[bg] == true) {
    if(windows_not_obstructing(PPU_MAIN, bg, x) == false)return;
    if(ppu.bg_color_enabled[bg] == true && ppu_pixel_cache[x].src_sub != BACK) {
      ppu_pixel_cache[x].blend_type = BLENDTYPE_COMBINE;
    } else {
      ppu_pixel_cache[x].blend_type = BLENDTYPE_MAIN;
    }
    ppu_pixel_cache[x].src_main   = bg;
    ppu_pixel_cache[x].color_main = pal_index;
  } else if(ppu.ss_bg_enabled[bg] == true) {
    if(windows_not_obstructing(PPU_SUB, bg, x) == false)return;
    ppu_pixel_cache[x].src_sub    = bg;
    ppu_pixel_cache[x].color_sub  = pal_index;
    if(ppu_pixel_cache[x].blend_type == BLENDTYPE_BACK) {
      ppu_pixel_cache[x].blend_type = BLENDTYPE_SUB;
    } else if(ppu_pixel_cache[x].blend_type == BLENDTYPE_MAIN) {
      if(ppu_pixel_cache[x].src_main != OAM || (ppu_pixel_cache[x].src_main == OAM && ppu_pixel_cache[x].color_main >= 192)) {
        if(ppu.bg_color_enabled[ppu_pixel_cache[x].src_main] == true) {
          ppu_pixel_cache[x].blend_type = BLENDTYPE_COMBINE;
        }
      }
    }
  }
}

struct {
  byte num;
  byte width, height;
  word x, y;
  word character;
  byte v_flip, h_flip;
  byte palette;
  byte priority;
}current_sprite;

void ppu_set_sprite_attributes(byte sprite_num) {
ulong t;
byte size, b;
word x;
  t = ppu.oam[(sprite_num << 2)    ]       |
      ppu.oam[(sprite_num << 2) + 1] <<  8 |
      ppu.oam[(sprite_num << 2) + 2] << 16 |
      ppu.oam[(sprite_num << 2) + 3] << 24;
  b = ppu.oam[512 + (sprite_num >> 2)];

  if     ((sprite_num & 3) == 0) { size = (b & 0x02)?1:0; x = (b & 0x01)?0x100:0; }
  else if((sprite_num & 3) == 1) { size = (b & 0x08)?1:0; x = (b & 0x04)?0x100:0; }
  else if((sprite_num & 3) == 2) { size = (b & 0x20)?1:0; x = (b & 0x10)?0x100:0; }
  else if((sprite_num & 3) == 3) { size = (b & 0x80)?1:0; x = (b & 0x40)?0x100:0; }

  current_sprite.num       = sprite_num;
  current_sprite.priority  = (t >> 28) & 3;
  current_sprite.x         = x | (t & 0xff);
  current_sprite.y         = (t >> 8) & 0xff;
  current_sprite.v_flip    = (t & 0x80000000)?1:0;
  current_sprite.h_flip    = (t & 0x40000000)?1:0;
  current_sprite.palette   = (t >> 25) & 7;
  current_sprite.character = (t >> 16) & 0x01ff;

//size: 0 = small, 1 = large
  switch(ppu.oam_base_size) {
  case 0:
    if(!size) { current_sprite.width =  8; current_sprite.height =  8; }
    else      { current_sprite.width = 16; current_sprite.height = 16; }
    break;
  case 1:
    if(!size) { current_sprite.width =  8; current_sprite.height =  8; }
    else      { current_sprite.width = 32; current_sprite.height = 32; }
    break;
  case 2:
    if(!size) { current_sprite.width =  8; current_sprite.height =  8; }
    else      { current_sprite.width = 64; current_sprite.height = 64; }
    break;
  case 3:
    if(!size) { current_sprite.width = 16; current_sprite.height = 16; }
    else      { current_sprite.width = 32; current_sprite.height = 32; }
    break;
  case 4:
    if(!size) { current_sprite.width = 16; current_sprite.height = 16; }
    else      { current_sprite.width = 64; current_sprite.height = 64; }
    break;
  case 5:
    if(!size) { current_sprite.width = 32; current_sprite.height = 32; }
    else      { current_sprite.width = 64; current_sprite.height = 64; }
    break;
  case 6:
    if(!size) { current_sprite.width = 16; current_sprite.height = 32; }
    else      { current_sprite.width = 32; current_sprite.height = 64; }
    break;
  case 7:
    if(!size) { current_sprite.width = 16; current_sprite.height = 32; }
    else      { current_sprite.width = 32; current_sprite.height = 32; }
    break;
  }
}

bool windows_not_obstructing(byte layer, byte bg, byte x) {
byte w1_mask, w2_mask; //1 = masked, 0 = not masked
  if(layer == PPU_MAIN) {
    if(ppu.bg_windowing_enabled[bg] == false)return true;
  } else if(layer == PPU_SUB) {
    if(ppu.ss_bg_windowing_enabled[bg] == false)return true;
  }

  if(ppu.bg_window1_enabled[bg] == true && ppu.bg_window2_enabled[bg] == false) {
    if(ppu.bg_window1_clipmode[bg] == CLIPMODE_IN) {
      if(x >= ppu.window1_left && x <= ppu.window1_right)return false;
      return true;
    } else {
      if(x <= ppu.window1_left || x >= ppu.window1_right)return false;
      return true;
    }
  } else if(ppu.bg_window2_enabled[bg] == true && ppu.bg_window1_enabled[bg] == false) {
    if(ppu.bg_window2_clipmode[bg] == CLIPMODE_IN) {
      if(x >= ppu.window2_left && x <= ppu.window2_right)return false;
      return true;
    } else {
      if(x <= ppu.window2_left || x >= ppu.window2_right)return false;
      return true;
    }
  } else if(ppu.bg_window1_enabled[bg] == true && ppu.bg_window2_enabled[bg] == true) {
    if(ppu.bg_window1_clipmode[bg] == CLIPMODE_IN) {
      if(x >= ppu.window1_left && x <= ppu.window1_right)w1_mask = 1;
      else w1_mask = 0;
    } else {
      if(x <= ppu.window1_left || x >= ppu.window1_right)w1_mask = 1;
      else w1_mask = 0;
    }

    if(ppu.bg_window2_clipmode[bg] == CLIPMODE_IN) {
      if(x >= ppu.window2_left && x <= ppu.window2_right)w2_mask = 1;
      else w2_mask = 0;
    } else {
      if(x <= ppu.window2_left || x >= ppu.window2_right)w2_mask = 1;
      else w2_mask = 0;
    }

    switch(ppu.bg_window_mask[bg]) {
    case WINDOWMASK_OR:
      if((w1_mask | w2_mask) == 1)return false;
      return true;
    case WINDOWMASK_AND:
      if((w1_mask & w2_mask) == 1)return false;
      return true;
    case WINDOWMASK_XOR:
      if((w1_mask ^ w2_mask) == 1)return false;
      return true;
    case WINDOWMASK_XNOR:
      if((w1_mask ^ w2_mask) == 0)return false;
      return true;
    }
  }
  return true;
}

bool color_windows_not_obstructing(byte x, byte color_mask_type) {
byte w1_mask, w2_mask; //1 = masked, 0 = not masked
byte color_mask;
bool r;
  if(color_mask_type == PPU_MAIN)color_mask = ppu.color_mask;
  else                           color_mask = ppu.ss_color_mask;

  if(color_mask == 0)return false;
  if(color_mask == 3)return true;

  if(ppu.color_window1_enabled == false && ppu.color_window2_enabled == false) {
    r = true;
  } else if(ppu.color_window1_enabled == true && ppu.color_window2_enabled == false) {
    if(ppu.color_window1_clipmode == CLIPMODE_IN) {
      if(x >= ppu.window1_left && x <= ppu.window1_right)r = false;
      else                                               r = true;
    } else {
      if(x <= ppu.window1_left || x >= ppu.window1_right)r = false;
      else                                               r = true;
    }
  } else if(ppu.color_window1_enabled == false && ppu.color_window2_enabled == true) {
    if(ppu.color_window2_clipmode == CLIPMODE_IN) {
      if(x >= ppu.window2_left && x <= ppu.window2_right)r = false;
      else                                               r = true;
    } else {
      if(x <= ppu.window2_left || x >= ppu.window2_right)r = false;
      else                                               r = true;
    }
  } else if(ppu.color_window1_enabled == true && ppu.color_window2_enabled == true) {
    if(ppu.color_window1_clipmode == CLIPMODE_IN) {
      if(x >= ppu.window1_left && x <= ppu.window1_right)w1_mask = 1;
      else w1_mask = 0;
    } else {
      if(x <= ppu.window1_left || x >= ppu.window1_right)w1_mask = 1;
      else w1_mask = 0;
    }

    if(ppu.color_window2_clipmode == CLIPMODE_IN) {
      if(x >= ppu.window2_left && x <= ppu.window2_right)w2_mask = 1;
      else w2_mask = 0;
    } else {
      if(x <= ppu.window2_left || x >= ppu.window2_right)w2_mask = 1;
      else w2_mask = 0;
    }

    switch(ppu.color_window_mask) {
    case WINDOWMASK_OR:
      if((w1_mask | w2_mask) == 1)r = false;
      else                        r = true;
      break;
    case WINDOWMASK_AND:
      if((w1_mask & w2_mask) == 1)r = false;
      else                        r = true;
      break;
    case WINDOWMASK_XOR:
      if((w1_mask ^ w2_mask) == 1)r = false;
      else                        r = true;
      break;
    case WINDOWMASK_XNOR:
      if((w1_mask ^ w2_mask) == 0)r = false;
      else                        r = true;
      break;
    }
  }

  if(color_mask == 2) {
    r = (r == true)?false:true;
  }

  return r;
}

/*
  *1 - When bit 8 of a sprite's character number is set, such that character data
       is read from the upper half (upper 8k) of sprite vram, bits 4-3 of $2101
       are added to bits 14-13 of the tiledata location. The address wraps around
       the 64k bank. Why this happens, or what it's for, I have no idea.
  *2 - The sprite tiledata is stored with 16 tiles making up the first row, followed
       by 16 tiles making up the second row, and so on. Therefore, to get the
       correct y tile, y / 8 * 1 row (16 tiles) must be used.
*/

#define ppu_render_oam_tile_line(__m) \
  x &= 511;               \
  if(x < 256) {           \
    col = 0;              \
    if(d0 & __m)col += 1; \
    if(d1 & __m)col += 2; \
    if(d2 & __m)col += 4; \
    if(d3 & __m)col += 8; \
    if(col) {                             \
      col += pal_index;                   \
      col += 128;                         \
      ppu_set_pixel(OAM, x, col);         \
    }                                     \
  }                                       \
  x++

void ppu_render_oam_sprite(void) {
word pos, col, chr, tiledata_inc;
byte d0, d1, d2, d3, pal_index;
int x, y, z, x1, mx, mask, p;
int tile_width;
  if(ppu.bg_enabled[OAM] == false && ppu.ss_bg_enabled[OAM] == false)return;

  tile_width = current_sprite.width >> SH_8; //e.x. 16x16 sprite = 2x2 tiles

  y = ppu.vline_pos;
  x = current_sprite.x;
  if(current_sprite.v_flip) {
    y = (current_sprite.height - 1) - (ppu.vline_pos - current_sprite.y);
  } else {
    y = ppu.vline_pos - current_sprite.y;
  }
  y &= 255;

  chr = current_sprite.character;
  tiledata_inc = (chr & 0x100)?(ppu.oam_name_sel << 13):0; //*1
  chr += (y >> SH_8) << SH_16; //*2
  pal_index = (current_sprite.palette << SH_16);
  for(x1=0;x1<tile_width;x1++) {
    if(current_sprite.h_flip)mx = (tile_width - 1) - x1;
    else                     mx = x1;
    pos = ppu.oam_tiledata_loc + ((chr + mx) << SH_32) + ((y & 7) << SH_2) + tiledata_inc;
    d0 = ppu.vram[pos     ];
    d1 = ppu.vram[pos +  1];
    d2 = ppu.vram[pos + 16];
    d3 = ppu.vram[pos + 17];
    if(current_sprite.h_flip) {
      ppu_render_oam_tile_line(0x01);
      ppu_render_oam_tile_line(0x02);
      ppu_render_oam_tile_line(0x04);
      ppu_render_oam_tile_line(0x08);
      ppu_render_oam_tile_line(0x10);
      ppu_render_oam_tile_line(0x20);
      ppu_render_oam_tile_line(0x40);
      ppu_render_oam_tile_line(0x80);
    } else {
      ppu_render_oam_tile_line(0x80);
      ppu_render_oam_tile_line(0x40);
      ppu_render_oam_tile_line(0x20);
      ppu_render_oam_tile_line(0x10);
      ppu_render_oam_tile_line(0x08);
      ppu_render_oam_tile_line(0x04);
      ppu_render_oam_tile_line(0x02);
      ppu_render_oam_tile_line(0x01);
    }
  }
}

void ppu_render_line_oam(byte priority) {
int s;
  if(ppu.bg_enabled[OAM] != true && ppu.ss_bg_enabled[OAM] != true)return;

  s = 128;
  while(s--) {
    ppu_set_sprite_attributes(s);
    if(current_sprite.priority == priority) {
//if the sprite is within screen boundaries... render the current line from the sprite
      if(ppu.vline_pos >= current_sprite.y && ppu.vline_pos < (current_sprite.y + current_sprite.height)) {
        ppu_render_oam_sprite();
//or if the sprite is so close to the bottom of the screen that the bottom of it actually wraps back around to the top...
      } else if((current_sprite.y + current_sprite.height) >= 256 && ppu.vline_pos < ((current_sprite.y + current_sprite.height) & 255)) {
        ppu_render_oam_sprite();
      }
    }
  }
}

/*
  *1 - map_index
    The tilemap can be 32x32, 64x32, 32x64, or 64x64. Rather than expanding the width
    and height of the tilemap, the game instead stores duplicate tilemaps immediately
    following the previous ones. For example, if you were in 64x64 mode, there would
    be four tilemaps. Each tilemap would be 2048 bytes in size
    (32 tiles * 32 tiles * 2 bytes/tile), the first tilemap would make the top left
    corner, the second the top right, the third the bottom left, and the fourth, the
    bottom right. Because x / y are divided by the tile size, the tile size setting
    (8x8 or 16x16) does not affect the result.
  *2 - pos = ppu.bg_tilemap_loc[bg] + map_index + ((y1 / tile_size) & 31) * 64 + ( ((x / tile_size) & 31) * 2);
    Format: tilemap start location +
              map index (either map 0 or map 1; see *1) +
              ((y tile #) mapped to tilemap boundary) * # of bytes per tilemap line +
              (((x tile #) mapped to tilemap boundary) * 2 (# of bytes per tilemap entry));
*/

void ppu_render_line_bg(byte color_depth, byte bg, byte priority) {
int x, y, z, x1, y1;
int mirror_x, mirror_y, p;
int screen_x, screen_y;
int bg_x, bg_y;
int xpos, ypos, mosaic_x, mosaic_y;
word t, base_xpos, base_pos, pos, ppos = 0;
word col;
byte *src, *bg_tiledata, *bg_tiledata_state;
byte tiledata_size;
byte tile_size, tile_width, tile_height;
byte mask, pal_index, pal_size;
word tile_num, screen_width, screen_height, screen_width_mask, screen_height_mask, map_index;
word *mosaic_table;
  if(ppu.bg_enabled[bg] == false && ppu.ss_bg_enabled[bg] == false)return;

  switch(color_depth) {
  case COLORDEPTH_4:
    pal_size      =     4;
    tiledata_size = SH_16;
    break;
  case COLORDEPTH_16:
    pal_size      =    16;
    tiledata_size = SH_32;
    break;
  case COLORDEPTH_256:
    pal_size      =   256;
    tiledata_size = SH_64;
    break;
  }
  bg_tiledata       = (byte*)ppu_bg_tiledata[color_depth];
  bg_tiledata_state = (byte*)ppu_bg_tiledata_state[color_depth];

  screen_width  = render.snes_width;
  screen_height = render.snes_width; //this is correct -- ppu tilemap is based around 256x256, etc.
  tile_size     = (ppu.bg_tile_size[bg])?SH_16:SH_8;
  tile_width    = tile_size;
  tile_height   = tile_size;

  if(ppu.interlace == true && (ppu.bg_mode == 5 || ppu.bg_mode == 6)) {
    screen_y = (ppu.vline_pos << SH_2) + ppu.interlace_frame;
  } else {
    screen_y = ppu.vline_pos;
  }

//Not sure why, but modes 5 and 6 seem to force 16-width tiles.
//The tile size attribute in $2105 has no effect on tile width.
  if(ppu.bg_mode == 5 || ppu.bg_mode == 6) {
    tile_width = SH_16;
  }

  if(tile_size == SH_16) {
    screen_width  <<= SH_2;
    screen_height <<= SH_2;
  }

  if(ppu.bg_tilemap_size[bg] & 0x01)screen_width  <<= SH_2;
  if(ppu.bg_tilemap_size[bg] & 0x02)screen_height <<= SH_2;

  screen_width_mask  = screen_width  - 1;
  screen_height_mask = screen_height - 1;

  if(render.snes_width == 512) {
    bg_x = (ppu.bg_hscroll_pos[bg] << SH_2) & screen_width_mask;
  } else {
    bg_x = ppu.bg_hscroll_pos[bg] & screen_width_mask;
  }
  if(render.snes_height == 448) {
    bg_y = (screen_y + ((ppu.bg_vscroll_pos[bg] << SH_2) & screen_height_mask)) & screen_height_mask;
  } else {
    bg_y = (screen_y + (ppu.bg_vscroll_pos[bg] & screen_height_mask)) & screen_height_mask;
  }

  mosaic_table = (word*)ppu.mosaic_table[ppu.mosaic_size];
  mosaic_y     = mosaic_table[bg_y];

  for(screen_x=0;screen_x<render.snes_width;screen_x++) {
    switch(ppu.bg_tilemap_size[bg]) {
    case 0:
      map_index  = 0;
      break;
    case 1:
      map_index  = ((bg_x >> tile_size) > 31)?32*32*2:0;
      break;
    case 2:
      map_index  = ((bg_y >> tile_size) > 31)?32*32*2:0;
      break;
    case 3:
      map_index  = ((bg_x >> tile_size) > 31)?32*32*2:0;
      map_index += ((bg_y >> tile_size) > 31)?32*32*2*2:0;
      break;
    }

    mosaic_x = mosaic_table[bg_x];

    base_xpos = ((mosaic_x >> SH_8) & 31);
    base_pos  = (((mosaic_y >> tile_height) & 31) << SH_32) + ((mosaic_x >> tile_width) & 31);
    pos       = ppu.bg_tilemap_loc[bg] + map_index + (base_pos << SH_2);
    t         = *((word*)ppu.vram + (pos >> SH_2));
    mirror_y  = (t & 0x8000)?1:0;
    mirror_x  = (t & 0x4000)?1:0;
    if(((t >> 13) & 1) == priority) {
      tile_num = t & 0x03ff;
      if(tile_width == SH_16) {
        if((mosaic_x & 15) >= 8)tile_num++;
        if(mirror_x) {
          if((mosaic_x & 15) >= 8)tile_num--;
          else                    tile_num++;
        }
        tile_num &= 0x03ff;
      }
      if(tile_height == SH_16) {
        if((mosaic_y & 15) >= 8)tile_num += 16;
        if(mirror_y) {
          if((mosaic_y & 15) >= 8)tile_num -= 16;
          else                    tile_num += 16;
        }
        tile_num &= 0x03ff;
      }
      tile_num += (ppu.bg_tiledata_loc[bg] >> tiledata_size);

      if(bg_tiledata_state[tile_num] == 1) {
        ppu_render_bg_tile(color_depth, bg, tile_num);
      }

      pal_index = ((t >> 10) & 7) * pal_size;

      if(mirror_y) { ypos = (7 - (mosaic_y & 7)); }
      else         { ypos = (    (mosaic_y & 7)); }

//loop while we are rendering from the same tile, as there's no need to do all of the above work
//unless we have rendered all of the visible tile, taking mosaic into account.
      while(1) {
        if(mirror_x) { xpos = (7 - (mosaic_x & 7)); }
        else         { xpos = (    (mosaic_x & 7)); }
        col  = *(bg_tiledata + (tile_num << SH_64) + (ypos << SH_8) + (xpos));
        if(col) {
          ppu_set_pixel(bg, screen_x, col + pal_index);
        }

        bg_x++;
        bg_x &= screen_width_mask;
        mosaic_x = mosaic_table[bg_x];

        if(base_xpos != ((mosaic_x >> SH_8) & 31))break;
        screen_x++;
        if(screen_x >= render.snes_width)break;
      }
    } else {
      while(1) {
        bg_x++;
        bg_x &= screen_width_mask;
        mosaic_x = mosaic_table[bg_x];

        if(base_xpos != ((mosaic_x >> SH_8) & 31))break;
        screen_x++;
        if(screen_x >= render.snes_width)break;
      }
    }
  }
}

#ifdef PUBLIC_DOMAIN
  #include "ppu_render_mode7f.cpp"
#else
  #include "ppu_render_mode7i.cpp"
#endif
