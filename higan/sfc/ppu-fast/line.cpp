auto PPU::Line::render() -> void {
  if(io.displayDisable) {
    for(uint x : range(512)) {
      outputLo[x] = 0;
      outputHi[x] = 0;
    }
  } else {
    auto aboveColor = cgram[0];
    auto belowColor = 0;
    for(uint x : range(256)) {
      above[x].source   = Source::COL;
      above[x].priority = 0;
      above[x].color    = aboveColor;
      below[x].source   = Source::COL;
      below[x].priority = 0;
      below[x].color    = belowColor;
    }
  }

  renderWindow(io.bg1.window);
  renderWindow(io.bg2.window);
  renderWindow(io.bg3.window);
  renderWindow(io.bg4.window);
  renderWindow(io.obj.window);
  renderWindow(io.col.window);
  renderBackground(io.bg1, Source::BG1);
  renderBackground(io.bg2, Source::BG2);
  renderBackground(io.bg3, Source::BG3);
  renderBackground(io.bg4, Source::BG4);
  renderObject(io.obj);

  for(uint x : range(512)) {
    outputLo[x] = io.displayBrightness << 15 | above[x >> 1].color;
    outputHi[x] = io.displayBrightness << 15 | above[x >> 1].color;
  }
}

auto PPU::Line::plotAbove(uint x, uint source, uint priority, uint color) -> void {
  if(priority > above[x].priority) {
    above[x].source   = source;
    above[x].priority = priority;
    above[x].color    = color;
  }
}

auto PPU::Line::plotBelow(uint x, uint source, uint priority, uint color) -> void {
  if(priority > below[x].priority) {
    below[x].source   = source;
    below[x].priority = priority;
    below[x].color    = color;
  }
}
