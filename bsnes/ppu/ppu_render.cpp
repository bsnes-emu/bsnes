byte ppu_addsub_adjust_buffer_full[96] = {
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31
};

byte ppu_addsub_adjust_buffer_half[96] = {
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
  32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63
};

#define ppu_pal_pixel(__i) \
  (*((word*)ppu.cgram + __i))

word ppu_addsub_pixels(byte x, byte cdest_index, byte cdest_bg, byte csrc_index, byte csrc_bg) {
int r, g, b;
word cdest = ppu_pal_pixel(cdest_index);
word csrc  = ppu_pal_pixel(csrc_index);
word res;
//oam palettes 0-3 are not affected by color add/sub
  if(cdest_bg == OAM) {
    if(cdest_index < 192) {
      return cdest;
    }
  }
  switch(ppu.color_mode) {
  case COLORMODE_ADD:
    if(ppu.bg_color_enabled[cdest_bg] == true && ppu.color_halve == 1) {
      r = *(ppu_addsub_adjust_buffer_half + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) )) >> 1;
      g = *(ppu_addsub_adjust_buffer_half + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) )) >> 1;
      b = *(ppu_addsub_adjust_buffer_half + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) )) >> 1;
    } else {
      r = *(ppu_addsub_adjust_buffer_full + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) ));
      g = *(ppu_addsub_adjust_buffer_full + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) ));
      b = *(ppu_addsub_adjust_buffer_full + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) ));
    }
    break;
  case COLORMODE_SUB:
    if(ppu.bg_color_enabled[cdest_bg] == true && ppu.color_halve == 1) {
      r = *(ppu_addsub_adjust_buffer_half + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) )) >> 1;
      g = *(ppu_addsub_adjust_buffer_half + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) )) >> 1;
      b = *(ppu_addsub_adjust_buffer_half + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) )) >> 1;
    } else {
      r = *(ppu_addsub_adjust_buffer_full + 32 + ( ((cdest      ) & 31) - ((csrc      ) & 31) ));
      g = *(ppu_addsub_adjust_buffer_full + 32 + ( ((cdest >>  5) & 31) - ((csrc >>  5) & 31) ));
      b = *(ppu_addsub_adjust_buffer_full + 32 + ( ((cdest >> 10) & 31) - ((csrc >> 10) & 31) ));
    }
    break;
  }
  return ((r) | (g << 5) | (b << 10));
}

word ppu_addsub_pixel(byte x, byte cdest_index, byte cdest_bg) {
int r, g, b;
word cdest = ppu_pal_pixel(cdest_index);
word csrc  = (ppu.color_r) | (ppu.color_g << 5) | (ppu.color_b << 10);
word res;
//only oam palettes 4-7 are affected by color add/sub
  if(cdest_bg == OAM) {
    if(cdest_index < 192) {
      return cdest;
    }
  }
  switch(ppu.color_mode) {
  case COLORMODE_ADD:
    if(ppu.bg_color_enabled[cdest_bg] == true && ppu.color_halve == 1 && ppu.addsub_mode == 0) {
      r = *(ppu_addsub_adjust_buffer_half + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) )) >> 1;
      g = *(ppu_addsub_adjust_buffer_half + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) )) >> 1;
      b = *(ppu_addsub_adjust_buffer_half + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) )) >> 1;
    } else {
      r = *(ppu_addsub_adjust_buffer_full + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) ));
      g = *(ppu_addsub_adjust_buffer_full + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) ));
      b = *(ppu_addsub_adjust_buffer_full + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) ));
    }
    break;
  case COLORMODE_SUB:
    if(ppu.bg_color_enabled[cdest_bg] == true && ppu.color_halve == 1 && ppu.addsub_mode == 0) {
      r = *(ppu_addsub_adjust_buffer_half + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) )) >> 1;
      g = *(ppu_addsub_adjust_buffer_half + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) )) >> 1;
      b = *(ppu_addsub_adjust_buffer_half + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) )) >> 1;
    } else {
      r = *(ppu_addsub_adjust_buffer_full + 32 + ( ((cdest      ) & 31) - ((csrc      ) & 31) ));
      g = *(ppu_addsub_adjust_buffer_full + 32 + ( ((cdest >>  5) & 31) - ((csrc >>  5) & 31) ));
      b = *(ppu_addsub_adjust_buffer_full + 32 + ( ((cdest >> 10) & 31) - ((csrc >> 10) & 31) ));
    }
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

