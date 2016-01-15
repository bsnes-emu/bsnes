inline auto PPU::get_palette(uint8 index) -> uint16 {
  const uint addr = index << 1;
  return cgram[addr] + (cgram[addr + 1] << 8);
}

//p = 00000bgr <palette data>
//t = BBGGGRRR <tilemap data>
//r = 0BBb00GGGg0RRRr0 <return data>
inline auto PPU::get_direct_color(uint8 p, uint8 t) -> uint16 {
  return ((t & 7) << 2) | ((p & 1) << 1) |
    (((t >> 3) & 7) << 7) | (((p >> 1) & 1) << 6) |
    ((t >> 6) << 13) | ((p >> 2) << 12);
}

inline auto PPU::get_pixel_normal(uint32 x) -> uint16 {
  pixel_t& p = pixel_cache[x];
  uint16 src_main, src_sub;
  uint8 bg_sub;

  src_main = p.src_main;

  if(!regs.addsub_mode) {
    bg_sub  = BACK;
    src_sub = regs.color_rgb;
  } else {
    bg_sub  = p.bg_sub;
    src_sub = p.src_sub;
  }

  if(!window[COL].main[x]) {
    if(!window[COL].sub[x]) {
      return 0x0000;
    }
    src_main = 0x0000;
  }

  if(!p.ce_main && regs.color_enabled[p.bg_main] && window[COL].sub[x]) {
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

inline auto PPU::get_pixel_swap(uint32 x) -> uint16 {
  pixel_t& p = pixel_cache[x];
  uint16 src_main, src_sub;
  uint8 bg_sub;

  src_main = p.src_sub;

  if(!regs.addsub_mode) {
    bg_sub  = BACK;
    src_sub = regs.color_rgb;
  } else {
    bg_sub  = p.bg_main;
    src_sub = p.src_main;
  }

  if(!window[COL].main[x]) {
    if(!window[COL].sub[x]) {
      return 0x0000;
    }
    src_main = 0x0000;
  }

  if(!p.ce_sub && regs.color_enabled[p.bg_sub] && window[COL].sub[x]) {
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

inline auto PPU::render_line_output() -> void {
  auto ptr = (uint32*)output + (line * 1024) + ((interlace() && field()) ? 512 : 0);

  if(!regs.pseudo_hires && regs.bg_mode != 5 && regs.bg_mode != 6) {
    for(uint x = 0; x < 256; x++) {
      uint color = (regs.display_brightness << 15) | get_pixel_normal(x);
      *ptr++ = color;
      *ptr++ = color;
    }
  } else {
    for(uint x = 0; x < 256; x++) {
      *ptr++ = (regs.display_brightness << 15) | get_pixel_swap(x);
      *ptr++ = (regs.display_brightness << 15) | get_pixel_normal(x);
    }
  }
}

inline auto PPU::render_line_clear() -> void {
  auto ptr = (uint32*)output + (line * 1024) + ((interlace() && field()) ? 512 : 0);
  memory::fill(ptr, 512 * sizeof(uint32));
}
