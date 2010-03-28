#include "scale2x.hpp"

void Scale2xFilter::size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height) {
  if(height > 240) return filter_direct.size(outwidth, outheight, width, height);
  outwidth  = (width  <= 256) ? width  * 2 : width;
  outheight = (height <= 240) ? height * 2 : height;
}

void Scale2xFilter::render(
  uint32_t *output, unsigned outpitch, const uint16_t *input, unsigned pitch,
  const unsigned *line, unsigned width, unsigned height
) {
  if(height > 240) {
    filter_direct.render(output, outpitch, input, pitch, line, width, height);
    return;
  }

  pitch >>= 1;
  outpitch >>= 2;

  uint32_t *out0 = output;
  uint32_t *out1 = output + outpitch;

  for(unsigned y = 0; y < height; y++) {
    unsigned linewidth = line[y];

    if(linewidth == 256) {
      int prevline = (y == 0) || (linewidth != line[y - 1]) ? 0 : pitch;
      int nextline = (y == height - 1) || (linewidth != line[y + 1]) ? 0 : pitch;

      for(unsigned x = 0; x < 256; x++) {
        uint16_t A = *(input - prevline);
        uint16_t B = (x >   0) ? *(input - 1) : *input;
        uint16_t C = *input;
        uint16_t D = (x < 255) ? *(input + 1) : *input;
        uint16_t E = *(input++ + nextline);
        uint32_t c = colortable[C];

        if(A != E && B != D) {
          *out0++ = (A == B ? colortable[A] : c);
          *out0++ = (A == D ? colortable[A] : c);
          *out1++ = (E == B ? colortable[E] : c);
          *out1++ = (E == D ? colortable[E] : c);
        } else {
          *out0++ = c;
          *out0++ = c;
          *out1++ = c;
          *out1++ = c;
        }
      }
    } else {
      for(unsigned x = 0; x < 512; x++) {
        *out0++ = *out1++ = colortable[*input++];
      }
    }

    input += pitch - linewidth;
    out0 += outpitch + outpitch - 512;
    out1 += outpitch + outpitch - 512;
  }
}
