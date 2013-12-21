#include <gb/gb.hpp>

#define VIDEO_CPP
namespace GameBoy {

Video video;

void Video::generate_palette(Emulator::Interface::PaletteMode mode) {
  this->mode = mode;
  if(system.dmg()) for(unsigned n = 0; n < 4; n++) palette[n] = palette_dmg(n);
  if(system.sgb()) for(unsigned n = 0; n < 4; n++) palette[n] = palette_sgb(n);
  if(system.cgb()) for(unsigned n = 0; n < (1 << 15); n++) palette[n] = palette_cgb(n);
}

Video::Video() {
  palette = new uint32_t[1 << 15]();
}

Video::~Video() {
  delete[] palette;
}

unsigned Video::palette_dmg(unsigned color) const {
  if(mode == Emulator::Interface::PaletteMode::Literal) {
    return color;
  }

  if(mode == Emulator::Interface::PaletteMode::Channel) {
    unsigned L = image::normalize(color, 2, 16);
    return interface->videoColor(color, 0, 0, 0, L);
  }

  if(mode == Emulator::Interface::PaletteMode::Standard) {
    unsigned L = image::normalize(3 - color, 2, 16);
    return interface->videoColor(color, 0, L, L, L);
  }

  if(mode == Emulator::Interface::PaletteMode::Emulation) {
    unsigned R = monochrome[color][0];
    unsigned G = monochrome[color][1];
    unsigned B = monochrome[color][2];
    return interface->videoColor(color, 0, R, G, B);
  }

  return 0;
}

unsigned Video::palette_sgb(unsigned color) const {
  return color;
}

unsigned Video::palette_cgb(unsigned color) const {
  if(mode == Emulator::Interface::PaletteMode::Literal) {
    return color;
  }

  unsigned r = (color >>  0) & 31;
  unsigned g = (color >>  5) & 31;
  unsigned b = (color >> 10) & 31;

  if(mode == Emulator::Interface::PaletteMode::Channel) {
    r = image::normalize(r, 5, 16);
    g = image::normalize(g, 5, 16);
    b = image::normalize(b, 5, 16);
    return interface->videoColor(color, 0, r, g, b);
  }

  if(mode == Emulator::Interface::PaletteMode::Standard) {
    r = image::normalize(r, 5, 16);
    g = image::normalize(g, 5, 16);
    b = image::normalize(b, 5, 16);
    return interface->videoColor(color, 0, r, g, b);
  }

  if(mode == Emulator::Interface::PaletteMode::Emulation) {
    unsigned R = (r * 26 + g *  4 + b *  2);
    unsigned G = (         g * 24 + b *  8);
    unsigned B = (r *  6 + g *  4 + b * 22);

    R = min(960, R);
    G = min(960, G);
    B = min(960, B);

    R = R << 6 | R >> 4;
    G = G << 6 | G >> 4;
    B = B << 6 | B >> 4;

    return interface->videoColor(color, 0, R, G, B);
  }

  return 0;
}

#define DMG_PALETTE_GREEN
//#define DMG_PALETTE_YELLOW
//#define DMG_PALETTE_WHITE

const uint16 Video::monochrome[4][3] = {
  #if defined(DMG_PALETTE_GREEN)
  {0xaeae, 0xd9d9, 0x2727},
  {0x5858, 0xa0a0, 0x2828},
  {0x2020, 0x6262, 0x2929},
  {0x1a1a, 0x4545, 0x2a2a},
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
