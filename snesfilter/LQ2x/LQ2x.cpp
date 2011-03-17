#include <nall/platform.hpp>
#include <nall/stdint.hpp>
using namespace nall;

extern "C" {
  void filter_size(unsigned&, unsigned&);
  void filter_render(uint32_t*, uint32_t*, unsigned, const uint16_t*, unsigned, unsigned, unsigned);
};

dllexport void filter_size(unsigned &width, unsigned &height) {
  width  *= 2;
  height *= 2;
}

dllexport void filter_render(
  uint32_t *colortable, uint32_t *output, unsigned outpitch,
  const uint16_t *input, unsigned pitch, unsigned width, unsigned height
) {
  pitch >>= 1;
  outpitch >>= 2;

  uint32_t *out0 = output;
  uint32_t *out1 = output + outpitch;

  for(unsigned y = 0; y < height; y++) {
    int prevline = (y == 0 ? 0 : pitch);
    int nextline = (y == height - 1 ? 0 : pitch);

    for(unsigned x = 0; x < width; x++) {
      uint16_t A = *(input - prevline);
      uint16_t B = (x > 0) ? *(input - 1) : *input;
      uint16_t C = *input;
      uint16_t D = (x < width - 1) ? *(input + 1) : *input;
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
    out0 += outpitch + outpitch - (width << 1);
    out1 += outpitch + outpitch - (width << 1);
  }
}
