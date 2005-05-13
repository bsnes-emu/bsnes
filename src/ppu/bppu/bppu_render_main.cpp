#define set_layer_pixel(__x, __c) layer_cache[(__x) * 12 + layer_pos] = __c
#define pal_pixel(__i) (*((uint16*)cgram + __i))

namespace bPPUAddSubTables {
  uint8 adjust_buffer_full[96] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31
  };
  uint8 adjust_buffer_half[96] = {
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63
  };
};

uint16 bPPU::addsub_pixels(uint8 x, uint8 cdest_index, uint8 cdest_bg, uint8 csrc_index, uint8 csrc_bg) {
int r, g, b;
uint16 cdest = pal_pixel(cdest_index);
uint16 csrc  = pal_pixel(csrc_index);
uint16 res;
//oam palettes 0-3 are not affected by color add/sub
  if(cdest_bg == OAM) {
    if(cdest_index < 192) {
      return cdest;
    }
  }
  switch(regs.color_mode) {
  case COLORMODE_ADD:
    if(regs.bg_color_enabled[cdest_bg] == true && regs.color_halve == true) {
      r = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) )) >> 1;
      g = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) )) >> 1;
      b = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) )) >> 1;
    } else {
      r = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) ));
      g = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) ));
      b = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) ));
    }
    break;
  case COLORMODE_SUB:
    if(regs.bg_color_enabled[cdest_bg] == true && regs.color_halve == true) {
      r = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) )) >> 1;
      g = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) )) >> 1;
      b = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) )) >> 1;
    } else {
      r = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest      ) & 31) - ((csrc      ) & 31) ));
      g = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >>  5) & 31) - ((csrc >>  5) & 31) ));
      b = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >> 10) & 31) - ((csrc >> 10) & 31) ));
    }
    break;
  }
  return ((r) | (g << 5) | (b << 10));
}

uint16 bPPU::addsub_pixel(uint8 x, uint8 cdest_index, uint8 cdest_bg) {
int r, g, b;
uint16 cdest = pal_pixel(cdest_index);
uint16 csrc  = (regs.color_r) | (regs.color_g << 5) | (regs.color_b << 10);
uint16 res;
//only oam palettes 4-7 are affected by color add/sub
  if(cdest_bg == OAM) {
    if(cdest_index < 192) {
      return cdest;
    }
  }
  switch(regs.color_mode) {
  case COLORMODE_ADD:
    if(regs.bg_color_enabled[cdest_bg] == true && regs.color_halve == true && regs.addsub_mode == 0) {
      r = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) )) >> 1;
      g = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) )) >> 1;
      b = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) )) >> 1;
    } else {
      r = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) ));
      g = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) ));
      b = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) ));
    }
    break;
  case COLORMODE_SUB:
    if(regs.bg_color_enabled[cdest_bg] == true && regs.color_halve == true && regs.addsub_mode == 0) {
      r = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest      ) & 31) + ((csrc      ) & 31) )) >> 1;
      g = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >>  5) & 31) + ((csrc >>  5) & 31) )) >> 1;
      b = *(bPPUAddSubTables::adjust_buffer_half + 32 + ( ((cdest >> 10) & 31) + ((csrc >> 10) & 31) )) >> 1;
    } else {
      r = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest      ) & 31) - ((csrc      ) & 31) ));
      g = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >>  5) & 31) - ((csrc >>  5) & 31) ));
      b = *(bPPUAddSubTables::adjust_buffer_full + 32 + ( ((cdest >> 10) & 31) - ((csrc >> 10) & 31) ));
    }
    break;
  }
  return ((r) | (g << 5) | (b << 10));
}

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

