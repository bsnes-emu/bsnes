#include <emulator/emulator.hpp>

namespace Emulator {

auto Interface::videoColor(uint16 r, uint16 g, uint16 b) -> uint32 {
  double saturation = 1.0;
  double gamma      = 1.0;
  double luminance  = 1.0;

  if(saturation != 1.0) {
    uint16 grayscale = uclamp<16>((r + g + b) / 3);
    double inverse = max(0.0, 1.0 - saturation);
    r = uclamp<16>(r * saturation + grayscale * inverse);
    g = uclamp<16>(g * saturation + grayscale * inverse);
    b = uclamp<16>(b * saturation + grayscale * inverse);
  }

  if(gamma != 1.0) {
    double reciprocal = 1.0 / 32767.0;
    r = r > 32767 ? r : 32767 * pow(r * reciprocal, gamma);
    g = g > 32767 ? g : 32767 * pow(g * reciprocal, gamma);
    b = b > 32767 ? b : 32767 * pow(b * reciprocal, gamma);
  }

  if(luminance != 1.0) {
    r = uclamp<16>(r * luminance);
    g = uclamp<16>(g * luminance);
    b = uclamp<16>(b * luminance);
  }

  return 255 << 24 | (r >> 8) << 16 | (g >> 8) << 8 | (b >> 8) << 0;
}

}
