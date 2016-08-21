auto VDP::Background::scanline(uint y) -> void {
  uint15 address = io.horizontalScrollAddress;

  static const uint mask[] = {0u, 7u, ~7u, ~0u};
  address += (y & mask[io.horizontalScrollMode]) << 1;
  address += (this == &vdp.planeB);

  state.horizontalScroll = vdp.vram[address].bits(0,9);
}

auto VDP::Background::run(uint x, uint y) -> void {
  output.priority = 0;
  output.color = 0;

  static const uint tiles[] = {32, 64, 0, 128};
  y -= vdp.vsram[(x >> 4) & (io.verticalScrollMode ? ~0u : 0u)];
  y &= (tiles[io.nametableHeight] << 3) - 1;
  x -= state.horizontalScroll;
  x &= (tiles[io.nametableWidth] << 3) - 1;

  uint tileX = x >> 3;
  uint tileY = y >> 3;
  uint15 nametableAddress = io.nametableAddress;
  nametableAddress += tileY * tiles[io.nametableWidth] + tileX;

  uint16 tileAttributes = vdp.vram[nametableAddress];
  uint15 tileAddress = tileAttributes.bits(0,10) << 4;
  uint pixelX = (x & 7) ^ (tileAttributes.bit(11) ? 7 : 0);
  uint pixelY = (y & 7) ^ (tileAttributes.bit(12) ? 7 : 0);
  tileAddress += pixelY << 1 | pixelX >> 2;

  uint16 tileData = vdp.vram[tileAddress];
  uint4 color = tileData >> (((pixelX & 3) ^ 3) << 2);
  if(color) {
    output.color = tileAttributes.bits(13,14) << 4 | color;
    output.priority = tileAttributes.bit(15);
  }
}

auto VDP::Background::power() -> void {
}

auto VDP::Background::reset() -> void {
  memory::fill(&io, sizeof(IO));
  memory::fill(&state, sizeof(State));
}
