#include <gb/gb.hpp>

namespace GameBoy {

Video video;

Video::Video() {
  output = new uint32[160 * 144];
  paletteStandard = new uint32[1 << 15];
  paletteEmulation = new uint32[1 << 15];
}

Video::~Video() {
  delete[] output;
  delete[] paletteStandard;
  delete[] paletteEmulation;
}

auto Video::power() -> void {
  memory::fill(output, 160 * 144 * sizeof(uint32));

  if(system.dmg()) {
    for(auto color : range(1 << 2)) {
      uint L = image::normalize(3 - color, 2, 8);
      uint R = monochrome[color][0] >> 8;
      uint G = monochrome[color][1] >> 8;
      uint B = monochrome[color][2] >> 8;
      paletteStandard[color] = (255 << 24) | (L << 16) | (L << 8) | (L << 0);
      paletteEmulation[color] = (255 << 24) | (R << 16) | (G << 8) | (B << 0);
    }
  }

  if(system.sgb()) {
    for(auto color : range(1 << 2)) {
      paletteStandard[color] = color;
      paletteEmulation[color] = color;
    }
  }

  if(system.cgb()) {
    for(auto color : range(1 << 15)) {
      uint r = (uint5)(color >>  0);
      uint g = (uint5)(color >>  5);
      uint b = (uint5)(color >> 10);

      { uint R = image::normalize(r, 5, 8);
        uint G = image::normalize(g, 5, 8);
        uint B = image::normalize(b, 5, 8);
        paletteStandard[color] = (255 << 24) | (R << 16) | (G << 8) | (B << 0);
      }

      { uint R = (r * 26 + g *  4 + b *  2);
        uint G = (         g * 24 + b *  8);
        uint B = (r *  6 + g *  4 + b * 22);
        R = min(960, R) >> 2;
        G = min(960, G) >> 2;
        B = min(960, B) >> 2;
        paletteEmulation[color] = (255 << 24) | (R << 16) | (G << 8) | (B << 0);
      }
    }
  }
}

auto Video::refresh() -> void {
  auto palette = settings.colorEmulation ? paletteEmulation : paletteStandard;

  for(uint y = 0; y < 144; y++) {
    auto source = ppu.screen + y * 160;
    auto target = output + y * 160;

    if(settings.blurEmulation) {
      for(uint x = 0; x < 160; x++) {
        auto a = palette[*source++];
        auto b = *target;
        *target++ = (a + b - ((a ^ b) & 0x01010101)) >> 1;
      }
    } else {
      for(uint x = 0; x < 160; x++) {
        auto color = palette[*source++];
        *target++ = color;
      }
    }
  }

  interface->videoRefresh(output, 4 * 160, 160, 144);
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
