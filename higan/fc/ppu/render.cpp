auto PPU::enable() const -> bool {
  return r.bgEnable || r.spriteEnable;
}

auto PPU::loadCHR(uint16 addr) -> uint8 {
  return enable() ? cartridge.readCHR(addr) : (uint8)0x00;
}

auto PPU::renderPixel() -> void {
  uint32* output = buffer + r.ly * 256;

  uint x = r.lx - 1;
  uint mask = 0x8000 >> (r.v.fineX + (x & 7));
  uint palette = 0;
  uint objectPalette = 0;
  bool objectPriority = 0;

  palette |= l.tiledataLo & mask ? 1 : 0;
  palette |= l.tiledataHi & mask ? 2 : 0;
  if(palette) {
    uint attr = l.attribute;
    if(mask >= 256) attr >>= 2;
    palette |= (attr & 3) << 2;
  }

  if(!r.bgEnable) palette = 0;
  if(!r.bgEdgeEnable && x < 8) palette = 0;

  if(r.spriteEnable)
  for(int sprite = 7; sprite >= 0; sprite--) {
    if(!r.spriteEdgeEnable && x < 8) continue;
    if(l.oam[sprite].id == 64) continue;

    uint spriteX = x - l.oam[sprite].x;
    if(spriteX >= 8) continue;

    if(l.oam[sprite].attr & 0x40) spriteX ^= 7;
    uint mask = 0x80 >> spriteX;
    uint spritePalette = 0;
    spritePalette |= l.oam[sprite].tiledataLo & mask ? 1 : 0;
    spritePalette |= l.oam[sprite].tiledataHi & mask ? 2 : 0;
    if(spritePalette == 0) continue;

    if(l.oam[sprite].id == 0 && palette && x != 255) r.spriteZeroHit = 1;
    spritePalette |= (l.oam[sprite].attr & 3) << 2;

    objectPriority = l.oam[sprite].attr & 0x20;
    objectPalette = 16 + spritePalette;
  }

  if(objectPalette) {
    if(palette == 0 || objectPriority == 0) palette = objectPalette;
  }

  if(!enable()) palette = 0;
  output[x] = r.emphasis << 6 | readCGRAM(palette);
}

auto PPU::renderSprite() -> void {
  if(!enable()) return;

  uint n = l.oamIterator++;
  int ly = r.ly == 261 ? -1 : r.ly;
  uint y = ly - oam[n * 4 + 0];

  if(y >= r.spriteHeight) return;
  if(l.oamCounter == 8) {
    r.spriteOverflow = 1;
    return;
  }

  auto& o = l.soam[l.oamCounter++];
  o.id   = n;
  o.y    = oam[n * 4 + 0];
  o.tile = oam[n * 4 + 1];
  o.attr = oam[n * 4 + 2];
  o.x    = oam[n * 4 + 3];
}

auto PPU::renderScanline() -> void {
  //Vblank
  if(r.ly >= 240 && r.ly <= 260) return step(341), scanline();

  l.oamIterator = 0;
  l.oamCounter = 0;

  for(auto n : range(8)) l.soam[n] = {};

  //  0
  step(1);

  //  1-256
  for(uint tile : range(32)) {
    uint nametable = loadCHR(0x2000 | (uint12)r.v.address);
    uint tileaddr = r.bgAddress | nametable << 4 | r.v.fineY;
    renderPixel();
    step(1);

    renderPixel();
    step(1);

    uint attribute = loadCHR(0x23c0 | r.v.nametable << 10 | (r.v.tileY >> 2) << 3 | r.v.tileX >> 2);
    if(r.v.tileY & 2) attribute >>= 4;
    if(r.v.tileX & 2) attribute >>= 2;
    renderPixel();
    step(1);

    if(enable() && ++r.v.tileX == 0) r.v.nametableX++;
    if(enable() && tile == 31 && ++r.v.fineY == 0 && ++r.v.tileY == 30) r.v.nametableY++, r.v.tileY = 0;
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

    l.nametable = l.nametable << 8 | nametable;
    l.attribute = l.attribute << 2 | (attribute & 3);
    l.tiledataLo = l.tiledataLo << 8 | tiledataLo;
    l.tiledataHi = l.tiledataHi << 8 | tiledataHi;
  }

  for(auto n : range(8)) l.oam[n] = l.soam[n];

  //257-320
  for(uint sprite : range(8)) {
    uint nametable = loadCHR(0x2000 | (uint12)r.v.address);
    step(1);

    if(enable() && sprite == 0) {
      //258
      r.v.nametableX = r.t.nametableX;
      r.v.tileX = r.t.tileX;
    }
    step(1);

    uint attribute = loadCHR(0x23c0 | r.v.nametable << 10 | (r.v.tileY >> 2) << 3 | r.v.tileX >> 2);
    uint tileaddr = r.spriteHeight == 8
    ? r.spriteAddress + l.oam[sprite].tile * 16
    : (l.oam[sprite].tile & ~1) * 16 + (l.oam[sprite].tile & 1) * 0x1000;
    step(2);

    uint spriteY = (r.ly - l.oam[sprite].y) & (r.spriteHeight - 1);
    if(l.oam[sprite].attr & 0x80) spriteY ^= r.spriteHeight - 1;
    tileaddr += spriteY + (spriteY & 8);

    l.oam[sprite].tiledataLo = loadCHR(tileaddr + 0);
    step(2);

    l.oam[sprite].tiledataHi = loadCHR(tileaddr + 8);
    step(2);

    if(enable() && sprite == 6 && r.ly == 261) {
      //305
      r.v.address = r.t.address;
    }
  }

  //321-336
  for(uint tile : range(2)) {
    uint nametable = loadCHR(0x2000 | (uint12)r.v.address);
    uint tileaddr = r.bgAddress | nametable << 4 | r.v.fineY;
    step(2);

    uint attribute = loadCHR(0x23c0 | r.v.nametable << 10 | (r.v.tileY >> 2) << 3 | r.v.tileX >> 2);
    if(r.v.tileY & 2) attribute >>= 4;
    if(r.v.tileX & 2) attribute >>= 2;
    step(1);

    if(enable() && ++r.v.tileX == 0) r.v.nametableX++;
    step(1);

    uint tiledataLo = loadCHR(tileaddr + 0);
    step(2);

    uint tiledataHi = loadCHR(tileaddr + 8);
    step(2);

    l.nametable = l.nametable << 8 | nametable;
    l.attribute = l.attribute << 2 | (attribute & 3);
    l.tiledataLo = l.tiledataLo << 8 | tiledataLo;
    l.tiledataHi = l.tiledataHi << 8 | tiledataHi;
  }

  //337-338
  loadCHR(0x2000 | (uint12)r.v.address);
  step(1);
  bool skip = enable() && r.field == 1 && r.ly == 261;
  step(1);

  //339
  loadCHR(0x2000 | (uint12)r.v.address);
  step(1);

  //340
  if(!skip) step(1);

  return scanline();
}
