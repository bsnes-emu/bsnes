#include <gb/gb.hpp>

#define VIDEO_CPP
namespace GameBoy {

Video video;

void Video::generate_palette(bool color_emulation) {
  this->color_emulation = color_emulation;
  if(system.dmg()) for(unsigned n = 0; n < 4; n++) palette[n] = palette_dmg(n);
  if(system.sgb()) for(unsigned n = 0; n < 4; n++) palette[n] = palette_sgb(n);
  if(system.cgb()) for(unsigned n = 0; n < (1 << 15); n++) palette[n] = palette_cgb(n);
}

Video::Video() {
  palette = new unsigned[1 << 15]();
}

Video::~Video() {
  delete[] palette;
}

unsigned Video::palette_dmg(unsigned color) const {
  if(color_emulation == false) {
    unsigned L = (3 - color) * 21845;
    return interface->videoColor(color, L, L, L);
  }

  unsigned R = monochrome[color][0];
  unsigned G = monochrome[color][1];
  unsigned B = monochrome[color][2];

  return interface->videoColor(color, R, G, B);
}

unsigned Video::palette_sgb(unsigned color) const {
  unsigned R = (3 - color) * 21845;
  unsigned G = (3 - color) * 21845;
  unsigned B = (3 - color) * 21845;

  return interface->videoColor(color, R, G, B);
}

unsigned Video::palette_cgb(unsigned color) const {
  unsigned r = (color >>  0) & 31;
  unsigned g = (color >>  5) & 31;
  unsigned b = (color >> 10) & 31;

  if(color_emulation == false) {
    unsigned R = (r << 11) | (r << 6) | (r << 1) | (r >> 4);
    unsigned G = (g << 11) | (g << 6) | (g << 1) | (g >> 4);
    unsigned B = (b << 11) | (b << 6) | (b << 1) | (b >> 4);
    return interface->videoColor(color, R, G, B);
  }

  unsigned R = (r * 26 + g *  4 + b *  2);
  unsigned G = (         g * 24 + b *  8);
  unsigned B = (r *  6 + g *  4 + b * 22);

  R = min(960, R);
  G = min(960, G);
  B = min(960, B);

  R = R << 6 | R >> 4;
  G = G << 6 | G >> 4;
  B = B << 6 | B >> 4;

  return interface->videoColor(color, R, G, B);
}

#define DMG_PALETTE_GREEN
//#define DMG_PALETTE_YELLOW
//#define DMG_PALETTE_WHITE

const uint16 Video::monochrome[4][3] = {
  #if defined(DMG_PALETTE_GREEN)
  {0x9a9a, 0xbbbb, 0x0505},
  {0x7878, 0x8484, 0x0505},
  {0x1d1d, 0x5555, 0x1d1d},
  {0x0505, 0x2525, 0x0505},
  #elif defined(DMG_PALETTE_YELLOW)
  {0xffff, 0xf7f7, 0x7b7b},
  {0xb5b5, 0xaeae, 0x4a4a},
  {0x6b6b, 0x6969, 0x3131},
  {0x2121, 0x2020, 0x1010},
  #else  //DMG_PALETTE_WHITE
  {0xffff, 0xffff, 0xffff},
  {0xaaaa, 0xaaaa, 0xaaaa},
  {0x5555, 0x5555, 0x5555},
  {0x0000, 0x0000, 0x0000},
  #endif
};

}
