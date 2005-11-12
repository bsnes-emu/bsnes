/*
  bsnes mode7 renderer

  base algorithm written by anomie
  bsnes implementation written by byuu

  supports mode 7 + extbg + rotate + zoom + direct color + scrolling + m7sel + windowing + mosaic
  does not support pseudo-hires
  interlace support is automatic via main rendering routine
*/

//13-bit sign extend
//--s---vvvvvvvvvv -> ssssssvvvvvvvvvv
#define CLIP(x) ( ((x) & 0x2000) ? ( (x) | ~0x03ff) : ((x) & 0x03ff) )
//#define CLIP(x) ( ((x) & 0x03ff) | (((x) & 0x2000) ? -0x0400 : 0) )

void bPPU::render_line_mode7(uint8 bg, uint8 pri0_pos, uint8 pri1_pos) {
  if(regs.bg_enabled[bg] == false && regs.bgsub_enabled[bg] == false)return;

int32 x, y;
int32 a, b, c, d, cx, cy;
int32 hofs, vofs;

int32 px, py;
int32 tx, ty, tile, palette;
  a = int32(int16(regs.m7a));
  b = int32(int16(regs.m7b));
  c = int32(int16(regs.m7c));
  d = int32(int16(regs.m7d));

  cx   = (int32(regs.m7x)         << 19) >> 19;
  cy   = (int32(regs.m7y)         << 19) >> 19;
  hofs = (int32(regs.m7_hofs)     << 19) >> 19;
//+1 breaks FF5 title screen mirror alignment...
  vofs = (int32(regs.m7_vofs + 0) << 19) >> 19;

int  _pri, _x;
bool _bg_enabled    = regs.bg_enabled[bg];
bool _bgsub_enabled = regs.bgsub_enabled[bg];

  build_window_tables(bg);
uint8 *wt_main = window_cache[bg].main;
uint8 *wt_sub  = window_cache[bg].sub;

  if(regs.mode7_vflip == true) {
    y = 255 - line.y;
  } else {
    y = line.y;
  }

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

int32 psx = ((a * CLIP(hofs - cx)) & ~63) + ((b * CLIP(vofs - cy)) & ~63) + ((b * mtable_y[y]) & ~63) + (cx << 8);
int32 psy = ((c * CLIP(hofs - cx)) & ~63) + ((d * CLIP(vofs - cy)) & ~63) + ((d * mtable_y[y]) & ~63) + (cy << 8);
  for(x=0;x<256;x++) {
    px = psx + (a * mtable_x[x]);
    py = psy + (c * mtable_x[x]);

  //mask floating-point bits (low 8 bits)
    px >>= 8;
    py >>= 8;

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
        tile = 0;
      } else {
        px &= 1023;
        py &= 1023;
        tx = ((px >> 3) & 127);
        ty = ((py >> 3) & 127);
        tile = vram[(ty * 128 + tx) << 1];
      }
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

  uint32 col;
    if(regs.direct_color == true && bg == BG1) {
    //direct color mode does not apply to bg2, as it is only 128 colors...
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

#undef CLIP
