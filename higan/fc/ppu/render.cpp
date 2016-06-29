auto PPU::enable() const -> bool {
  return io.bgEnable || io.spriteEnable;
}

auto PPU::loadCHR(uint16 addr) -> uint8 {
  return enable() ? cartridge.readCHR(addr) : (uint8)0x00;
}

auto PPU::renderPixel() -> void {
  uint32* output = buffer + io.ly * 256;

  uint x = io.lx - 1;
  uint mask = 0x8000 >> (io.v.fineX + (x & 7));
  uint palette = 0;
  uint objectPalette = 0;
  bool objectPriority = 0;

  palette |= latch.tiledataLo & mask ? 1 : 0;
  palette |= latch.tiledataHi & mask ? 2 : 0;
  if(palette) {
    uint attr = latch.attribute;
    if(mask >= 256) attr >>= 2;
    palette |= (attr & 3) << 2;
  }

  if(!io.bgEnable) palette = 0;
  if(!io.bgEdgeEnable && x < 8) palette = 0;

  if(io.spriteEnable)
  for(int sprite = 7; sprite >= 0; sprite--) {
    if(!io.spriteEdgeEnable && x < 8) continue;
    if(latch.oam[sprite].id == 64) continue;

    uint spriteX = x - latch.oam[sprite].x;
    if(spriteX >= 8) continue;

    if(latch.oam[sprite].attr & 0x40) spriteX ^= 7;
    uint mask = 0x80 >> spriteX;
    uint spritePalette = 0;
    spritePalette |= latch.oam[sprite].tiledataLo & mask ? 1 : 0;
    spritePalette |= latch.oam[sprite].tiledataHi & mask ? 2 : 0;
    if(spritePalette == 0) continue;

    if(latch.oam[sprite].id == 0 && palette && x != 255) io.spriteZeroHit = 1;
    spritePalette |= (latch.oam[sprite].attr & 3) << 2;

    objectPriority = latch.oam[sprite].attr & 0x20;
    objectPalette = 16 + spritePalette;
  }

  if(objectPalette) {
    if(palette == 0 || objectPriority == 0) palette = objectPalette;
  }

  if(!enable()) palette = 0;
  output[x] = io.emphasis << 6 | readCGRAM(palette);
}

auto PPU::renderSprite() -> void {
  if(!enable()) return;

  uint n = latch.oamIterator++;
  int ly = io.ly == 261 ? -1 : io.ly;
  uint y = ly - oam[n * 4 + 0];

  if(y >= io.spriteHeight) return;
  if(latch.oamCounter == 8) {
    io.spriteOverflow = 1;
    return;
  }

  auto& o = latch.soam[latch.oamCounter++];
  o.id   = n;
  o.y    = oam[n * 4 + 0];
  o.tile = oam[n * 4 + 1];
  o.attr = oam[n * 4 + 2];
  o.x    = oam[n * 4 + 3];
}

auto PPU::renderScanline() -> void {
  //Vblank
  if(io.ly >= 240 && io.ly <= 260) return step(341), scanline();

  latch.oamIterator = 0;
  latch.oamCounter = 0;

  for(auto n : range(8)) latch.soam[n] = {};

  //  0
  step(1);

  //  1-256
  for(uint tile : range(32)) {
    uint nametable = loadCHR(0x2000 | (uint12)io.v.address);
    uint tileaddr = io.bgAddress | nametable << 4 | io.v.fineY;
    renderPixel();
    step(1);

    renderPixel();
    step(1);

    uint attribute = loadCHR(0x23c0 | io.v.nametable << 10 | (io.v.tileY >> 2) << 3 | io.v.tileX >> 2);
    if(io.v.tileY & 2) attribute >>= 4;
    if(io.v.tileX & 2) attribute >>= 2;
    renderPixel();
    step(1);

    if(enable() && ++io.v.tileX == 0) io.v.nametableX++;
    if(enable() && tile == 31 && ++io.v.fineY == 0 && ++io.v.tileY == 30) io.v.nametableY++, io.v.tileY = 0;
    renderPixel();
    renderSprite();
    step(1);

    uint tiledataLo = loadCHR(tileaddr + 0);
    renderPixel();
    step(1);

    renderPixel();
    step(1);

    uint tiledataHi = loadCHR(tileaddr + 8);
    renderPixel();
    step(1);

    renderPixel();
    renderSprite();
    step(1);

    latch.nametable = latch.nametable << 8 | nametable;
    latch.attribute = latch.attribute << 2 | (attribute & 3);
    latch.tiledataLo = latch.tiledataLo << 8 | tiledataLo;
    latch.tiledataHi = latch.tiledataHi << 8 | tiledataHi;
  }

  for(auto n : range(8)) latch.oam[n] = latch.soam[n];

  //257-320
  for(uint sprite : range(8)) {
    uint nametable = loadCHR(0x2000 | (uint12)io.v.address);
    step(1);

    if(enable() && sprite == 0) {
      //258
      io.v.nametableX = io.t.nametableX;
      io.v.tileX = io.t.tileX;
    }
    step(1);

    uint attribute = loadCHR(0x23c0 | io.v.nametable << 10 | (io.v.tileY >> 2) << 3 | io.v.tileX >> 2);
    uint tileaddr = io.spriteHeight == 8
    ? io.spriteAddress + latch.oam[sprite].tile * 16
    : (latch.oam[sprite].tile & ~1) * 16 + (latch.oam[sprite].tile & 1) * 0x1000;
    step(2);

    uint spriteY = (io.ly - latch.oam[sprite].y) & (io.spriteHeight - 1);
    if(latch.oam[sprite].attr & 0x80) spriteY ^= io.spriteHeight - 1;
    tileaddr += spriteY + (spriteY & 8);

    latch.oam[sprite].tiledataLo = loadCHR(tileaddr + 0);
    step(2);

    latch.oam[sprite].tiledataHi = loadCHR(tileaddr + 8);
    step(2);

    if(enable() && sprite == 6 && io.ly == 261) {
      //305
      io.v.address = io.t.address;
    }
  }

  //321-336
  for(uint tile : range(2)) {
    uint nametable = loadCHR(0x2000 | (uint12)io.v.address);
    uint tileaddr = io.bgAddress | nametable << 4 | io.v.fineY;
    step(2);

    uint attribute = loadCHR(0x23c0 | io.v.nametable << 10 | (io.v.tileY >> 2) << 3 | io.v.tileX >> 2);
    if(io.v.tileY & 2) attribute >>= 4;
    if(io.v.tileX & 2) attribute >>= 2;
    step(1);

    if(enable() && ++io.v.tileX == 0) io.v.nametableX++;
    step(1);

    uint tiledataLo = loadCHR(tileaddr + 0);
    step(2);

    uint tiledataHi = loadCHR(tileaddr + 8);
    step(2);

    latch.nametable = latch.nametable << 8 | nametable;
    latch.attribute = latch.attribute << 2 | (attribute & 3);
    latch.tiledataLo = latch.tiledataLo << 8 | tiledataLo;
    latch.tiledataHi = latch.tiledataHi << 8 | tiledataHi;
  }

  //337-338
  loadCHR(0x2000 | (uint12)io.v.address);
  step(1);
  bool skip = enable() && io.field == 1 && io.ly == 261;
  step(1);

  //339
  loadCHR(0x2000 | (uint12)io.v.address);
  step(1);

  //340
  if(!skip) step(1);

  return scanline();
}
