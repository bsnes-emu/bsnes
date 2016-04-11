//note: this file is currently not compiled into higan
//it is temporarily only used as a reference

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
