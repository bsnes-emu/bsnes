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
  output = 0;
}
