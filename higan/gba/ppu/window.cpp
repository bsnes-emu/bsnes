/*
auto PPU::renderWindow(uint w) -> void {
  uint y = regs.vcounter;

  uint y1 = regs.window[w].y1, y2 = regs.window[w].y2;
  uint x1 = regs.window[w].x1, x2 = regs.window[w].x2;

  if(y2 < y1 || y2 > 160) y2 = 160;
  if(x2 < x1 || x2 > 240) x2 = 240;

  if(y >= y1 && y < y2) {
    for(uint x = x1; x < x2; x++) {
      windowmask[w][x] = true;
    }
  }
}
*/

auto PPU::Window::run(uint x, uint y) -> void {
  auto x1 = io.x1, x2 = io.x2;
  auto y1 = io.y1, y2 = io.y2;

  if(x2 < x1 || x2 > 240) x2 = 240;
  if(y2 < y1 || y2 > 160) y2 = 160;

  output = (x >= x1 && x < x2 && y >= y1 && y < y2);
}

auto PPU::Window::power(uint id) -> void {
  this->id = id;

  memory::fill(&io, sizeof(IO));
}
