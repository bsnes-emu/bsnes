#define CLIP_10BIT_SIGNED(x) \
  ((x) & ((1 << 10) - 1)) + (((((x) & (1 << 13)) ^ (1 << 13)) - (1 << 13)) >> 3)

#define CAST_WORDTOINT(x) \
  (int32)(((x & 0x8000) ? (x | 0xffff0000) : (x & 0x00007fff)))

void bPPU::render_line_mode7(uint8 bg1_pri, uint8 bg2b_pri, uint8 bg2a_pri) {
int32 x;
int32 step_m7a, step_m7c, m7a, m7b, m7c, m7d;
int32 hoffset, voffset;
int32 centerx, centery;
int32 xx, yy;
int32 px, py;
int32 tx, ty, tile, palette;
uint8 layer_pos;
  hoffset = ((int32)regs.m7_hofs << 19) >> 19;
  voffset = ((int32)regs.m7_vofs << 19) >> 19;

  centerx = ((int32)regs.m7x << 19) >> 19;
  centery = ((int32)regs.m7y << 19) >> 19;

  if(regs.mode7_vflip == true) {
    yy = 255 - _y;
  } else {
    yy = _y;
  }
  yy += CLIP_10BIT_SIGNED(voffset - centery);

  m7b = CAST_WORDTOINT(regs.m7b) * yy + (centerx << 8);
  m7d = CAST_WORDTOINT(regs.m7d) * yy + (centery << 8);

  step_m7a = CAST_WORDTOINT(regs.m7a);
  step_m7c = CAST_WORDTOINT(regs.m7c);

  xx = CLIP_10BIT_SIGNED(hoffset - centerx);

  m7a = CAST_WORDTOINT(regs.m7a) * xx;
  m7c = CAST_WORDTOINT(regs.m7c) * xx;

int _pri, _x, _bg;
bool _bg_enabled, _bgsub_enabled;
  if(regs.mode7_extbg == false) {
    _pri           = bg1_pri;
    _bg            = BG1;
    _bg_enabled    = regs.bg_enabled[BG1];
    _bgsub_enabled = regs.bgsub_enabled[BG1];
  } else {
    _bg            = BG2;
    _bg_enabled    = regs.bg_enabled[BG2];
    _bgsub_enabled = regs.bgsub_enabled[BG2];
  }

uint8 *wt_main = main_windowtable[_bg];
uint8 *wt_sub  = sub_windowtable[_bg];
  build_window_tables(_bg);

  for(x=0;x<256;x++) {
    px = ((m7a + m7b) >> 8);
    py = ((m7c + m7d) >> 8);

    switch(regs.mode7_repeat) {
    case 0: //screen repitition outside of screen area
    case 1: //same as case 0
      px &= 1023;
      py &= 1023;
      tx = ((px >> 3) & 127);
      ty = ((py >> 3) & 127);
      tile    = vram[(ty * 128 + tx) << 1];
      palette = vram[(((tile << 6) + ((py & 7) << 3) + (px & 7)) << 1) + 1];
      break;
    case 2: //character 0 repetition outside of screen area
      if(px < 0 || px > 1023 || py < 0 || py > 1023) {
        tx = 0;
        ty = 0;
      } else {
        px &= 1023;
        py &= 1023;
        tx = ((px >> 3) & 127);
        ty = ((py >> 3) & 127);
      }
      tile    = vram[(ty * 128 + tx) << 1];
      palette = vram[(((tile << 6) + ((py & 7) << 3) + (px & 7)) << 1) + 1];
      break;
    case 3: //palette color 0 outside of screen area
      if(px < 0 || px > 1023 || py < 0 || py > 1023) {
        palette = 0;
      } else {
        px &= 1023;
        py &= 1023;
        tx = ((px >> 3) & 127);
        ty = ((py >> 3) & 127);
        tile    = vram[(ty * 128 + tx) << 1];
        palette = vram[(((tile << 6) + ((py & 7) << 3) + (px & 7)) << 1) + 1];
      }
      break;
    }

    if(!palette)goto _end_setpixel;

    if(regs.mode7_extbg == false) {
    //_pri set at top of function, as it is static
      if(regs.mode7_hflip == true) {
        _x = 255 - x;
      } else {
        _x = x;
      }
    } else {
      _pri = (palette >> 7) ? bg2a_pri : bg2b_pri;
      palette &= 0x7f;
      if(regs.mode7_hflip == true) {
        _x = 255 - x;
      } else {
        _x = x;
      }
    }

    if(main_colorwindowtable[_x]) {
      if(_bg_enabled == true && !wt_main[_x]) {
        if(pixel_cache[_x].pri_main < _pri) {
          pixel_cache[_x].pri_main = _pri;
          pixel_cache[_x].bg_main  = 0x80 | _bg;
          pixel_cache[_x].src_main = palette;
        }
      }
      if(_bgsub_enabled == true && !wt_sub[_x]) {
        if(pixel_cache[_x].pri_sub < _pri) {
          pixel_cache[_x].pri_sub = _pri;
          pixel_cache[_x].bg_sub  = 0x80 | _bg;
          pixel_cache[_x].src_sub = palette;
        }
      }
    }

_end_setpixel:
    m7a += step_m7a;
    m7c += step_m7c;
  }
}
