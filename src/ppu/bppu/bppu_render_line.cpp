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

inline uint16 bPPU::get_pixel(uint32 x) {
_pixel *p = &pixel_cache[x];
  if(!p->bg_main) {
    p->bg_main      = BACK;
    p->src_main     = get_palette(0);
    p->color_exempt = false;
  } else {
    p->bg_main &= 0x7f;
  }

  if(!p->bg_sub) {
    p->bg_sub  = BACK;
    p->src_sub = regs.color_rgb;
  } else {
    p->bg_sub &= 0x7f;
  }

  if(!regs.addsub_mode) {
    p->bg_sub  = BACK;
    p->src_sub = regs.color_rgb;
  }

  if(!window_cache[COL].main[x]) {
    if(!window_cache[COL].sub[x]) {
      return 0x0000;
    }
  //p->bg_main remains the same, even when the main color window
  //masks out the color. this is needed for regs.color_enabled[p->bg_main]
  //test below. illusion of gaia relies on this behavior for its load menu.
    p->src_main = 0x0000;
  }

  if(!p->color_exempt && regs.color_enabled[p->bg_main] && window_cache[COL].sub[x]) {
    return addsub_pixels(x);
  }
  return p->src_main;
}

inline void bPPU::render_line_output() {
uint16 r, x;
uint16 *ptr    = (uint16*)output + (line.y * 1024) +
                 ((line.interlace && line.interlace_field) ? 512 : 0);
uint16 *ltable = (uint16*)light_table + (regs.display_brightness << 15);

  if(line.width == 256) {
    for(x=0;x<256;x++) {
      r = get_pixel(x);
      *ptr++ = *(ltable + r);
    }
  } else {
    for(x=0;x<512;x++) {
      r = get_pixel(x);
      *ptr++ = *(ltable + r);
    }
  }
}
