#include <nall/platform.hpp>
#include <nall/stdint.hpp>
using namespace nall;

extern "C" {
  void filter_size(unsigned&, unsigned&);
  void filter_render(uint32_t*, unsigned, const uint32_t*, unsigned, unsigned, unsigned);
};

dllexport void filter_size(unsigned &width, unsigned &height) {
  width  *= 2;
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

    int prevline = (y == 0 ? 0 : inputPitch);
    int nextline = (y == height - 1 ? 0 : inputPitch);

    for(unsigned x = 0; x < width; x++) {
      uint32_t A = *(in - prevline);
      uint32_t B = (x > 0) ? *(in - 1) : *in;
      uint32_t C = *in;
      uint32_t D = (x < width - 1) ? *(in + 1) : *in;
      uint32_t E = *(in++ + nextline);

      if(A != E && B != D) {
        *out0++ = (A == B ? C + A - ((C ^ A) & 0x010101) >> 1 : C);
        *out0++ = (A == D ? C + A - ((C ^ A) & 0x010101) >> 1 : C);
        *out1++ = (E == B ? C + E - ((C ^ E) & 0x010101) >> 1 : C);
        *out1++ = (E == D ? C + E - ((C ^ E) & 0x010101) >> 1 : C);
      } else {
        *out0++ = C;
        *out0++ = C;
        *out1++ = C;
        *out1++ = C;
      }
    }
  }
}
