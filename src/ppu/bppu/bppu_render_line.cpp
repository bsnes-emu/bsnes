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

  if(!window[COL].main[x]) {
    if(!window[COL].sub[x]) {
      return 0x0000;
    }
    src_main = 0x0000;
  }

  if(!p->ce_main && regs.color_enabled[p->bg_main] && window[COL].sub[x]) {
  bool halve = false;
    if(regs.color_halve && window[COL].main[x]) {
      if(regs.addsub_mode && bg_sub == BACK);
      else {
        halve = true;
      }
    }
    return addsub(src_main, src_sub, halve);
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

  if(!window[COL].main[x]) {
    if(!window[COL].sub[x]) {
      return 0x0000;
    }
    src_main = 0x0000;
  }

  if(!p->ce_sub && regs.color_enabled[p->bg_sub] && window[COL].sub[x]) {
  bool halve = false;
    if(regs.color_halve && window[COL].main[x]) {
      if(regs.addsub_mode && bg_sub == BACK);
      else {
        halve = true;
      }
    }
    return addsub(src_main, src_sub, halve);
  }

  return src_main;
}

inline void bPPU::render_line_output() {
uint16 *ptr  = (uint16*)output + (line.y * 1024) +
               ((line.interlace && line.interlace_field) ? 512 : 0);
uint16 *luma = light_table[regs.display_brightness];
  if(!regs.pseudo_hires && regs.bg_mode != 5 && regs.bg_mode != 6) {
    for(int x = 0; x < 256; x++) {
      *ptr++ = luma[get_pixel_normal(x)];
    }
  } else {
  uint32 curr, prev = 0;
    for(int x = 0; x < 256; x++) {
      curr = luma[get_pixel_swap(x)];
      *ptr++ = (prev + curr - ((prev ^ curr) & 0x0421)) >> 1;
      prev = curr;

      curr = luma[get_pixel_normal(x)];
      *ptr++ = (prev + curr - ((prev ^ curr) & 0x0421)) >> 1;
      prev = curr;
    }
  }
}
