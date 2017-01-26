GameBoyInterface::GameBoyInterface() {
  information.manufacturer = "Nintendo";
  information.name         = "Game Boy";
  information.overscan     = false;

  media.append({ID::GameBoy, "Game Boy", "gb"});
}

auto GameBoyInterface::videoColors() -> uint32 {
  return 1 << 2;
}

auto GameBoyInterface::videoColor(uint32 color) -> uint64 {
  if(!settings.colorEmulation) {
    uint64 L = image::normalize(3 - color, 2, 16);
    return L << 32 | L << 16 | L << 0;
  } else {
    #define DMG_PALETTE_GREEN
  //#define DMG_PALETTE_YELLOW
  //#define DMG_PALETTE_WHITE

    const uint16 monochrome[4][3] = {
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
      #elif defined(DMG_PALETTE_WHITE)
      {0xffff, 0xffff, 0xffff},
      {0xaaaa, 0xaaaa, 0xaaaa},
      {0x5555, 0x5555, 0x5555},
      {0x0000, 0x0000, 0x0000},
      #endif
    };

    uint64 R = monochrome[color][0];
    uint64 G = monochrome[color][1];
    uint64 B = monochrome[color][2];

    return R << 32 | G << 16 | B << 0;
  }
}

auto GameBoyInterface::load(uint id) -> bool {
  if(id == ID::GameBoy) return system.load(this, System::Model::GameBoy);
  return false;
}
