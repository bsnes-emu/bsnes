#include <gba/gba.hpp>

namespace GameBoyAdvance {

Video video;

void Video::generate_palette(bool color_emulation) {
  //todo: implement LCD color emulation
  for(unsigned color = 0; color < (1 << 15); color++) {
    unsigned B = (color >> 10) & 31;
    unsigned G = (color >>  5) & 31;
    unsigned R = (color >>  0) & 31;

    if(color_emulation) {
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

      R = R << 8 | R;
      G = G << 8 | G;
      B = B << 8 | B;
    } else {
      R = R << 11 | R << 6 | R << 1 | R >> 4;
      G = G << 11 | G << 6 | G << 1 | G >> 4;
      B = B << 11 | B << 6 | B << 1 | B >> 4;
    }

    palette[color] = interface->videoColor(color, R, G, B);
  }
}

Video::Video() {
  palette = new uint32[1 << 15]();
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
