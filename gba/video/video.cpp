#include <gba/gba.hpp>

namespace GameBoyAdvance {

Video video;

void Video::generate_palette(Emulator::Interface::PaletteMode mode) {
  for(unsigned color = 0; color < (1 << 15); color++) {
    if(mode == Emulator::Interface::PaletteMode::Literal) {
      palette[color] = color;
      continue;
    }

    unsigned B = (color >> 10) & 31;
    unsigned G = (color >>  5) & 31;
    unsigned R = (color >>  0) & 31;

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
#if 0
      R = curve[R];
      G = curve[G];
      B = curve[B];

      unsigned Rr = R * 16;
      unsigned Gr = R *  4;
      unsigned Br = R *  4;

      unsigned Rg = G *  8;
      unsigned Gg = G * 16;
      unsigned Bg = G *  8;

      unsigned Rb = B *  0;  //intentionally always zero
      unsigned Gb = B *  8;
      unsigned Bb = B * 16;

      if(Rr < Rg) std::swap(Rr, Rg);
      if(Rr < Rb) std::swap(Rr, Rb);
      if(Rg < Rb) std::swap(Rg, Rb);

      if(Gr < Gg) std::swap(Gr, Gg);
      if(Gr < Gb) std::swap(Gr, Gb);
      if(Gg < Gb) std::swap(Gg, Gb);

      if(Br < Bg) std::swap(Br, Bg);
      if(Br < Bb) std::swap(Br, Bb);
      if(Bg < Bb) std::swap(Bg, Bb);

      R = (((4 * Rr + 2 * Rg + Rb) * 160) >> 14) + 32;
      G = (((4 * Gr + 2 * Gg + Gb) * 160) >> 14) + 32;
      B = (((4 * Br + 2 * Bg + Bb) * 160) >> 14) + 32;

      R = image::normalize(R, 8, 16);
      G = image::normalize(G, 8, 16);
      B = image::normalize(B, 8, 16);
#else
      double lcdGamma = 4.0, outGamma = 2.2;
      double lb = pow(B / 31.0, lcdGamma);
      double lg = pow(G / 31.0, lcdGamma);
      double lr = pow(R / 31.0, lcdGamma);
      B = pow((220 * lb +  10 * lg +  50 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
      G = pow(( 30 * lb + 230 * lg +  10 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
      R = pow((  0 * lb +  50 * lg + 255 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
#endif

      palette[color] = interface->videoColor(color, 0, R, G, B);
      continue;
    }

    palette[color] = 0;
  }
}

Video::Video() {
  palette = new uint32_t[1 << 15]();
}

Video::~Video() {
  delete[] palette;
}

const uint8 Video::curve[32] = {
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0e, 0x10, 0x12,
  0x14, 0x16, 0x18, 0x1c, 0x20, 0x28, 0x38, 0x38,
  0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x80,
  0x88, 0x90, 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0,
};

}
