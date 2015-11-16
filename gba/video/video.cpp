#include <gba/gba.hpp>

namespace GameBoyAdvance {

Video video;

Video::Video() {
  palette = new uint32[1 << 15]();
}

Video::~Video() {
  delete[] palette;
}

auto Video::generatePalette(Emulator::Interface::PaletteMode mode) -> void {
  for(auto color : range(1 << 15)) {
    if(mode == Emulator::Interface::PaletteMode::Literal) {
      palette[color] = color;
      continue;
    }

    uint B = (color >> 10) & 31;
    uint G = (color >>  5) & 31;
    uint R = (color >>  0) & 31;

    if(mode == Emulator::Interface::PaletteMode::Channel) {
      R = image::normalize(R, 5, 16);
      G = image::normalize(G, 5, 16);
      B = image::normalize(B, 5, 16);
      palette[color] = interface->videoColor(color, 0, R, G, B);
      continue;
    }

    if(mode == Emulator::Interface::PaletteMode::Standard) {
      R = image::normalize(R, 5, 16);
      G = image::normalize(G, 5, 16);
      B = image::normalize(B, 5, 16);
      palette[color] = interface->videoColor(color, 0, R, G, B);
      continue;
    }

    if(mode == Emulator::Interface::PaletteMode::Emulation) {
      double lcdGamma = 4.0, outGamma = 2.2;
      double lb = pow(B / 31.0, lcdGamma);
      double lg = pow(G / 31.0, lcdGamma);
      double lr = pow(R / 31.0, lcdGamma);
      B = pow((220 * lb +  10 * lg +  50 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
      G = pow(( 30 * lb + 230 * lg +  10 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
      R = pow((  0 * lb +  50 * lg + 255 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);

      palette[color] = interface->videoColor(color, 0, R, G, B);
      continue;
    }

    palette[color] = 0;
  }
}

const uint8 Video::curve[32] = {
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0e, 0x10, 0x12,
  0x14, 0x16, 0x18, 0x1c, 0x20, 0x28, 0x38, 0x38,
  0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x80,
  0x88, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0,
};

}
