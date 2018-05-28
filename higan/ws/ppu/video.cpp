//note: this file is currently not compiled into higan
//it is temporarily only used as a reference

auto Video::refresh() -> void {
  auto& palette = settings.colorEmulation ? paletteEmulation : paletteStandard;

  if(system.orientation() == 0) {
    for(uint y : range(224)) {
      auto target = output() + y * 224;
      if(y < 40 || y >= 184) {
        memory::fill<uint32>(target, 224);
        continue;
      }
      auto source = ppu.output + (y - 40) * 224;
      for(uint x : range(224)) {
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
    for(uint y : range(224)) {
      auto target = output() + y * 224;
      memory::fill<uint32>(target, 40);
      memory::fill<uint32>(target + 184, 40);
      target += 40;
      for(uint x : range(144)) {
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
