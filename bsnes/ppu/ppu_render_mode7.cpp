/*
  The algorithm in this file was derived from the snes9x source code.
  The snes9x source code is not public domain. If you wish to use this
  code, you must abide by the terms of the snes9x license. If you do not
  wish to abide by the snes9x licensing terms, please define the precompiler
  variable PUBLIC_DOMAIN so that ppu_render_mode7f.cpp is used instead of
  this file. You must also remove this file from any work that you release
  that does not follow the snes9x license.
  See license.txt for more info on the license of this software and snes9x.
*/

#define CLIP_10BIT_SIGNED(x) \
  ((x) & ((1 << 10) - 1)) + (((((x) & (1 << 13)) ^ (1 << 13)) - (1 << 13)) >> 3)

#define CAST_WORDTOINT(x) \
  (int)(((x & 0x8000)?(x | 0xffff0000):(x & 0x00007fff)))

void ppu_render_line_m7(byte layer_pos_bg1, byte layer_pos_bg2_pri0, byte layer_pos_bg2_pri1) {
int x;
int step_m7a, step_m7c, m7a, m7b, m7c, m7d;
int hoffset, voffset;
int centerx, centery;
int xx, yy;
int px, py;
int tx, ty, tile, palette, priority;
byte layer_pos;
  hoffset = (CAST_WORDTOINT(ppu.m7hofs) << 7) >> 7;
  voffset = (CAST_WORDTOINT(ppu.m7vofs) << 7) >> 7;

  centerx = (CAST_WORDTOINT(ppu.m7x) << 7) >> 7;
  centery = (CAST_WORDTOINT(ppu.m7y) << 7) >> 7;

  if(ppu.mode7_vflip == true) {
    yy = 223 - ppu.vline_pos;
  } else {
    yy = ppu.vline_pos;
  }
  yy += CLIP_10BIT_SIGNED(voffset - centery);

  m7b = CAST_WORDTOINT(ppu.m7b) * yy + (centerx << 8);
  m7d = CAST_WORDTOINT(ppu.m7d) * yy + (centery << 8);

  step_m7a = CAST_WORDTOINT(ppu.m7a);
  step_m7c = CAST_WORDTOINT(ppu.m7c);

  xx = CLIP_10BIT_SIGNED(hoffset - centerx);

  m7a = CAST_WORDTOINT(ppu.m7a) * xx;
  m7c = CAST_WORDTOINT(ppu.m7c) * xx;

  for(x=0;x<256;x++) {
    px = ((m7a + m7b) >> 8);
    py = ((m7c + m7d) >> 8);

    switch(ppu.mode7_repeat) {
    case 0: //screen repitition outside of screen area
    case 1: //same as case 0
      px &= 1023;
      py &= 1023;
      tx = ((px >> SH_8) & 127);
      ty = ((py >> SH_8) & 127);
      tile    = ppu.vram[(ty * 128 + tx) << 1];
      palette = ppu.vram[(((tile << SH_64) + ((py & 7) << SH_8) + (px & 7)) << 1) + 1];
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
      tile    = ppu.vram[(ty * 128 + tx) << 1];
      palette = ppu.vram[(((tile << SH_64) + ((py & 7) << SH_8) + (px & 7)) << 1) + 1];
      break;
    case 3: //palette color 0 outside of screen area
      if(px < 0 || px > 1023 || py < 0 || py > 1023) {
        palette = 0;
      } else {
        px &= 1023;
        py &= 1023;
        tx = ((px >> SH_8) & 127);
        ty = ((py >> SH_8) & 127);
        tile    = ppu.vram[(ty * 128 + tx) << 1];
        palette = ppu.vram[(((tile << SH_64) + ((py & 7) << SH_8) + (px & 7)) << 1) + 1];
      }
      break;
    }

    if(ppu.mode7_extbg == false) {
      if(palette) {
        layer_pos = layer_pos_bg1;
        if(ppu.mode7_hflip == true) {
          ppu_set_layer_pixel(255 - x, palette);
        } else {
          ppu_set_layer_pixel(x, palette);
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
        if(ppu.mode7_hflip == true) {
          ppu_set_layer_pixel(255 - x, palette);
        } else {
          ppu_set_layer_pixel(x, palette);
        }
      }
    }

    m7a += step_m7a;
    m7c += step_m7c;
  }
}
