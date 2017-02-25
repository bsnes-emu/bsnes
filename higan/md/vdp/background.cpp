auto VDP::Background::isWindowed(uint x, uint y) -> bool {
  if((x < io.horizontalOffset) ^ io.horizontalDirection) return true;
  if((y < io.verticalOffset  ) ^ io.verticalDirection  ) return true;
  return false;
}

auto VDP::Background::updateHorizontalScroll(uint y) -> void {
  if(id == ID::Window) return;

  uint15 address = io.horizontalScrollAddress;

  static const uint mask[] = {0u, 7u, ~7u, ~0u};
  address += (y & mask[io.horizontalScrollMode]) << 1;
  address += id == ID::PlaneB;

  state.horizontalScroll = vdp.vram.read(address).bits(0,9);
}

auto VDP::Background::updateVerticalScroll(uint x, uint y) -> void {
  if(id == ID::Window) return;

  auto address = (x >> 4 & 0 - io.verticalScrollMode) << 1;
  address += id == ID::PlaneB;

  state.verticalScroll = vdp.vsram.read(address);
}

auto VDP::Background::nametableAddress() -> uint15 {
  if(id == ID::Window && vdp.screenWidth() == 320) return io.nametableAddress & ~0x0400;
  return io.nametableAddress;
}

auto VDP::Background::nametableWidth() -> uint {
  if(id == ID::Window) return vdp.screenWidth() == 320 ? 64 : 32;
  return 32 * (1 + io.nametableWidth);
}

auto VDP::Background::nametableHeight() -> uint {
  if(id == ID::Window) return 32;
  return 32 * (1 + io.nametableHeight);
}

auto VDP::Background::scanline(uint y) -> void {
  updateHorizontalScroll(y);
}

auto VDP::Background::run(uint x, uint y) -> void {
  updateVerticalScroll(x, y);

  output.priority = 0;
  output.color = 0;

  x -= state.horizontalScroll;
  y += state.verticalScroll;

  uint width  = nametableWidth();
  uint height = nametableHeight();

  uint tileX = x >> 3 & width  - 1;
  uint tileY = y >> 3 & height - 1;

  auto address = nametableAddress();
  address += (tileY * width + tileX) & 0x0fff;

  uint16 tileAttributes = vdp.vram.read(address);
  uint15 tileAddress = tileAttributes.bits(0,10) << 4;
  uint pixelX = (x & 7) ^ (tileAttributes.bit(11) ? 7 : 0);
  uint pixelY = (y & 7) ^ (tileAttributes.bit(12) ? 7 : 0);
  tileAddress += pixelY << 1 | pixelX >> 2;

  uint16 tileData = vdp.vram.read(tileAddress);
  uint4 color = tileData >> (((pixelX & 3) ^ 3) << 2);
  if(color) {
    output.color = tileAttributes.bits(13,14) << 4 | color;
    output.priority = tileAttributes.bit(15);
  }
}

auto VDP::Background::power() -> void {
  memory::fill(&io, sizeof(IO));
  memory::fill(&state, sizeof(State));
}
