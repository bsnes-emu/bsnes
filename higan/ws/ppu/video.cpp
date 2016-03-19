Video video;

Video::Video() {
  output = new uint32[224 * 224];
  paletteLiteral = new uint32[1 << 12];
  paletteStandard = new uint32[1 << 12];
  paletteEmulation = new uint32[1 << 12];
}

auto Video::power() -> void {
  memory::fill(output(), 224 * 224 * sizeof(uint32));

  for(uint12 color : range(1 << 12)) {
    paletteLiteral[color] = color;

    uint b = color.bits(0, 3);
    uint g = color.bits(4, 7);
    uint r = color.bits(8,11);

    uint R = image::normalize(r, 4, 16);
    uint G = image::normalize(g, 4, 16);
    uint B = image::normalize(b, 4, 16);
    paletteStandard[color] = interface->videoColor(R, G, B);

    //todo: this uses the Game Boy Advance color emulation algorithm
    //need to determine proper color emulation for WonderSwan systems
    R = (r * 26 + g *  4 + b *  2);
    G = (         g * 24 + b *  8);
    B = (r *  6 + g *  4 + b * 22);
    R = image::normalize(min(480, R), 9, 16);
    G = image::normalize(min(480, G), 9, 16);
    B = image::normalize(min(480, B), 9, 16);
    paletteEmulation[color] = interface->videoColor(R, G, B);
  }
}

auto Video::refresh() -> void {
  auto& palette = settings.colorEmulation ? paletteEmulation : paletteStandard;

  if(system.orientation() == 0) {
    for(uint y = 0; y < 224; y++) {
      auto target = output() + y * 224;
      if(y < 40 || y >= 184) {
        memory::fill(target, 224 * sizeof(uint32));
        continue;
      }
      auto source = ppu.output + (y - 40) * 224;
      for(uint x = 0; x < 224; x++) {
        auto color = palette[*source++];
        if(settings.blurEmulation) {
          auto a = color, b = *target;
          *target++ = (a + b - ((a ^ b) & 0x01010101)) >> 1;
        } else {
          *target++ = color;
        }
      }
    }
  }

  if(system.orientation() == 1) {
    for(uint y = 0; y < 224; y++) {
      auto target = output() + y * 224;
      memory::fill(target, 40 * sizeof(uint32));
      memory::fill(target + 184, 40 * sizeof(uint32));
      target += 40;
      for(uint x = 0; x < 144; x++) {
        auto source = ppu.output + x * 224 + (223 - y);
        auto color = palette[*source];
        if(settings.blurEmulation) {
          auto a = color, b = *target;
          *target++ = (a + b - ((a ^ b) & 0x01010101)) >> 1;
        } else {
          *target++ = color;
        }
      }
    }
  }

  interface->videoRefresh(output(), 224 * sizeof(uint32), 224, 224);
}
