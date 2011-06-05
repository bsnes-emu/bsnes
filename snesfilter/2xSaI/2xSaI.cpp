#include <nall/platform.hpp>
#include <nall/stdint.hpp>
using namespace nall;

#include "implementation.cpp"

extern "C" {
  void filter_size(unsigned&, unsigned&);
  void filter_render(uint32_t*, uint32_t*, unsigned, const uint16_t*, unsigned, unsigned, unsigned);
};

uint32_t temp[512 * 480];

dllexport void filter_size(unsigned &width, unsigned &height) {
  width  *= 2;
  height *= 2;
}

dllexport void filter_render(
  uint32_t *colortable, uint32_t *output, unsigned outpitch,
  const uint16_t *input, unsigned pitch, unsigned width, unsigned height
) {
  for(unsigned y = 0; y < height; y++) {
    const uint16_t *line_in = (const uint16_t*)(((const uint8_t*)input) + pitch * y);
    uint32_t *line_out = temp + y * width;
    for(unsigned x = 0; x < width; x++) {
      line_out[x] = colortable[line_in[x]];
    }
  }

  _2xSaI32((unsigned char*)temp, width * sizeof(uint32_t), 0, (unsigned char*)output, outpitch, width, height);
}