void bPPU::render_bg_tile(uint8 color_depth, uint8 bg, uint16 tile_num) {
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

void bPPU::render_line_to_output() {
int x, x1;
uint16 *ptr, *ptri, *ltable;
uint16 c, cx, cy;
uint16 screen_width = (regs.bg_mode == 5 || regs.bg_mode == 6)?512:256;
uint16 vline_pos = clock->vcounter();

  if(clock->interlace() == false) {
    ptr  = (uint16*)output + ((vline_pos << 1)    ) * 512;
    ptri = (uint16*)output + ((vline_pos << 1) + 1) * 512;
  } else {
    ptr  = (uint16*)output + ((vline_pos << 1) + clock->interlace_field()) * 512;
  }
  ltable = (uint16*)light_table + (regs.display_brightness * 65536);

  for(x=x1=0;x<screen_width;x++) {
    switch(pixel_cache[x].blend_type) {
    case BLENDTYPE_BACK:
      if(color_windows_not_obstructing(x, PPU_MAIN) == true) {
        cx = 0x0000;
      } else if(regs.bg_color_enabled[BACK] == true && color_windows_not_obstructing(x, PPU_SUB) == false) {
        cx = addsub_pixel(x, 0, BACK);
      } else {
        cx = pal_pixel(0);
      }
      break;
    case BLENDTYPE_MAIN:
      if(regs.bg_color_enabled[pixel_cache[x].src_main] == true && color_windows_not_obstructing(x, PPU_MAIN) == true) {
        cx = 0x0000;
      } else if(regs.bg_color_enabled[pixel_cache[x].src_main] == true && color_windows_not_obstructing(x, PPU_SUB) == false) {
        cx = addsub_pixel(x, pixel_cache[x].color_main, pixel_cache[x].src_main);
      } else {
        cx = pal_pixel(pixel_cache[x].color_main);
      }
      break;
    case BLENDTYPE_SUB:
      if(regs.bg_color_enabled[BACK] && color_windows_not_obstructing(x, PPU_SUB) == false) {
        cx = addsub_pixels(x, 0, BACK, pixel_cache[x].color_sub, pixel_cache[x].src_sub);
      } else {
        cx = pal_pixel(pixel_cache[x].color_sub);
      }
      break;
    case BLENDTYPE_COMBINE:
      if(color_windows_not_obstructing(x, PPU_SUB) == false) {
        if(pixel_cache[x].src_sub == BACK) {
          cx = addsub_pixels(x, pixel_cache[x].color_main, pixel_cache[x].src_main, 0, BACK);
        } else {
          cx = addsub_pixels(x, pixel_cache[x].color_main, pixel_cache[x].src_main,
                                pixel_cache[x].color_sub,  pixel_cache[x].src_sub);
        }
      } else {
        cx = pal_pixel(pixel_cache[x].color_main);
      }
      break;
    }

    if(clock->interlace() == false) {
      *(ptr  + (x1  )) = *(ltable + cx);
      *(ptri + (x1++)) = *(ltable + cx);
      if(screen_width != 256)continue;
      *(ptr  + (x1  )) = *(ltable + cx);
      *(ptri + (x1++)) = *(ltable + cx);
    } else {
      *(ptr  + (x1++)) = *(ltable + cx);
      if(screen_width != 256)continue;
      *(ptr  + (x1++)) = *(ltable + cx);
    }
  }
}

void bPPU::set_pixel(uint8 bg, uint16 x, uint8 pal_index) {
  if(regs.bg_enabled[bg] == true && regs.bgsub_enabled[bg] == true) {
    if(windows_not_obstructing(PPU_MAIN, bg, x) == false)return;
    pixel_cache[x].blend_type = BLENDTYPE_MAIN;
    pixel_cache[x].src_main   = bg;
    pixel_cache[x].color_main = pal_index;
    if(color_windows_not_obstructing(x, PPU_SUB) == false) {
      pixel_cache[x].src_sub    = bg;
      pixel_cache[x].color_sub  = pal_index;
    }
  } else if(regs.bg_enabled[bg] == true && bg == OAM && pal_index < 192) {
    if(windows_not_obstructing(PPU_MAIN, bg, x) == false)return;
    pixel_cache[x].blend_type = BLENDTYPE_MAIN;
    pixel_cache[x].src_main   = bg;
    pixel_cache[x].color_main = pal_index;
  } else if(regs.bg_enabled[bg] == true) {
    if(windows_not_obstructing(PPU_MAIN, bg, x) == false)return;
    if(regs.bg_color_enabled[bg] == true && pixel_cache[x].src_sub != BACK) {
      pixel_cache[x].blend_type = BLENDTYPE_COMBINE;
    } else {
      pixel_cache[x].blend_type = BLENDTYPE_MAIN;
    }
    pixel_cache[x].src_main   = bg;
    pixel_cache[x].color_main = pal_index;
  } else if(regs.bgsub_enabled[bg] == true) {
    if(windows_not_obstructing(PPU_SUB, bg, x) == false)return;
    pixel_cache[x].src_sub    = bg;
    pixel_cache[x].color_sub  = pal_index;
    if(pixel_cache[x].blend_type == BLENDTYPE_BACK) {
      pixel_cache[x].blend_type = BLENDTYPE_SUB;
    } else if(pixel_cache[x].blend_type == BLENDTYPE_MAIN) {
      if(pixel_cache[x].src_main != OAM || (pixel_cache[x].src_main == OAM && pixel_cache[x].color_main >= 192)) {
        if(regs.bg_color_enabled[pixel_cache[x].src_main] == true) {
          pixel_cache[x].blend_type = BLENDTYPE_COMBINE;
        }
      }
    }
  }
}

void bPPU::set_layer_pixels(uint8 layer_count, uint8 *layer_bg_lookup) {
int layer, x = 0, x1;
uint8 pal;
uint16 width = (regs.bg_mode == 5 || regs.bg_mode == 6)?512:256;
  do {
    layer = 0;
    x1 = x * 12;
    do {
      pal = layer_cache[x1 + layer];
      if(pal) {
        set_pixel(layer_bg_lookup[layer], x, pal);
      }
      layer++;
    } while(layer < layer_count);
    x++;
  } while(x < width);
}

void bPPU::set_sprite_attributes(uint8 sprite_num) {
uint32 t;
uint8  size, b;
uint16 x;
  t = *((uint32*)oam + sprite_num);
  b = oam[512 + (sprite_num >> 2)];

  switch(sprite_num & 3) {
  case 0: size = !!(b & 0x02); x = (b & 0x01)?0x100:0; break;
  case 1: size = !!(b & 0x08); x = (b & 0x04)?0x100:0; break;
  case 2: size = !!(b & 0x20); x = (b & 0x10)?0x100:0; break;
  case 3: size = !!(b & 0x80); x = (b & 0x40)?0x100:0; break;
  }

  current_sprite.num       = sprite_num;
  current_sprite.priority  = (t >> 28) & 3;
  current_sprite.x         = x | (t & 0xff);
  current_sprite.y         = ((t >> 8) + 1) & 0xff;
  current_sprite.v_flip    = !!(t & 0x80000000);
  current_sprite.h_flip    = !!(t & 0x40000000);
  current_sprite.palette   = (t >> 25) & 7;
  current_sprite.character = (t >> 16) & 0x01ff;

//size: 0 = small, 1 = large
  switch(regs.oam_basesize) {
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

void bPPU::render_oam_sprite(void) {
uint16 pos, col, chr, tiledata_inc;
uint8 d0, d1, d2, d3, pal_index;
int x, y, z, x1, mx, mask, p;
int tile_width;
int vline_pos = clock->vcounter();
int snes_width = (regs.bg_mode == 5 || regs.bg_mode == 6)?512:256;
  if(regs.bg_enabled[OAM] == false && regs.bgsub_enabled[OAM] == false)return;

  tile_width = current_sprite.width >> SH_8; //e.x. 16x16 sprite = 2x2 tiles

  if(clock->interlace() == true && (regs.bg_mode == 5 || regs.bg_mode == 6)) {
    y = (vline_pos << SH_2) + clock->interlace_field();
  } else {
    y = vline_pos;
  }

  x = current_sprite.x;
  if(snes_width == 512) {
    x <<= SH_2;
  }

  if(current_sprite.v_flip) {
    y = ((current_sprite.height - 1) - (vline_pos - current_sprite.y));
  } else {
    y = (vline_pos - current_sprite.y);
  }
  y &= 255;
  if(regs.oam_halve == true) {
    y <<= 1;
    y += clock->interlace_field();
  }

  chr = current_sprite.character;
  tiledata_inc = (chr & 0x100)?(regs.oam_nameselect << 13):0;
  chr += (y >> SH_8) << SH_16;
  pal_index = (current_sprite.palette << SH_16);
  for(x1=0;x1<tile_width;x1++) {
    if(current_sprite.h_flip)mx = (tile_width - 1) - x1;
    else                     mx = x1;
    pos = regs.oam_tdaddr + ((chr + mx) << SH_32) + ((y & 7) << SH_2) + tiledata_inc;
    d0 = vram[pos     ];
    d1 = vram[pos +  1];
    d2 = vram[pos + 16];
    d3 = vram[pos + 17];
    for(z=0;z<8;z++) {
      if(current_sprite.h_flip) {
        mask = 0x01 << z;
      } else {
        mask = 0x80 >> z;
      }
      x &= 511;
      if(x < snes_width) {
        col = 0;
        if(d0 & mask)col += 1;
        if(d1 & mask)col += 2;
        if(d2 & mask)col += 4;
        if(d3 & mask)col += 8;
        if(col) {
          col += pal_index;
          col += 128;
          if(oam_line_pri[x] == OAM_PRI_NONE) {
            oam_line_pal[x] = col;
            oam_line_pri[x] = current_sprite.priority;
          }
          if(snes_width == 512) {
            if(oam_line_pri[x + 1] == OAM_PRI_NONE) {
              oam_line_pal[x + 1] = col;
              oam_line_pri[x + 1] = current_sprite.priority;
            }
          }
        }
      }
      x++;
      if(snes_width == 512) {
        x++;
      }
    }
  }
}

void bPPU::render_line_oam(uint8 layer_pos_pri0, uint8 layer_pos_pri1, uint8 layer_pos_pri2, uint8 layer_pos_pri3) {
int i, s;
int vline_pos = clock->vcounter();
int snes_width = (regs.bg_mode == 5 || regs.bg_mode == 6)?512:256;
uint8 layer_pos;
  if(regs.bg_enabled[OAM] != true && regs.bgsub_enabled[OAM] != true)return;

  memset(oam_line_pri, OAM_PRI_NONE, 512);
  for(s=0;s<128;s++) {
    set_sprite_attributes(s);
    if(regs.oam_halve == false) {
      if(vline_pos >= current_sprite.y && vline_pos < (current_sprite.y + current_sprite.height)) {
        render_oam_sprite();
      } else if((current_sprite.y + current_sprite.height) >= 256 && vline_pos < ((current_sprite.y + current_sprite.height) & 255)) {
        render_oam_sprite();
      }
    } else {
      if(vline_pos >= current_sprite.y && vline_pos < (current_sprite.y + (current_sprite.height >> 1))) {
        render_oam_sprite();
      } else if((current_sprite.y + current_sprite.height) >= 256 && vline_pos < ((current_sprite.y + (current_sprite.height >> 1)) & 255)) {
        render_oam_sprite();
      }
    }
  }

  for(i=0;i<snes_width;i++) {
    if(oam_line_pri[i] != OAM_PRI_NONE) {
      switch(oam_line_pri[i]) {
      case 0:layer_pos = layer_pos_pri0;break;
      case 1:layer_pos = layer_pos_pri1;break;
      case 2:layer_pos = layer_pos_pri2;break;
      case 3:layer_pos = layer_pos_pri3;break;
      }
      set_layer_pixel(i, oam_line_pal[i]);
    }
  }
}

void bPPU::render_line_bg(uint8 layer_pos_pri0, uint8 layer_pos_pri1, uint8 color_depth, uint8 bg) {
int x, y, z, x1, y1;
int mirror_x, mirror_y, p;
int screen_x, screen_y;
int bg_x, bg_y;
int xpos, ypos, mosaic_x, mosaic_y;
int snes_width = (regs.bg_mode == 5 || regs.bg_mode == 6)?512:256;
int vline_pos = clock->vcounter();
uint16 t, base_xpos, base_pos, pos, ppos = 0;
uint16 col;
uint8  *src, *bg_td, *bg_td_state, *tile_ptr;
uint8  tiledata_size;
uint8  tile_size, tile_width, tile_height, tile_x;
uint8  mask, pal_index, pal_size;
uint16 tile_num, screen_width, screen_height, screen_width_mask, screen_height_mask, map_index;
uint16 *mtable;
uint8  layer_pos;
uint16 opt_valid_bit, voffset, hoffset, vscroll, hscroll;
  if(regs.bg_enabled[bg] == false && regs.bgsub_enabled[bg] == false)return;

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
  bg_td       = (byte*)bg_tiledata[color_depth];
  bg_td_state = (byte*)bg_tiledata_state[color_depth];

  screen_width  = snes_width;
  screen_height = snes_width; //this is correct -- ppu tilemap is based around 256x256, etc.
  tile_size     = (regs.bg_tilesize[bg])?SH_16:SH_8;
  tile_width    = tile_size;
  tile_height   = tile_size;

  if(clock->interlace() == true && (regs.bg_mode == 5 || regs.bg_mode == 6)) {
    screen_y = (vline_pos << SH_2) + clock->interlace_field();
  } else {
    screen_y = vline_pos;
  }

//Modes 5 and 6 seem to force 16-width tiles due to having twice the resolution.
//The tile size attribute in $2105 has no effect on tile width.
  if(regs.bg_mode == 5 || regs.bg_mode == 6) {
    tile_width = SH_16;
  }

  if(tile_size == SH_16) {
    screen_width  <<= SH_2;
    screen_height <<= SH_2;
  }

  if(regs.bg_scsize[bg] & 0x01)screen_width  <<= SH_2;
  if(regs.bg_scsize[bg] & 0x02)screen_height <<= SH_2;

  screen_width_mask  = screen_width  - 1;
  screen_height_mask = screen_height - 1;

  if(snes_width == 512) {
    hscroll = (regs.bg_hofs[bg] << SH_2) & screen_width_mask;
  } else {
    hscroll = regs.bg_hofs[bg] & screen_width_mask;
  }
  bg_x = hscroll;

  if(snes_width == 512 && clock->interlace() == true) {
    vscroll = (regs.bg_vofs[bg] << SH_2) & screen_height_mask;
  } else {
    vscroll = regs.bg_vofs[bg] & screen_height_mask;
  }
  bg_y = (screen_y + vscroll) & screen_height_mask;

  if(regs.mosaic_enabled[bg] == true) {
    mtable = (uint16*)mosaic_table[regs.mosaic_size];
  } else {
    mtable = (uint16*)mosaic_table[0];
  }
  mosaic_x = mtable[bg_x];
  mosaic_y = mtable[bg_y];

  for(screen_x=0;screen_x<snes_width;screen_x++) {
    if(regs.bg_mode == 2 || regs.bg_mode == 4 || regs.bg_mode == 6) {
      if(regs.bg_mode == 6) {
        tile_x = (mtable[screen_x + (hscroll & 15)] >> SH_16);
      } else {
        tile_x = (mtable[screen_x + (hscroll &  7)] >>  SH_8);
      }
      hoffset = hscroll;
      voffset = vscroll;
      if(tile_x != 0) {
        tile_x = (tile_x - 1) & 31;
        if(regs.bg_mode == 4) {
          pos = regs.bg_scaddr[BG3] + (tile_x << SH_2);
          t   = *((uint16*)vram + (pos >> SH_2));
          if(t & opt_valid_bit) {
            if(!(t & 0x8000)) {
              hoffset = ((t & 0x1ff8) | (hscroll & 7)) & screen_width_mask;
            } else {
              voffset = (t & 0x1fff) & screen_height_mask;
            }
          }
        } else {
          pos = regs.bg_scaddr[BG3] + (tile_x << SH_2);
          t   = *((uint16*)vram + (pos >> SH_2));
          if(t & opt_valid_bit) {
            hoffset = ((t & 0x1ff8) | (hscroll & 7)) & screen_width_mask;
          }
          pos = regs.bg_scaddr[BG3] + 64 + (tile_x << SH_2);
          t   = *((uint16*)vram + (pos >> SH_2));
          if(t & opt_valid_bit) {
            voffset = (t & 0x1fff) & screen_height_mask;
          }
        }
      }
      mosaic_x = mtable[(screen_x + hoffset) & screen_width_mask ];
      mosaic_y = mtable[(screen_y + voffset) & screen_height_mask];
    }

    switch(regs.bg_scsize[bg]) {
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
    pos       = regs.bg_scaddr[bg] + map_index + (base_pos << SH_2);
    t         = *((uint16*)vram + (pos >> SH_2));
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
    tile_num += (regs.bg_tdaddr[bg] >> tiledata_size);

    if(bg_td_state[tile_num] == 1) {
      render_bg_tile(color_depth, bg, tile_num);
    }

    pal_index = ((t >> 10) & 7) * pal_size;

    if(mirror_y) { ypos = (7 - (mosaic_y & 7)); }
    else         { ypos = (    (mosaic_y & 7)); }

//loop while we are rendering from the same tile, as there's no need to do all of the above work
//unless we have rendered all of the visible tile, taking mosaic into account.
    tile_ptr = (uint8*)bg_td + (tile_num << SH_64) + (ypos << SH_8);
    while(1) {
      if(mirror_x) { xpos = (7 - (mosaic_x & 7)); }
      else         { xpos = (    (mosaic_x & 7)); }
      col = *(tile_ptr + xpos);
      if(col) {
        set_layer_pixel(screen_x, col + pal_index);
      }

      bg_x++;
      bg_x &= screen_width_mask;
      mosaic_x = mtable[bg_x];

      if(base_xpos != ((mosaic_x >> SH_8) & 31))break;
      screen_x++;
      if(screen_x >= snes_width)break;
    }
  }
}
