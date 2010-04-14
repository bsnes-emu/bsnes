#include "lq2x.hpp"

void LQ2xFilter::size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height) {
  if(width > 256 || height > 240) return filter_direct.size(outwidth, outheight, width, height);
  outwidth  = width  * 2;
  outheight = height * 2;
}

void LQ2xFilter::render(
  uint32_t *output, unsigned outpitch,
  const uint16_t *input, unsigned pitch, unsigned width, unsigned height
) {
  if(width > 256 || height > 240) {
    filter_direct.render(output, outpitch, input, pitch, width, height);
    return;
  }

  pitch >>= 1;
  outpitch >>= 2;

  uint32_t *out0 = output;
  uint32_t *out1 = output + outpitch;

  for(unsigned y = 0; y < height; y++) {
    int prevline = (y == 0 ? 0 : pitch);
    int nextline = (y == height - 1 ? 0 : pitch);

    for(unsigned x = 0; x < width; x++) {
      uint16_t A = *(input - prevline);
      uint16_t B = (x >   0) ? *(input - 1) : *input;
      uint16_t C = *input;
      uint16_t D = (x < 255) ? *(input + 1) : *input;
      uint16_t E = *(input++ + nextline);
      uint32_t c = colortable[C];

      if(A != E && B != D) {
        *out0++ = (A == B ? colortable[C + A - ((C ^ A) & 0x0421) >> 1] : c);
        *out0++ = (A == D ? colortable[C + A - ((C ^ A) & 0x0421) >> 1] : c);
        *out1++ = (E == B ? colortable[C + E - ((C ^ E) & 0x0421) >> 1] : c);
        *out1++ = (E == D ? colortable[C + E - ((C ^ E) & 0x0421) >> 1] : c);
      } else {
        *out0++ = c;
        *out0++ = c;
        *out1++ = c;
        *out1++ = c;
      }
    }

    input += pitch - width;
    out0 += outpitch + outpitch - 512;
    out1 += outpitch + outpitch - 512;
  }
}
