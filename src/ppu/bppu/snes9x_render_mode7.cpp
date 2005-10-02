#define CLIP_10BIT_SIGNED(x) \
  ((x) & ((1 << 10) - 1)) + (((((x) & (1 << 13)) ^ (1 << 13)) - (1 << 13)) >> 3)

#define CAST_WORDTOINT(x) \
  (int32)((((x) & 0x8000) ? ((x) | 0xffff0000) : ((x) & 0x00007fff)))

void bPPU::render_line_mode7(uint8 bg, uint8 pri0_pos, uint8 pri1_pos) {
int32 x;
int32 step_m7a, step_m7c, m7a, m7b, m7c, m7d;
int32 hoffset, voffset;
int32 centerx, centery;
int32 xx, yy;
int32 px, py;
int32 tx, ty, tile, palette;
uint8 layer_pos;
  if(regs.bg_enabled[bg] == false && regs.bgsub_enabled[bg] == false)return;

uint16 *mtable_x, *mtable_y;
  if(bg == BG1) {
    mtable_x = (uint16*)mosaic_table[(regs.mosaic_enabled[BG1] == true) ? regs.mosaic_size : 0];
    mtable_y = (uint16*)mosaic_table[(regs.mosaic_enabled[BG1] == true) ? regs.mosaic_size : 0];
  } else { //bg == BG2
  //Mode7 EXTBG BG2 uses BG1 mosaic enable to control vertical mosaic,
  //and BG2 mosaic enable to control horizontal mosaic...
    mtable_x = (uint16*)mosaic_table[(regs.mosaic_enabled[BG2] == true) ? regs.mosaic_size : 0];
    mtable_y = (uint16*)mosaic_table[(regs.mosaic_enabled[BG1] == true) ? regs.mosaic_size : 0];
  }

  hoffset = ((int32)regs.m7_hofs << 19) >> 19;
  voffset = ((int32)regs.m7_vofs << 19) >> 19;

  centerx = ((int32)regs.m7x << 19) >> 19;
  centery = ((int32)regs.m7y << 19) >> 19;

  if(regs.mode7_vflip == true) {
    yy = 255 - _y;
  } else {
    yy = _y;
  }
  yy  = mtable_y[yy];
  yy += CLIP_10BIT_SIGNED(voffset - centery);

  m7b = CAST_WORDTOINT(regs.m7b) * yy + (centerx << 8);
  m7d = CAST_WORDTOINT(regs.m7d) * yy + (centery << 8);

  step_m7a = CAST_WORDTOINT(regs.m7a);
  step_m7c = CAST_WORDTOINT(regs.m7c);

  xx = CLIP_10BIT_SIGNED(hoffset - centerx);

  m7a = CAST_WORDTOINT(regs.m7a) * xx;
  m7c = CAST_WORDTOINT(regs.m7c) * xx;

int _pri, _x;
bool _bg_enabled, _bgsub_enabled;
  _bg_enabled    = regs.bg_enabled[bg];
  _bgsub_enabled = regs.bgsub_enabled[bg];

  build_window_tables(bg);
uint8 *wt_main = main_windowtable[bg];
uint8 *wt_sub  = sub_windowtable[bg];

  for(x=0;x<256;x++) {
    px = ((m7a + step_m7a * mtable_x[x] + m7b) >> 8);
    py = ((m7c + step_m7c * mtable_x[x] + m7d) >> 8);

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
    case 2: //palette color 0 outside of screen area
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
    case 3: //character 0 repetition outside of screen area
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
    }

    if(bg == BG1) {
      _pri = pri0_pos;
    } else {
      _pri = (palette >> 7) ? pri1_pos : pri0_pos;
      palette &= 0x7f;
    }

    if(!palette)continue;

    if(regs.mode7_hflip == true) {
      _x = 255 - x;
    } else {
      _x = x;
    }

    if(main_colorwindowtable[_x]) {
    uint32 col;
      if(regs.direct_color == true && bg == BG1) {
      //direct color mode does not apply to bg2 as it is only 128 colors...
        col = get_direct_color(0, palette);
      } else {
        col = get_palette(palette);
      }

      if(regs.bg_enabled[bg] == true && !wt_main[_x]) {
        if(pixel_cache[_x].pri_main < _pri) {
          pixel_cache[_x].pri_main = _pri;
          pixel_cache[_x].bg_main  = 0x80 | bg;
          pixel_cache[_x].src_main = col;
          pixel_cache[_x].color_exempt = false;
        }
      }
      if(regs.bgsub_enabled[bg] == true && !wt_sub[_x]) {
        if(pixel_cache[_x].pri_sub < _pri) {
          pixel_cache[_x].pri_sub = _pri;
          pixel_cache[_x].bg_sub  = 0x80 | bg;
          pixel_cache[_x].src_sub = col;
        }
      }
    }
  }
}

#undef CLIP_10BIT_SIGNED
#undef CAST_WORDTOINT
