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
  enum : unsigned { Mask = (1022 << 20) + (1022 << 10) + (1022 << 0) };
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

      unsigned Ar = (A >> 20) & 1023, Ag = (A >> 10) & 1023, Ab = (A >> 0) & 1023;
      unsigned Br = (B >> 20) & 1023, Bg = (B >> 10) & 1023, Bb = (B >> 0) & 1023;
      unsigned Cr = (C >> 20) & 1023, Cg = (C >> 10) & 1023, Cb = (C >> 0) & 1023;

      A = ((Br >> 0) << 20) + ((Bg >> 1) << 10) + ((Ab >> 1) << 0);
      B = ((Br >> 1) << 20) + ((Bg >> 0) << 10) + ((Bb >> 1) << 0);
      C = ((Cr >> 1) << 20) + ((Bg >> 1) << 10) + ((Bb >> 0) << 0);

      in++;

      *out0++ = A;
      *out1++ = A;
      *out2++ = (A & Mask) >> 1;

      *out0++ = B;
      *out1++ = B;
      *out2++ = (B & Mask) >> 1;

      *out0++ = C;
      *out1++ = C;
      *out2++ = (C & Mask) >> 1;
    }
  }
}
