#include <nall/platform.hpp>
#include <nall/stdint.hpp>
using namespace nall;

extern "C" {
  void filter_size(unsigned&, unsigned&);
  void filter_render(uint16_t*, unsigned, const uint16_t*, unsigned, unsigned, unsigned);
};

dllexport void filter_size(unsigned &width, unsigned &height) {
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
    uint16_t *out = output + y * outputPitch;
    for(unsigned x = 0; x < width; x++) {
      uint16_t pixel = *in++;
      if(x < 8 || x >= width - 8 || y < 8 || y >= height - 8) pixel = 0;
      *out++ = pixel;
    }
  }
}
