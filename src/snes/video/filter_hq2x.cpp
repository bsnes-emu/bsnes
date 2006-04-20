/*****
 * HQ2x Algorithm (C) 2003 Maxim Stepin
 * License: LGPL
 *
 * Optimizations (C) 2006 Shay Green, byuu
 *****/

#define diff(x, y)  ((yuvtable[x] - yuvtable[y] + diff_offset) & diff_mask)
#define hdiff(x, y) ((x - yuvtable[y]) & diff_mask)
#define expand_rgb(n) { n |= n << 16; n &= 0x03e07c1f; }
#define pack_rgb(n)   { n &= 0x03e07c1f; n |= n >> 16; }

static uint16 blend1(uint32 c1, uint32 c2) {
  expand_rgb(c1);
  expand_rgb(c2);
  c1 = (c1 * 3 + c2) >> 2;
  pack_rgb(c1);
  return c1;
}

static uint16 blend2(uint32 c1, uint32 c2, uint32 c3) {
//c1 = (c1 * 2 + c2 + c3) >> 2;
  c2 = (c2 + c3 - ((c2 ^ c3) & 0x0421)) >> 1;
  c1 = (c1 + c2 - ((c1 ^ c2) & 0x0421)) >> 1;
  return c1;
}

static uint16 blend6(uint32 c1, uint32 c2, uint32 c3) {
  expand_rgb(c1);
  expand_rgb(c2);
  expand_rgb(c3);
  c1 = (c1 * 5 + c2 * 2 + c3) >> 3;
  pack_rgb(c1);
  return c1;
}

static uint16 blend7(uint32 c1, uint32 c2, uint32 c3) {
  expand_rgb(c1);
  expand_rgb(c2);
  expand_rgb(c3);
  c1 = (c1 * 6 + c2 + c3) >> 3;
  pack_rgb(c1);
  return c1;
}

static uint16 blend9(uint32 c1, uint32 c2, uint32 c3) {
  expand_rgb(c1);
  expand_rgb(c2);
  expand_rgb(c3);
  c1 = (c1 * 2 + (c2 + c3) * 3) >> 3;
  pack_rgb(c1);
  return c1;
}

static uint16 blend10(uint32 c1, uint32 c2, uint32 c3) {
  expand_rgb(c1);
  expand_rgb(c2);
  expand_rgb(c3);
  c1 = (c1 * 14 + c2 + c3) >> 4;
  pack_rgb(c1);
  return c1;
}

void HQ2xVideoFilter::run(
uint32 *colortbl, uint16 *data, uint32 width, uint32 height, uint32 pitch,
uint16 *output, uint32 max_width, uint32 max_height, uint32 output_pitch,
uint32 req_width, uint32 req_height, uint32 &result_width, uint32 &result_height,
uint16 *scanline_widths
) {
  pitch >>= 1;
  output_pitch >>= 1;

  if(width == 256 && height <= 240) {
    lores_progressive(colortbl, data, height, pitch, output, output_pitch);
    result_width  = 512;
    result_height = height * 2;
    return;
  }

  for(int y = 0; y < height; y++) {
    if(width == 512 && scanline_widths[y] == 256) {
      for(int x = 0; x < 256; x++) {
        *output++ = colortbl[*data];
        *output++ = colortbl[*data++];
      }
      data += 256;
    } else {
      for(int x = 0; x < width; x++) {
        *output++ = colortbl[*data++];
      }
    }
    data   += pitch - width;
    output += output_pitch - width;
  }
  result_width  = width;
  result_height = height;
}

void HQ2xVideoFilter::lores_progressive(
uint32 *colortbl, uint16 *data, uint32 height, uint32 pitch,
uint16 *output, uint32 output_pitch
) {
uint16 *out0 = (uint16*)(output);
uint16 *out1 = (uint16*)(output + output_pitch);
#define W1 data[-1 - (SNES::VIDEOPITCH_LORES >> 1)]
#define W2 data[ 0 - (SNES::VIDEOPITCH_LORES >> 1)]
#define W3 data[+1 - (SNES::VIDEOPITCH_LORES >> 1)]
#define W4 data[-1]
#define W5 data[ 0]
#define W6 data[+1]
#define W7 data[-1 + (SNES::VIDEOPITCH_LORES >> 1)]
#define W8 data[ 0 + (SNES::VIDEOPITCH_LORES >> 1)]
#define W9 data[+1 + (SNES::VIDEOPITCH_LORES >> 1)]

  data += pitch;
  memset(out0, 0, 1024); out0 += output_pitch << 1;
  memset(out1, 0, 1024); out1 += output_pitch << 1;

  for(int y = height - 2; y; --y) {
    data++;
    *(uint32*)out0 = 0; out0 += 2;
    *(uint32*)out1 = 0; out1 += 2;

  int32 pattern = diff(W5, W4) ? 0x10 : 0x00;
    for(int x = 256 - 2; x; --x) {
    uint32 center = yuvtable[W5] + diff_offset;
    //W4 for pixel x+1 is the same as W6 for pixel x
      pattern  = (pattern & 0x10) >> 1;
      pattern |= hdiff(center, W1) ? 0x01 : 0x00;
      pattern |= hdiff(center, W2) ? 0x02 : 0x00;
      pattern |= hdiff(center, W3) ? 0x04 : 0x00;
    //pattern |= hdiff(center, W4) ? 0x08 : 0x00;
      pattern |= hdiff(center, W6) ? 0x10 : 0x00;
      pattern |= hdiff(center, W7) ? 0x20 : 0x00;
      pattern |= hdiff(center, W8) ? 0x40 : 0x00;
      pattern |= hdiff(center, W9) ? 0x80 : 0x00;

      switch(pattern) {
        #include "filter_hq2x_lookuptbl.h"
      }

      data++;
      out0 += 2;
      out1 += 2;
    }

    data++;
    *(uint32*)out0 = 0; out0 += 2;
    *(uint32*)out1 = 0; out1 += 2;

    data += pitch - 256;
    out0 += output_pitch + output_pitch - 512;
    out1 += output_pitch + output_pitch - 512;
  }

  memset(out0, 0, 1024);
  memset(out1, 0, 1024);
}

HQ2xVideoFilter::HQ2xVideoFilter() {
  for(int i = 0; i < 32768; i++) {
  int ir = (i)       & 0x1f;
  int ig = (i >> 5)  & 0x1f;
  int ib = (i >> 10) & 0x1f;

  //bgr555->bgr888
  double r = (ir << 3) | (ir >> 2);
  double g = (ig << 3) | (ig >> 2);
  double b = (ib << 3) | (ib >> 2);

  //bgr888->yuv888
  double y = (r + g + b) * (0.25f * (63.5f / 48.0f));
  double u = ((r - b) * 0.25f + 128.0f) * (7.5f / 7.0f);
  double v = ((g * 2.0f - r - b) * 0.125f + 128.0f) * (7.5f / 6.0f);
    yuvtable[i] = (int(y) << 21) + (int(u) << 11) + (int(v));
  }

  diff_offset = (0x440 << 21) + (0x207 << 11) + 0x407;
  diff_mask   = (0x380 << 21) + (0x1f0 << 11) + 0x3f0;
}
