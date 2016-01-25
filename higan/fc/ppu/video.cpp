#include <cmath>

Video video;

Video::Video() {
  output = new uint32[256 * 480];
  paletteLiteral = new uint32[1 << 9];
  paletteStandard = new uint32[1 << 9];
  paletteEmulation = new uint32[1 << 9];
}

auto Video::reset() -> void {
  memory::fill(output(), 256 * 480);

  for(auto color : range(1 << 9)) {
    paletteLiteral[color] = color;
    paletteStandard[color] = generateColor(color, 2.0, 0.0, 1.0, 1.0, 2.2);
    paletteEmulation[color] = generateColor(color, 2.0, 0.0, 1.0, 1.0, 1.8);
  }
}

auto Video::refresh() -> void {
  auto output = this->output();
  auto& palette = settings.colorEmulation ? paletteEmulation : paletteStandard;

  if(settings.scanlineEmulation) {
    for(uint y = 0; y < 240; y++) {
      auto source = ppu.buffer + y * 256;
      auto targetLo = output + y * 512;
      auto targetHi = output + y * 512 + 256;
      for(uint x = 0; x < 256; x++) {
        auto color = palette[*source++];
        *targetLo++ = color;
        *targetHi++ = (255 << 24) | ((color & 0xfefefe) >> 1);
      }
    }
    interface->videoRefresh(output, 256 * sizeof(uint32), 256, 480);
  } else {
    for(uint y = 0; y < 240; y++) {
      auto source = ppu.buffer + y * 256;
      auto target = output + y * 256;
      for(uint x = 0; x < 256; x++) {
        *target++ = palette[*source++];
      }
    }
    interface->videoRefresh(output, 256 * sizeof(uint32), 256, 240);
  }
}

auto Video::generateColor(
  uint n, double saturation, double hue,
  double contrast, double brightness, double gamma
) -> uint32 {
  int color = (n & 0x0f), level = color < 0xe ? (n >> 4) & 3 : 1;

  static const double black = 0.518, white = 1.962, attenuation = 0.746;
  static const double levels[8] = {
    0.350, 0.518, 0.962, 1.550,
    1.094, 1.506, 1.962, 1.962,
  };

  double lo_and_hi[2] = {
    levels[level + 4 * (color == 0x0)],
    levels[level + 4 * (color <  0xd)],
  };

  double y = 0.0, i = 0.0, q = 0.0;
  auto wave = [](int p, int color) { return (color + p + 8) % 12 < 6; };
  for(int p : range(12)) {
    double spot = lo_and_hi[wave(p, color)];

    if(((n & 0x040) && wave(p, 12))
    || ((n & 0x080) && wave(p,  4))
    || ((n & 0x100) && wave(p,  8))
    ) spot *= attenuation;

    double v = (spot - black) / (white - black);

    v = (v - 0.5) * contrast + 0.5;
    v *= brightness / 12.0;

    y += v;
    i += v * std::cos((3.141592653 / 6.0) * (p + hue));
    q += v * std::sin((3.141592653 / 6.0) * (p + hue));
  }

  i *= saturation;
  q *= saturation;

  auto gammaAdjust = [=](double f) { return f < 0.0 ? 0.0 : std::pow(f, 2.2 / gamma); };
  uint r = uclamp<16>(65535.0 * gammaAdjust(y +  0.946882 * i +  0.623557 * q));
  uint g = uclamp<16>(65535.0 * gammaAdjust(y + -0.274788 * i + -0.635691 * q));
  uint b = uclamp<16>(65535.0 * gammaAdjust(y + -1.108545 * i +  1.709007 * q));

  return interface->videoColor(r, g, b);
}
