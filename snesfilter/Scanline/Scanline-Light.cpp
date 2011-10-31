#include <nall/platform.hpp>
#include <nall/stdint.hpp>
using namespace nall;

extern "C" {
  void filter_size(unsigned&, unsigned&);
  void filter_render(uint32_t*, unsigned, const uint32_t*, unsigned, unsigned, unsigned);
};

dllexport void filter_size(unsigned &width, unsigned &height) {
  height *= 2;
}

dllexport void filter_render(
  uint32_t *output, unsigned outputPitch,
  const uint32_t *input, unsigned inputPitch,
  unsigned width, unsigned height
) {
  outputPitch >>= 2, inputPitch >>= 2;

  #pragma omp parallel for
  for(unsigned y = 0; y < height; y++) {
    const uint32_t *in = input + y * inputPitch;
    uint32_t *out0 = output + y * outputPitch * 2;
    uint32_t *out1 = output + y * outputPitch * 2 + outputPitch;

    for(unsigned x = 0; x < width; x++) {
      *out0++ = *in;
      *out1++ = (*in++ & 0xf8f8f8) >> 1;
    }
  }
}
