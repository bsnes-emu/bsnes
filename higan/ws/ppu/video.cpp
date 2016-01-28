Video video;

Video::Video() {
  output = new uint32[224 * 224];
  paletteLiteral = new uint32[1 << 12];
  paletteStandard = new uint32[1 << 12];
}

auto Video::power() -> void {
  memory::fill(output(), 224 * 224 * sizeof(uint32));

  for(auto color : range(1 << 12)) {
    paletteLiteral[color] = color;

    uint R = (uint4)(color >> 8);
    uint G = (uint4)(color >> 4);
    uint B = (uint4)(color >> 0);

    R = image::normalize(R, 4, 16);
    G = image::normalize(G, 4, 16);
    B = image::normalize(B, 4, 16);
    paletteStandard[color] = interface->videoColor(R, G, B);
  }
}

auto Video::refresh() -> void {
  for(uint y = 0; y < 144; y++) {
    auto source = ppu.output + y * 224;
    auto target = output() + y * 224;
    for(uint x = 0; x < 224; x++) {
      auto color = paletteStandard[*source++];
      *target++ = color;
    }
  }

  interface->videoRefresh(output(), 224 * sizeof(uint32), 224, 224);
  scheduler.exit(Scheduler::ExitReason::FrameEvent);
}
