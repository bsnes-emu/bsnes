#define CLIP_10BIT_SIGNED(x) \
  ((x) & ((1 << 10) - 1)) + (((((x) & (1 << 13)) ^ (1 << 13)) - (1 << 13)) >> 3)

#define CAST_WORDTOINT(x) \
  (int)(((x & 0x8000)?(x | 0xffff0000):(x & 0x00007fff)))

void bPPU::render_line_m7(uint8 layer_pos_bg1, uint8 layer_pos_bg2_pri0, uint8 layer_pos_bg2_pri1) {
int x;
int step_m7a, step_m7c, m7a, m7b, m7c, m7d;
int hoffset, voffset;
int centerx, centery;
int xx, yy;
int px, py;
int tx, ty, tile, palette, priority;
uint8 layer_pos;
  hoffset = (CAST_WORDTOINT(regs.bg_hofs[BG1]) << 7) >> 7;
  voffset = (CAST_WORDTOINT(regs.bg_vofs[BG1]) << 7) >> 7;

  centerx = (CAST_WORDTOINT(regs.m7x) << 7) >> 7;
  centery = (CAST_WORDTOINT(regs.m7y) << 7) >> 7;

  if(regs.mode7_vflip == true) {
    yy = 223 - clock->vcounter();
  } else {
    yy = clock->vcounter();
  }
  yy += CLIP_10BIT_SIGNED(voffset - centery);

  m7b = CAST_WORDTOINT(regs.m7b) * yy + (centerx << 8);
  m7d = CAST_WORDTOINT(regs.m7d) * yy + (centery << 8);

  step_m7a = CAST_WORDTOINT(regs.m7a);
  step_m7c = CAST_WORDTOINT(regs.m7c);

  xx = CLIP_10BIT_SIGNED(hoffset - centerx);

  m7a = CAST_WORDTOINT(regs.m7a) * xx;
  m7c = CAST_WORDTOINT(regs.m7c) * xx;

  for(x=0;x<256;x++) {
    px = ((m7a + m7b) >> 8);
    py = ((m7c + m7d) >> 8);

    switch(regs.mode7_repeat) {
    case 0: //screen repitition outside of screen area
    case 1: //same as case 0
      px &= 1023;
      py &= 1023;
      tx = ((px >> SH_8) & 127);
      ty = ((py >> SH_8) & 127);
      tile    = vram[(ty * 128 + tx) << 1];
      palette = vram[(((tile << SH_64) + ((py & 7) << SH_8) + (px & 7)) << 1) + 1];
      break;
    case 2: //character 0 repetition outside of screen area
      if(px < 0 || px > 1023 || py < 0 || py > 1023) {
        tx = 0;
        ty = 0;
      } else {
        px &= 1023;
        py &= 1023;
        tx = ((px >> SH_8) & 127);
        ty = ((py >> SH_8) & 127);
      }
      tile    = vram[(ty * 128 + tx) << 1];
      palette = vram[(((tile << SH_64) + ((py & 7) << SH_8) + (px & 7)) << 1) + 1];
      break;
    case 3: //palette color 0 outside of screen area
      if(px < 0 || px > 1023 || py < 0 || py > 1023) {
        palette = 0;
      } else {
        px &= 1023;
        py &= 1023;
        tx = ((px >> SH_8) & 127);
        ty = ((py >> SH_8) & 127);
        tile    = vram[(ty * 128 + tx) << 1];
        palette = vram[(((tile << SH_64) + ((py & 7) << SH_8) + (px & 7)) << 1) + 1];
      }
      break;
    }

    if(regs.mode7_extbg == false) {
      if(palette) {
        layer_pos = layer_pos_bg1;
        if(regs.mode7_hflip == true) {
          set_layer_pixel(255 - x, palette);
        } else {
          set_layer_pixel(x, palette);
        }
      }
    } else {
      priority = (palette >> 7);
      palette &= 0x7f;
      if(palette) {
        if(priority == 0) {
          layer_pos = layer_pos_bg2_pri0;
        } else {
          layer_pos = layer_pos_bg2_pri1;
        }
        if(regs.mode7_hflip == true) {
          set_layer_pixel(255 - x, palette);
        } else {
          set_layer_pixel(x, palette);
        }
      }
    }

    m7a += step_m7a;
    m7c += step_m7c;
  }
}