bool windows_not_obstructing(byte layer, byte bg, word x);
bool color_windows_not_obstructing(word x, byte color_mask_type);

void ppu_render_line_to_screen(void) {
int x, x1;
word *ptr, *ptri, *light_table, *light_tablei;
word c, cx, cy;
word screen_width = render.snes_width;

  if(ppu.interlace == false) {
    ptr  = (word*)ppu.screen + ((ppu.vline_pos << 1)    ) * 512;
    ptri = (word*)ppu.screen + ((ppu.vline_pos << 1) + 1) * 512;
  } else {
    ptr  = (word*)ppu.screen + ((ppu.vline_pos << 1) + ppu.interlace_frame) * 512;
  }
  light_table = (word*)ppu.light_table + (ppu.display_brightness * 65536);

  for(x=x1=0;x<screen_width;x++) {
    switch(ppu_pixel_cache[x].blend_type) {
    case BLENDTYPE_BACK:
      if(color_windows_not_obstructing(x, PPU_MAIN) == true) {
        cx = 0x0000;
      } else if(ppu.bg_color_enabled[BACK] == true && color_windows_not_obstructing(x, PPU_SUB) == false) {
        cx = ppu_addsub_pixel(x, 0, BACK);
      } else {
        cx = ppu_pal_pixel(0);
      }
      break;
    case BLENDTYPE_MAIN:
      if(ppu.bg_color_enabled[ppu_pixel_cache[x].src_main] == true && color_windows_not_obstructing(x, PPU_MAIN) == true) {
        cx = 0x0000;
      } else if(ppu.bg_color_enabled[ppu_pixel_cache[x].src_main] == true && color_windows_not_obstructing(x, PPU_SUB) == false) {
        cx = ppu_addsub_pixel(x, ppu_pixel_cache[x].color_main, ppu_pixel_cache[x].src_main);
      } else {
        cx = ppu_pal_pixel(ppu_pixel_cache[x].color_main);
      }
      break;
    case BLENDTYPE_SUB:
      if(ppu.bg_color_enabled[BACK] && color_windows_not_obstructing(x, PPU_SUB) == false) {
        cx = ppu_addsub_pixels(x, 0, BACK, ppu_pixel_cache[x].color_sub, ppu_pixel_cache[x].src_sub);
      } else {
        cx = ppu_pal_pixel(ppu_pixel_cache[x].color_sub);
      }
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
      break;
    }

    if(ppu.interlace == false) {
      *(ptr  + (x1  )) = *(light_table + cx);
      *(ptri + (x1++)) = *(light_table + cx);
      if(screen_width != 256)continue;
      *(ptr  + (x1  )) = *(light_table + cx);
      *(ptri + (x1++)) = *(light_table + cx);
    } else {
      *(ptr  + (x1++)) = *(light_table + cx);
      if(screen_width != 256)continue;
      *(ptr  + (x1++)) = *(light_table + cx);
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

void ppu_set_layer_pixels(byte layer_count, byte *layer_bg_lookup) {
int  layer, x = 0, x1;
byte pal;
  do {
    layer = 0;
    x1 = x * 12;
    do {
      pal = ppu_layer_cache[x1 + layer];
      if(pal) {
        ppu_set_pixel(layer_bg_lookup[layer], x, pal);
      }
      layer++;
    } while(layer < layer_count);
    x++;
  } while(x < render.snes_width);
}

#define ppu_set_layer_pixel(__x, __c) ppu_layer_cache[(__x) * 12 + layer_pos] = __c

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
  t = *((ulong*)ppu.oam + sprite_num);
  b = ppu.oam[512 + (sprite_num >> 2)];

  switch(sprite_num & 3) {
  case 0: size = (b & 0x02)?1:0; x = (b & 0x01)?0x100:0; break;
  case 1: size = (b & 0x08)?1:0; x = (b & 0x04)?0x100:0; break;
  case 2: size = (b & 0x20)?1:0; x = (b & 0x10)?0x100:0; break;
  case 3: size = (b & 0x80)?1:0; x = (b & 0x40)?0x100:0; break;
  }

  current_sprite.num       = sprite_num;
  current_sprite.priority  = (t >> 28) & 3;
  current_sprite.x         = x | (t & 0xff);
  current_sprite.y         = ((t >> 8) + 1) & 0xff;
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

bool windows_not_obstructing(byte layer, byte bg, word x) {
byte w1_mask, w2_mask; //1 = masked, 0 = not masked
word window1_left, window1_right, window2_left, window2_right;
  if(layer == PPU_MAIN) {
    if(ppu.bg_windowing_enabled[bg] == false)return true;
  } else if(layer == PPU_SUB) {
    if(ppu.ss_bg_windowing_enabled[bg] == false)return true;
  }

  window1_left  = ppu.window1_left;
  window1_right = ppu.window1_right;
  window2_left  = ppu.window2_left;
  window2_right = ppu.window2_right;

  if(ppu.bg_mode == 5 || ppu.bg_mode == 6) {
    window1_left  <<= 1;
    window1_right <<= 1;
    window2_left  <<= 1;
    window2_right <<= 1;
  }

  if(ppu.bg_window1_enabled[bg] == true && ppu.bg_window2_enabled[bg] == false) {
    if(ppu.bg_window1_clipmode[bg] == CLIPMODE_IN) {
      if(x >= window1_left && x <= window1_right)return false;
      return true;
    } else {
      if(x <  window1_left || x >  window1_right)return false;
      return true;
    }
  } else if(ppu.bg_window2_enabled[bg] == true && ppu.bg_window1_enabled[bg] == false) {
    if(ppu.bg_window2_clipmode[bg] == CLIPMODE_IN) {
      if(x >= window2_left && x <= window2_right)return false;
      return true;
    } else {
      if(x <  window2_left || x >  window2_right)return false;
      return true;
    }
  } else if(ppu.bg_window1_enabled[bg] == true && ppu.bg_window2_enabled[bg] == true) {
    if(ppu.bg_window1_clipmode[bg] == CLIPMODE_IN) {
      if(x >= window1_left && x <= window1_right)w1_mask = 1;
      else w1_mask = 0;
    } else {
      if(x <  window1_left || x >  window1_right)w1_mask = 1;
      else w1_mask = 0;
    }

    if(ppu.bg_window2_clipmode[bg] == CLIPMODE_IN) {
      if(x >= window2_left && x <= window2_right)w2_mask = 1;
      else w2_mask = 0;
    } else {
      if(x <  window2_left || x >  window2_right)w2_mask = 1;
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

bool color_windows_not_obstructing(word x, byte color_mask_type) {
byte w1_mask, w2_mask; //1 = masked, 0 = not masked
byte color_mask;
bool r;
word window1_left, window1_right, window2_left, window2_right;
  if(color_mask_type == PPU_MAIN)color_mask = ppu.color_mask;
  else                           color_mask = ppu.ss_color_mask;

  if(color_mask == 0)return false;
  if(color_mask == 3)return true;

  window1_left  = ppu.window1_left;
  window1_right = ppu.window1_right;
  window2_left  = ppu.window2_left;
  window2_right = ppu.window2_right;

  if(ppu.bg_mode == 5 || ppu.bg_mode == 6) {
    window1_left  <<= 1;
    window1_right <<= 1;
    window2_left  <<= 1;
    window2_right <<= 1;
  }

  if(ppu.color_window1_enabled == false && ppu.color_window2_enabled == false) {
    r = true;
  } else if(ppu.color_window1_enabled == true && ppu.color_window2_enabled == false) {
    if(ppu.color_window1_clipmode == CLIPMODE_IN) {
      if(x >= window1_left && x <= window1_right)r = false;
      else r = true;
    } else {
      if(x <  window1_left || x >  window1_right)r = false;
      else r = true;
    }
  } else if(ppu.color_window1_enabled == false && ppu.color_window2_enabled == true) {
    if(ppu.color_window2_clipmode == CLIPMODE_IN) {
      if(x >= window2_left && x <= window2_right)r = false;
      else r = true;
    } else {
      if(x <  window2_left || x >  window2_right)r = false;
      else r = true;
    }
  } else if(ppu.color_window1_enabled == true && ppu.color_window2_enabled == true) {
    if(ppu.color_window1_clipmode == CLIPMODE_IN) {
      if(x >= window1_left && x <= window1_right)w1_mask = 1;
      else w1_mask = 0;
    } else {
      if(x <  window1_left || x >  window1_right)w1_mask = 1;
      else w1_mask = 0;
    }

    if(ppu.color_window2_clipmode == CLIPMODE_IN) {
      if(x >= window2_left && x <= window2_right)w2_mask = 1;
      else w2_mask = 0;
    } else {
      if(x <  window2_left || x >  window2_right)w2_mask = 1;
      else w2_mask = 0;
    }

    switch(ppu.color_window_mask) {
    case WINDOWMASK_OR:
      if((w1_mask | w2_mask) == 1)r = false;
      else r = true;
      break;
    case WINDOWMASK_AND:
      if((w1_mask & w2_mask) == 1)r = false;
      else r = true;
      break;
    case WINDOWMASK_XOR:
      if((w1_mask ^ w2_mask) == 1)r = false;
      else r = true;
      break;
    case WINDOWMASK_XNOR:
      if((w1_mask ^ w2_mask) == 0)r = false;
      else r = true;
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

#define OAM_PRI_NONE 4
byte ppu_oam_line_pal[512], ppu_oam_line_pri[512];

void ppu_render_oam_sprite(void) {
word pos, col, chr, tiledata_inc;
byte d0, d1, d2, d3, pal_index;
int x, y, z, x1, mx, mask, p;
int tile_width;
  if(ppu.bg_enabled[OAM] == false && ppu.ss_bg_enabled[OAM] == false)return;

  tile_width = current_sprite.width >> SH_8; //e.x. 16x16 sprite = 2x2 tiles

  if(ppu.interlace == true && (ppu.bg_mode == 5 || ppu.bg_mode == 6)) {
    y = (ppu.vline_pos << SH_2) + ppu.interlace_frame;
  } else {
    y = ppu.vline_pos;
  }

  x = current_sprite.x;
  if(render.snes_width == 512) {
    x <<= SH_2;
  }

  if(current_sprite.v_flip) {
    y = ((current_sprite.height - 1) - (ppu.vline_pos - current_sprite.y));
  } else {
    y = (ppu.vline_pos - current_sprite.y);
  }
  y &= 255;
  if(ppu.sprite_halve == true) {
    y <<= 1;
    y += ppu.interlace_frame;
  }

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
    for(z=0;z<8;z++) {
      if(current_sprite.h_flip) {
        mask = 0x01 << z;
      } else {
        mask = 0x80 >> z;
      }
      x &= 511;
      if(x < render.snes_width) {
        col = 0;
        if(d0 & mask)col += 1;
        if(d1 & mask)col += 2;
        if(d2 & mask)col += 4;
        if(d3 & mask)col += 8;
        if(col) {
          col += pal_index;
          col += 128;
          if(ppu_oam_line_pri[x] == OAM_PRI_NONE) {
            ppu_oam_line_pal[x] = col;
            ppu_oam_line_pri[x] = current_sprite.priority;
          }
          if(render.snes_width == 512) {
            if(ppu_oam_line_pri[x + 1] == OAM_PRI_NONE) {
              ppu_oam_line_pal[x + 1] = col;
              ppu_oam_line_pri[x + 1] = current_sprite.priority;
            }
          }
        }
      }
      x++;
      if(render.snes_width == 512) {
        x++;
      }
    }
  }
}

/*

*/

void ppu_render_line_oam(byte layer_pos_pri0, byte layer_pos_pri1, byte layer_pos_pri2, byte layer_pos_pri3) {
int i, s;
byte layer_pos;
  if(ppu.bg_enabled[OAM] != true && ppu.ss_bg_enabled[OAM] != true)return;

  memset(ppu_oam_line_pri, OAM_PRI_NONE, 512);
  for(s=0;s<128;s++) {
    ppu_set_sprite_attributes(s);
    if(ppu.sprite_halve == false) {
      if(ppu.vline_pos >= current_sprite.y && ppu.vline_pos < (current_sprite.y + current_sprite.height)) {
        ppu_render_oam_sprite();
      } else if((current_sprite.y + current_sprite.height) >= 256 && ppu.vline_pos < ((current_sprite.y + current_sprite.height) & 255)) {
        ppu_render_oam_sprite();
      }
    } else {
      if(ppu.vline_pos >= current_sprite.y && ppu.vline_pos < (current_sprite.y + (current_sprite.height >> 1))) {
        ppu_render_oam_sprite();
      } else if((current_sprite.y + current_sprite.height) >= 256 && ppu.vline_pos < ((current_sprite.y + (current_sprite.height >> 1)) & 255)) {
        ppu_render_oam_sprite();
      }
    }
  }

  for(i=0;i<render.snes_width;i++) {
    if(ppu_oam_line_pri[i] != OAM_PRI_NONE) {
      switch(ppu_oam_line_pri[i]) {
      case 0:layer_pos = layer_pos_pri0;break;
      case 1:layer_pos = layer_pos_pri1;break;
      case 2:layer_pos = layer_pos_pri2;break;
      case 3:layer_pos = layer_pos_pri3;break;
      }
      ppu_set_layer_pixel(i, ppu_oam_line_pal[i]);
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

void ppu_render_line_bg(byte layer_pos_pri0, byte layer_pos_pri1, byte color_depth, byte bg) {
int x, y, z, x1, y1;
int mirror_x, mirror_y, p;
int screen_x, screen_y;
int bg_x, bg_y;
int xpos, ypos, mosaic_x, mosaic_y;
word t, base_xpos, base_pos, pos, ppos = 0;
word col;
byte *src, *bg_tiledata, *bg_tiledata_state, *tile_ptr;
byte tiledata_size;
byte tile_size, tile_width, tile_height, tile_x;
byte mask, pal_index, pal_size;
word tile_num, screen_width, screen_height, screen_width_mask, screen_height_mask, map_index;
word *mosaic_table;
byte layer_pos;
word opt_valid_bit, voffset, hoffset, vscroll, hscroll;
  if(ppu.bg_enabled[bg] == false && ppu.ss_bg_enabled[bg] == false)return;

  if     (bg == BG1)opt_valid_bit = 0x2000;
  else if(bg == BG2)opt_valid_bit = 0x4000;
  else              opt_valid_bit = 0x0000;

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

//Modes 5 and 6 seem to force 16-width tiles due to having twice the resolution.
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
    hscroll = (ppu.bg_hscroll_pos[bg] << SH_2) & screen_width_mask;
  } else {
    hscroll = ppu.bg_hscroll_pos[bg] & screen_width_mask;
  }
  bg_x = hscroll;

  if(render.snes_height == 448) {
    vscroll = (ppu.bg_vscroll_pos[bg] << SH_2) & screen_height_mask;
  } else {
    vscroll = ppu.bg_vscroll_pos[bg] & screen_height_mask;
  }
  bg_y = (screen_y + vscroll) & screen_height_mask;

  if(ppu.mosaic_enabled[bg] == true) {
    mosaic_table = (word*)ppu.mosaic_table[ppu.mosaic_size];
  } else {
    mosaic_table = (word*)ppu.mosaic_table[0];
  }
  mosaic_x = mosaic_table[bg_x];
  mosaic_y = mosaic_table[bg_y];

  for(screen_x=0;screen_x<render.snes_width;screen_x++) {
    if(ppu.bg_mode == 2 || ppu.bg_mode == 4 || ppu.bg_mode == 6) {
      if(ppu.bg_mode == 6) {
        tile_x = (mosaic_table[screen_x + (hscroll & 15)] >> SH_16);
      } else {
        tile_x = (mosaic_table[screen_x + (hscroll &  7)] >>  SH_8);
      }
      hoffset = hscroll;
      voffset = vscroll;
      if(tile_x != 0) {
        tile_x = (tile_x - 1) & 31;
        if(ppu.bg_mode == 4) {
          pos = ppu.bg_tilemap_loc[BG3] + (tile_x << SH_2);
          t   = *((word*)ppu.vram + (pos >> SH_2));
          if(t & opt_valid_bit) {
            if(!(t & 0x8000)) {
              hoffset = ((t & 0x1ff8) | (hscroll & 7)) & screen_width_mask;
            } else {
              voffset = (t & 0x1fff) & screen_height_mask;
            }
          }
        } else {
          pos = ppu.bg_tilemap_loc[BG3] + (tile_x << SH_2);
          t   = *((word*)ppu.vram + (pos >> SH_2));
          if(t & opt_valid_bit) {
            hoffset = ((t & 0x1ff8) | (hscroll & 7)) & screen_width_mask;
          }
          pos = ppu.bg_tilemap_loc[BG3] + 64 + (tile_x << SH_2);
          t   = *((word*)ppu.vram + (pos >> SH_2));
          if(t & opt_valid_bit) {
            voffset = (t & 0x1fff) & screen_height_mask;
          }
        }
      }
      mosaic_x = mosaic_table[(screen_x + hoffset) & screen_width_mask ];
      mosaic_y = mosaic_table[(screen_y + voffset) & screen_height_mask];
    }

    switch(ppu.bg_tilemap_size[bg]) {
    case 0:
      map_index = 0;
      break;
    case 1:
      map_index = ((mosaic_x >> tile_size) >> SH_32) << SH_2048;
      break;
    case 2:
      map_index = ((mosaic_y >> tile_size) >> SH_32) << SH_2048;
      break;
    case 3:
      map_index = ((mosaic_x >> tile_size) >> SH_32) << SH_2048 |
                  ((mosaic_y >> tile_size) >> SH_32) << SH_4096;
      break;
    }

    base_xpos = ((mosaic_x >> SH_8) & 31);
    base_pos  = (((mosaic_y >> tile_height) & 31) << SH_32) + ((mosaic_x >> tile_width) & 31);
    pos       = ppu.bg_tilemap_loc[bg] + map_index + (base_pos << SH_2);
    t         = *((word*)ppu.vram + (pos >> SH_2));
    mirror_y  = (t & 0x8000)?1:0;
    mirror_x  = (t & 0x4000)?1:0;

    if((t & 0x2000) == 0) {
      layer_pos = layer_pos_pri0;
    } else {
      layer_pos = layer_pos_pri1;
    }

    tile_num = t & 0x03ff;
    if(tile_width == SH_16) {
      if(((mosaic_x & 15) >= 8 && !mirror_x) ||
         ((mosaic_x & 15) <  8 &&  mirror_x))tile_num++;
      tile_num &= 0x03ff;
    }
    if(tile_height == SH_16) {
      if(((mosaic_y & 15) >= 8 && !mirror_y) ||
         ((mosaic_y & 15) <  8 &&  mirror_y))tile_num += 16;
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
    tile_ptr = (byte*)bg_tiledata + (tile_num << SH_64) + (ypos << SH_8);
    while(1) {
      if(mirror_x) { xpos = (7 - (mosaic_x & 7)); }
      else         { xpos = (    (mosaic_x & 7)); }
      col = *(tile_ptr + xpos);
      if(col) {
        ppu_set_layer_pixel(screen_x, col + pal_index);
      }

      bg_x++;
      bg_x &= screen_width_mask;
      mosaic_x = mosaic_table[bg_x];

      if(base_xpos != ((mosaic_x >> SH_8) & 31))break;
      screen_x++;
      if(screen_x >= render.snes_width)break;
    }
  }
}

#include "ppu_render_mode7.cpp"
