HQ2xFilter filter_hq2x;

#define diff(x, y)  ((yuvtable[x] - yuvtable[y] + diff_offset) & diff_mask)
#define hdiff(x, y) ((x - yuvtable[y]) & diff_mask)
#define expand_rgb(n) { n |= n << 16; n &= 0x03e07c1f; }
#define pack_rgb(n)   { n &= 0x03e07c1f; n |= n >> 16; }

static uint16_t blend1(uint32_t c1, uint32_t c2) {
  expand_rgb(c1);
  expand_rgb(c2);
  c1 = (c1 * 3 + c2) >> 2;
  pack_rgb(c1);
  return c1;
}

static uint16_t blend2(uint32_t c1, uint32_t c2, uint32_t c3) {
//c1 = (c1 * 2 + c2 + c3) >> 2;
  c2 = (c2 + c3 - ((c2 ^ c3) & 0x0421)) >> 1;
  c1 = (c1 + c2 - ((c1 ^ c2) & 0x0421)) >> 1;
  return c1;
}

static uint16_t blend6(uint32_t c1, uint32_t c2, uint32_t c3) {
  expand_rgb(c1);
  expand_rgb(c2);
  expand_rgb(c3);
  c1 = (c1 * 5 + c2 * 2 + c3) >> 3;
  pack_rgb(c1);
  return c1;
}

static uint16_t blend7(uint32_t c1, uint32_t c2, uint32_t c3) {
  expand_rgb(c1);
  expand_rgb(c2);
  expand_rgb(c3);
  c1 = (c1 * 6 + c2 + c3) >> 3;
  pack_rgb(c1);
  return c1;
}

static uint16_t blend9(uint32_t c1, uint32_t c2, uint32_t c3) {
  expand_rgb(c1);
  expand_rgb(c2);
  expand_rgb(c3);
  c1 = (c1 * 2 + (c2 + c3) * 3) >> 3;
  pack_rgb(c1);
  return c1;
}

static uint16_t blend10(uint32_t c1, uint32_t c2, uint32_t c3) {
  expand_rgb(c1);
  expand_rgb(c2);
  expand_rgb(c3);
  c1 = (c1 * 14 + c2 + c3) >> 4;
  pack_rgb(c1);
  return c1;
}

void HQ2xFilter::render(
  uint32_t *output, unsigned outpitch, unsigned &outwidth, unsigned &outheight,
  uint16_t *input, unsigned pitch, unsigned *line, unsigned width, unsigned height
) {
  if(width > 256 || height > 240) {
    filter_direct.render(output, outpitch, outwidth, outheight, input, pitch, line, width, height);
    return;
  }

  pitch >>= 1;
  outpitch >>= 2;

  uint32_t *out0 = output;
  uint32_t *out1 = output + outpitch;

  #define W1 input[-1 - pitch]
  #define W2 input[ 0 - pitch]
  #define W3 input[+1 - pitch]
  #define W4 input[-1]
  #define W5 input[ 0]
  #define W6 input[+1]
  #define W7 input[-1 + pitch]
  #define W8 input[ 0 + pitch]
  #define W9 input[+1 + pitch]

  input += pitch;
  memset(out0, 0, 2048); out0 += outpitch << 1;
  memset(out1, 0, 2048); out1 += outpitch << 1;

  for(int y = height - 2; y; --y) {
    input++;
    *out0++ = 0; *out0++ = 0;
    *out1++ = 0; *out1++ = 0;

    int32_t pattern = diff(W5, W4) ? 0x10 : 0x00;
    for(int x = 256 - 2; x; --x) {
      uint32_t center = yuvtable[W5] + diff_offset;

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
        #include "hq2x_table.h"
      }

      input++;
      out0 += 2;
      out1 += 2;
    }

    input++;
    *out0++ = 0; *out0++ = 0;
    *out1++ = 0; *out1++ = 0;

    input += pitch - 256;
    out0 += outpitch + outpitch - 512;
    out1 += outpitch + outpitch - 512;
  }

  memset(out0, 0, 2048);
  memset(out1, 0, 2048);

  outwidth = width * 2;
  outheight = height * 2;
}

HQ2xFilter::HQ2xFilter() {
  yuvtable = new uint32_t[32768];

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

HQ2xFilter::~HQ2xFilter() {
  delete[] yuvtable;
}
