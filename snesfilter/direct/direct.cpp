#include "direct.hpp"

void DirectFilter::size(unsigned &outwidth, unsigned &outheight, unsigned width, unsigned height) {
  outwidth  = width;
  outheight = height;
}

void DirectFilter::render(
  uint32_t *output, unsigned outpitch, const uint16_t *input, unsigned pitch,
  unsigned width, unsigned height
) {
  pitch >>= 1;
  outpitch >>= 2;

  for(unsigned y = 0; y < height; y++) {
    for(unsigned x = 0; x < width; x++) {
      uint16_t p = *input++;
      *output++ = colortable[p];
    }
    input += pitch - width;
    output += outpitch - width;
  }
}
