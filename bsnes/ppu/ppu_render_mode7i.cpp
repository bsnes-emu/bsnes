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

void ppu_render_line_m7(void) {
int x;
int step_m7a, step_m7c, m7a, m7b, m7c, m7d;
int hoffset, voffset;
int centerx, centery;
int xx, yy;
int px, py;
int tile, palette;
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
    px = ((m7a + m7b) >> 8) & 1023;
    py = ((m7c + m7d) >> 8) & 1023;

    tile    = ppu.vram[(((py / 8) & 127) * 128 + ((px / 8) & 127)) << 1];
    palette = ppu.vram[((tile * 64 + (py & 7) * 8 + (px & 7)) << 1) + 1];

    if(ppu.mode7_hflip == true) {
      ppu_set_pixel(BG1, 255 - x, palette);
    } else {
      ppu_set_pixel(BG1, x, palette);
    }

    m7a += step_m7a;
    m7c += step_m7c;
  }
}
