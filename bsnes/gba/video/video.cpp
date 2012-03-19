#include <gba/gba.hpp>

namespace GBA {

Video video;

unsigned Video::color(unsigned color) const {
  uint5 b = color >> 10;
  uint5 g = color >>  5;
  uint5 r = color >>  0;

  uint10 R = (r << 5) | (r << 0);
  uint10 G = (g << 5) | (g << 0);
  uint10 B = (b << 5) | (b << 0);

  return (R << 20) | (G << 10) | (B << 0);
}

void Video::generate(Format format) {
  for(unsigned n = 0; n < (1 << 15); n++) palette[n] = color(n);

  if(format == Format::RGB24) {
    for(unsigned n = 0; n < (1 << 15); n++) {
      unsigned color = palette[n];
      palette[n] = ((color >> 6) & 0xff0000) + ((color >> 4) & 0x00ff00) + ((color >> 2) & 0x0000ff);
    }
  }

  if(format == Format::RGB16) {
    for(unsigned n = 0; n < (1 << 15); n++) {
      unsigned color = palette[n];
      palette[n] = ((color >> 14) & 0xf800) + ((color >> 9) & 0x07e0) + ((color >> 5) & 0x001f);
    }
  }

  if(format == Format::RGB15) {
    for(unsigned n = 0; n < (1 << 15); n++) {
      unsigned color = palette[n];
      palette[n] = ((color >> 15) & 0x7c00) + ((color >> 10) & 0x03e0) + ((color >> 5) & 0x001f);
    }
  }
}

Video::Video() {
  palette = new uint32[1 << 15]();
}

Video::~Video() {
  delete[] palette;
}

}
