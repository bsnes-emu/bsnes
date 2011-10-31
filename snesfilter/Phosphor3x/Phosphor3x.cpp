#include <nall/platform.hpp>
#include <nall/stdint.hpp>
using namespace nall;

extern "C" {
  void filter_size(unsigned&, unsigned&);
  void filter_render(uint32_t*, unsigned, const uint32_t*, unsigned, unsigned, unsigned);
};

dllexport void filter_size(unsigned &width, unsigned &height) {
  width  *= 3;
  height *= 3;
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
    uint32_t *out0 = output + y * outputPitch * 3;
    uint32_t *out1 = output + y * outputPitch * 3 + outputPitch;
    uint32_t *out2 = output + y * outputPitch * 3 + outputPitch + outputPitch;

    for(unsigned x = 0; x < width; x++) {
      uint32_t A = (x == 0 ? 0 : *(in - 1));
      uint32_t B = *in;
      uint32_t C = (x == width - 1 ? 0 : *(in + 1));

      uint8_t Ar = A >> 16, Ag = A >> 8, Ab = A >> 0;
      uint8_t Br = B >> 16, Bg = B >> 8, Bb = B >> 0;
      uint8_t Cr = C >> 16, Cg = C >> 8, Cb = C >> 0;

      A = ((Br >> 0) << 16) + ((Bg >> 1) << 8) + ((Ab >> 1) << 0);
      B = ((Br >> 1) << 16) + ((Bg >> 0) << 8) + ((Bb >> 1) << 0);
      C = ((Cr >> 1) << 16) + ((Bg >> 1) << 8) + ((Bb >> 0) << 0);

      in++;

      *out0++ = A;
      *out1++ = A;
      *out2++ = (A & 0xf8f8f8) >> 1;

      *out0++ = B;
      *out1++ = B;
      *out2++ = (B & 0xf8f8f8) >> 1;

      *out0++ = C;
      *out1++ = C;
      *out2++ = (C & 0xf8f8f8) >> 1;
    }
  }
}
