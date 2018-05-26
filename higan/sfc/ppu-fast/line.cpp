auto PPU::Line::render() -> void {
  renderWindow(io.bg1.window);
  renderWindow(io.bg2.window);
  renderWindow(io.bg3.window);
  renderWindow(io.bg4.window);
  renderWindow(io.obj.window);
  renderWindow(io.col.window);
  renderBackground(io.bg1);
  renderBackground(io.bg2);
  renderBackground(io.bg3);
  renderBackground(io.bg4);
  renderObject(io.obj);

  if(io.displayDisable) {
    for(uint x : range(512)) {
      outputLo[x] = 0;
      outputHi[x] = 0;
    }
    return;
  }
}
