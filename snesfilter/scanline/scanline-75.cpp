#include <nall/platform.hpp>
#include <nall/stdint.hpp>
using namespace nall;

extern "C" {
  void filter_size(unsigned&, unsigned&);
  void filter_render(uint32_t*, uint32_t*, unsigned, const uint16_t*, unsigned, unsigned, unsigned);
};

uint16_t adjust[32768];

void initialize() {
  static bool initialized = false;
  if(initialized == true) return;
  initialized = true;

  for(unsigned i = 0; i < 32768; i++) {
    uint8_t r = (i >> 10) & 31;
    uint8_t g = (i >>  5) & 31;
    uint8_t b = (i >>  0) & 31;
    r *= 0.75;
    g *= 0.75;
    b *= 0.75;
    adjust[i] = (r << 10) + (g << 5) + (b << 0);
  }
}

dllexport void filter_size(unsigned &width, unsigned &height) {
  initialize();
  height *= 2;
}

dllexport void filter_render(
  uint32_t *palette, uint32_t *output, unsigned outpitch,
  const uint16_t *input, unsigned pitch, unsigned width, unsigned height
) {
  initialize();
  pitch >>= 1;
  outpitch >>= 2;

  uint32_t *out0 = output;
  uint32_t *out1 = output + outpitch;

  for(unsigned y = 0; y < height; y++) {
    const uint16_t *in = input + y * pitch;
    uint32_t *out0 = output + y * outpitch * 2;
    uint32_t *out1 = output + y * outpitch * 2 + outpitch;

    for(unsigned x = 0; x < width; x++) {
      uint16_t color = *in++;
      *out0++ = palette[color];
      *out1++ = palette[adjust[color]];
    }
  }
}
