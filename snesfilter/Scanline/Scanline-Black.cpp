#include <nall/platform.hpp>
#include <nall/stdint.hpp>
using namespace nall;

extern "C" {
  void filter_size(unsigned&, unsigned&);
  void filter_render(uint32_t*, uint32_t*, unsigned, const uint16_t*, unsigned, unsigned, unsigned);
};

dllexport void filter_size(unsigned &width, unsigned &height) {
  height *= 2;
}

dllexport void filter_render(
  uint32_t *palette, uint32_t *output, unsigned outpitch,
  const uint16_t *input, unsigned pitch, unsigned width, unsigned height
) {
  pitch >>= 1;
  outpitch >>= 2;

  uint32_t *out0 = output;
  uint32_t *out1 = output + outpitch;

  for(unsigned y = 0; y < height; y++) {
    const uint16_t *in = input + y * pitch;
    uint32_t *out0 = output + y * outpitch * 2;
    uint32_t *out1 = output + y * outpitch * 2 + outpitch;

    for(unsigned x = 0; x < width; x++) {
      *out0++ = palette[*in++];
      *out1++ = 0;
    }
  }
}
