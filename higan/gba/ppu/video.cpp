Video video;

Video::Video() {
  output = new uint32[240 * 160];
  paletteLiteral = new uint32[1 << 15];
  paletteStandard = new uint32[1 << 15];
  paletteEmulation = new uint32[1 << 15];
}

auto Video::power() -> void {
  memory::fill(output(), 240 * 160 * sizeof(uint32));

  for(auto color : range(1 << 15)) {
    paletteLiteral[color] = color;

    uint B = (uint5)(color >> 10);
    uint G = (uint5)(color >>  5);
    uint R = (uint5)(color >>  0);

    uint b = image::normalize(B, 5, 16);
    uint g = image::normalize(G, 5, 16);
    uint r = image::normalize(R, 5, 16);
    paletteStandard[color] = interface->videoColor(r, g, b);

    double lcdGamma = 4.0, outGamma = 2.2;
    double lb = pow(B / 31.0, lcdGamma);
    double lg = pow(G / 31.0, lcdGamma);
    double lr = pow(R / 31.0, lcdGamma);
    b = pow((220 * lb +  10 * lg +  50 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
    g = pow(( 30 * lb + 230 * lg +  10 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
    r = pow((  0 * lb +  50 * lg + 255 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
    paletteEmulation[color] = interface->videoColor(r, g, b);
  }
}

auto Video::refresh() -> void {
  auto output = this->output();
  auto& palette = settings.colorEmulation ? paletteEmulation : paletteStandard;

  for(uint y = 0; y < 160; y++) {
    auto source = ppu.output + y * 240;
    auto target = output + y * 240;

    if(settings.blurEmulation) {
      for(uint x = 0; x < 240; x++) {
        auto a = palette[*source++];
        auto b = *target;
        *target++ = (a + b - ((a ^ b) & 0x01010101)) >> 1;
      }
    } else {
      for(uint x = 0; x < 240; x++) {
        auto color = palette[*source++];
        *target++ = color;
      }
    }
  }

  interface->videoRefresh(output, 240 * sizeof(uint32), 240, 160);
}
