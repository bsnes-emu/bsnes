inline uint16 bPPU::get_palette(int index) {
  return *((uint16*)cgram + index);
}

inline uint16 bPPU::get_pixel(int x) {
_pixel *p = &pixel_cache[x];
uint16 _r;
  if(p->bg_main && p->bg_sub) {
    if(regs.bg_color_enabled[p->bg_main & 0x7f] && sub_colorwindowtable[x]) {
      if(regs.addsub_mode) {
        _r = addsub_pixels(p->src_main, p->bg_main & 0x7f, p->src_sub, p->bg_sub & 0x7f);
      } else {
        _r = addsub_pixel(p->src_main, p->bg_main & 0x7f);
      }
    } else {
      _r = get_palette(p->src_main);
    }
  } else if(p->bg_main) {
    if(regs.bg_color_enabled[p->bg_main & 0x7f] && sub_colorwindowtable[x]) {
      _r = addsub_pixel(p->src_main, p->bg_main & 0x7f);
    } else {
      _r = get_palette(p->src_main);
    }
  } else if(p->bg_sub) {
    if(regs.bg_color_enabled[BACK]) {
      if(sub_colorwindowtable[x]) {
        if(regs.addsub_mode) {
          _r = addsub_pixels(0, BACK, p->src_sub, p->bg_sub & 0x7f);
        } else {
          _r = addsub_pixel(0, BACK);
        }
      } else {
        _r = get_palette(0);
      }
    } else {
      _r = 0x0000;
    }
  } else {
    if(main_colorwindowtable[x]) {
      if(regs.bg_color_enabled[BACK] && sub_colorwindowtable[x]) {
        _r = addsub_pixel(0, BACK);
      } else {
        _r = get_palette(0);
      }
    } else {
      _r = 0x0000;
    }
  }
  return _r;
}

inline void bPPU::render_line_output() {
int x;
uint16 _r;
uint16 *ptr;
  ptr = (uint16*)output->buffer + (_y << 10);
  if(_interlace == true) {
    ptr += _interlace_field << 9;
  }

uint16 *ltable;
  ltable = (uint16*)light_table + (regs.display_brightness << 15);

  if(_screen_width == 256) {
    for(x=0;x<256;x++) {
      _r = get_pixel(x);
      *ptr = *(ltable + _r);
      ptr += 2;
    }
  } else {
    for(x=0;x<512;x++) {
      _r = get_pixel(x);
      *ptr++ = *(ltable + _r);
    }
  }
}
