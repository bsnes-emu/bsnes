#include <nall/platform.hpp>
#include <nall/stdint.hpp>
using namespace nall;

extern "C" {
  void filter_size(unsigned&, unsigned&);
  void filter_render(uint16_t*, unsigned, const uint16_t*, unsigned, unsigned, unsigned);
};

dllexport void filter_size(unsigned &width, unsigned &height) {
  width  *= 2;
  height *= 2;
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
    uint16_t *out0 = output + y * outputPitch * 2;
    uint16_t *out1 = output + y * outputPitch * 2 + outputPitch;

    int prevline = (y == 0 ? 0 : inputPitch);
    int nextline = (y == height - 1 ? 0 : inputPitch);

    for(unsigned x = 0; x < width; x++) {
      uint16_t A = *(input - prevline);
      uint16_t B = (x > 0) ? *(input - 1) : *input;
      uint16_t C = *input;
      uint16_t D = (x < width - 1) ? *(input + 1) : *input;
      uint16_t E = *(input++ + nextline);

      if(A != E && B != D) {
        *out0++ = (A == B ? C + A - ((C ^ A) & 0x0421) >> 1 : C);
        *out0++ = (A == D ? C + A - ((C ^ A) & 0x0421) >> 1 : C);
        *out1++ = (E == B ? C + E - ((C ^ E) & 0x0421) >> 1 : C);
        *out1++ = (E == D ? C + E - ((C ^ E) & 0x0421) >> 1 : C);
      } else {
        *out0++ = C;
        *out0++ = C;
        *out1++ = C;
        *out1++ = C;
      }
    }
  }
}
