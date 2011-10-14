#include <nall/platform.hpp>
#include <nall/stdint.hpp>
using namespace nall;

extern "C" {
  void filter_size(unsigned&, unsigned&);
  void filter_render(uint16_t*, unsigned, const uint16_t*, unsigned, unsigned, unsigned);
};

dllexport void filter_size(unsigned &width, unsigned &height) {
  width  *= 3;
  height *= 3;
}

dllexport void filter_render(
  uint16_t *output, unsigned outputPitch,
  const uint16_t *input, unsigned inputPitch,
  unsigned width, unsigned height
) {
  outputPitch >>= 1, inputPitch >>= 1;

  #pragma omp parallel for
  for(unsigned y = 0; y < height; y++) {
    const uint16_t *in = input + y * inputPitch;
    uint16_t *out0 = output + y * outputPitch * 3;
    uint16_t *out1 = output + y * outputPitch * 3 + outputPitch;
    uint16_t *out2 = output + y * outputPitch * 3 + outputPitch + outputPitch;

    for(unsigned x = 0; x < width; x++) {
      uint16_t full = *in++, half = (full >> 1) & 0x3def;

      *out0++ = (full & 0x7c00);
      *out1++ = (full & 0x7c00);
      *out2++ = (half & 0x7c00);

      *out0++ = (full & 0x03e0);
      *out1++ = (full & 0x03e0);
      *out2++ = (half & 0x03e0);

      *out0++ = (full & 0x001f);
      *out1++ = (full & 0x001f);
      *out2++ = (half & 0x001f);
    }
  }
}
