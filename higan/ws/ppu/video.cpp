Video video;

Video::Video() {
  output = new uint32[224 * 224];
  paletteLiteral = new uint32[1 << 12];
  paletteStandard = new uint32[1 << 12];
}

auto Video::power() -> void {
  memory::fill(output(), 224 * 224 * sizeof(uint32));

  for(uint12 color : range(1 << 12)) {
    paletteLiteral[color] = color;

    uint B = color.bits(0, 3);
    uint G = color.bits(4, 7);
    uint R = color.bits(8,11);

    R = image::normalize(R, 4, 16);
    G = image::normalize(G, 4, 16);
    B = image::normalize(B, 4, 16);
    paletteStandard[color] = interface->videoColor(R, G, B);
  }
}

auto Video::refresh() -> void {
  if(system.orientation() == 0) {
    for(uint y = 0; y < 224; y++) {
      auto target = output() + y * 224;
      if(y < 40 || y >= 184) {
        memory::fill(target, 224 * sizeof(uint32));
        continue;
      }
      auto source = ppu.output + (y - 40) * 224;
      for(uint x = 0; x < 224; x++) {
        auto color = paletteStandard[*source++];
        *target++ = color;
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
        auto color = paletteStandard[*source];
        *target++ = color;
      }
    }
  }

  interface->videoRefresh(output(), 224 * sizeof(uint32), 224, 224);
}
