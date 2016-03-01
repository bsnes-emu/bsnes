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
  for(uint y = 0; y < 144; y++) {
    auto source = ppu.output + y * 224;
    for(uint x = 0; x < 224; x++) {
      auto color = paletteStandard[*source++];
    //*(output() + y * 224 + x) = color;
      *(output() + (223 - x) * 224 + 40 + y) = color;
    }
  }

  interface->videoRefresh(output(), 224 * sizeof(uint32), 224, 224);
}
