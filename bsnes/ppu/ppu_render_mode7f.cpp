/*
  This mode7 code was derived from my own research, assisted by
  information in the public domain. It is therefore redistributed
  to the public domain. This code is infinitesimally less accurate
  than ppu_render_mode7i.cpp, but nearly twice as slow.
  However, this file contains no licensing restrictions.
  Please define the precompiler variable PUBLIC_DOMAIN to use this
  code instead of mode7i.
*/

/*
  *1 - converts 16-bit word value to float within the range -128/127. decimal
       has range of 256 with steps of 0.00390625
  *2 - (int)float will turn anything from -0.999 to 0.999 to 0. this code will
       check to see if the number is negative, and if the decimal place (.xxx)
       is zero or not. If value is positive or the decimal place ends on .000,
       the number is converted directly to an int. otherwise, one is subtracted
       to make up for the rounding error of 1 mentioned above.
       result is masked by 1023 (width / height of map is 1024) to prevent
       negative values from causing an error with the division in the tile
       calculation below.
  *3 - mode7 vram consists of 32k of data. there are two 16k regions interleaved
       in bytes. the low byte is the tile number, and the high byte is tile data.
       the tile data directly indexes into the palette. the map is 128*128, and
       there are 256 possible characters. each character is 64 bytes in length.
*/
void ppu_render_line_m7(void) {
int   x, y, sx, sy;
byte  tile, palette;
float x_stepx, x_stepy, y_stepx, y_stepy;
float centerx, centery, scrollx, scrolly, startx, starty;
  y               = (ppu.mode7_vflip == true)?223 - ppu.vline_pos:ppu.vline_pos;

//*1
  x_stepx = (float)((signed short)ppu.m7a) / 256.0;
  x_stepy = (float)((signed short)ppu.m7b) / 256.0;
  y_stepx = (float)((signed short)ppu.m7c) / 256.0;
  y_stepy = (float)((signed short)ppu.m7d) / 256.0;

  centerx = (float)(1.0 - x_stepx) * (signed short)ppu.m7x - x_stepy * (signed short)ppu.m7y;
  centery = (float)(1.0 - y_stepy) * (signed short)ppu.m7y - y_stepx * (signed short)ppu.m7x;

  scrollx = (float)((signed short)ppu.m7hofs);
  scrolly = (float)((signed short)ppu.m7vofs);

  startx  = centerx + ((scrollx) * x_stepx) + ((scrolly + y) * x_stepy);
  starty  = centery + ((scrollx) * y_stepx) + ((scrolly + y) * y_stepy);

  for(x=0;x<256;x++) {
//*2
    if(startx < 0.0 && (int)(startx * 32768) & 32767) {
      sx = (int)(startx - 1) & 1023;
    } else {
      sx = (int)startx & 1023;
    }
    if(starty < 0.0 && (int)(starty * 32768) & 32767) {
      sy = (int)(starty - 1) & 1023;
    } else {
      sy = (int)starty & 1023;
    }
//*3
    tile    = ppu.vram[(((sy / 8) & 127) * 128 + ((sx / 8) & 127)) << 1];
    palette = ppu.vram[((tile * 64 + (sy & 7) * 8 + (sx & 7)) << 1) + 1];
    if(palette) {
      if(ppu.mode7_hflip == true) {
        ppu_set_pixel(BG1, 255 - x, palette);
      } else {
        ppu_set_pixel(BG1, x, palette);
      }
    }
    startx += x_stepx;
    starty += y_stepx;
  }
}
