inline uint16 bPPU::get_palette(uint8 index) {
  return read16(cgram, index << 1);
}

inline uint16 bPPU::get_direct_color(uint8 p, uint8 t) {
//p = 00000bgr <palette data>
//t = BBGGGRRR <tilemap data>
//r = 0BBb00GGGg0RRRr0 <return data>
  return ((t & 7) << 2) | ((p & 1) << 1) |
    (((t >> 3) & 7) << 7) | (((p >> 1) & 1) << 6) |
    ((t >> 6) << 13) | ((p >> 2) << 12);
}

inline uint16 bPPU::get_pixel_normal(uint32 x) {
_pixel *p = &pixel_cache[x];
uint16 src_main, src_sub;
uint8  bg_sub;
  src_main = p->src_main;

  if(!regs.addsub_mode) {
    bg_sub  = BACK;
    src_sub = regs.color_rgb;
  } else {
    bg_sub  = p->bg_sub;
    src_sub = p->src_sub;
  }

  if(!window_cache[COL].main[x]) {
    if(!window_cache[COL].sub[x]) {
      return 0x0000;
    }
    src_main = 0x0000;
  }

  if(!p->ce_main && regs.color_enabled[p->bg_main] && window_cache[COL].sub[x]) {
  bool halve = false;
    if(regs.color_halve && window_cache[COL].main[x]) {
      if(regs.addsub_mode && bg_sub == BACK);
      else {
        halve = true;
      }
    }
    return addsub_pixels(x, src_main, src_sub, halve);
  }

  return src_main;
}

inline uint16 bPPU::get_pixel_swap(uint32 x) {
_pixel *p = &pixel_cache[x];
uint16 src_main, src_sub;
uint8  bg_sub;
  src_main = p->src_sub;

  if(!regs.addsub_mode) {
    bg_sub  = BACK;
    src_sub = regs.color_rgb;
  } else {
    bg_sub  = p->bg_main;
    src_sub = p->src_main;
  }

  if(!window_cache[COL].main[x]) {
    if(!window_cache[COL].sub[x]) {
      return 0x0000;
    }
    src_main = 0x0000;
  }

  if(!p->ce_sub && regs.color_enabled[p->bg_sub] && window_cache[COL].sub[x]) {
  bool halve = false;
    if(regs.color_halve && window_cache[COL].main[x]) {
      if(regs.addsub_mode && bg_sub == BACK);
      else {
        halve = true;
      }
    }
    return addsub_pixels(x, src_main, src_sub, halve);
  }

  return src_main;
}

inline uint16 bPPU::get_pixel_lores(uint32 x) {
  return get_pixel_normal(x);
}

inline uint16 bPPU::get_pixel_hires(uint32 x) {
  if(x & 1) {
    return get_pixel_normal(x >> 1);
  } else {
    return get_pixel_swap(x >> 1);
  }
}

inline void bPPU::render_line_output() {
uint32 r, x;
uint16 *ptr    = (uint16*)output + (line.y * 1024) +
                 ((line.interlace && line.interlace_field) ? 512 : 0);
uint16 *ltable = (uint16*)light_table + (regs.display_brightness << 15);

  if(!regs.pseudo_hires && !regs.hires) {
    for(x=0;x<256;x++) {
      r = get_pixel_lores(x);
      *ptr++ = *(ltable + r);
    }
  } else {
    for(x=0;x<512;x++) {
      r = get_pixel_hires(x);
      *ptr++ = *(ltable + r);
    }
    if(regs.pseudo_hires && !regs.hires) {
      ptr -= 512;
      for(x=0;x<256;x++) {
        *(ptr + x) = ((ptr[x << 1] & 0x7bde) >> 1) + ((ptr[(x << 1) + 1] & 0x7bde) >> 1);
      }
    }
  }
}
