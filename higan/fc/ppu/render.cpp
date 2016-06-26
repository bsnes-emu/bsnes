//vaddr = 0yyy VHYY  YYYX XXXX
//yyy = fine Yscroll (y:d0-d2)
//V = V nametable (y:d8)
//H = H nametable (x:d8)
//YYYYY = Y nametable (y:d3-d7)
//XXXXX = X nametable (x:d3-d7)

auto PPU::enable() const -> bool {
  return r.bgEnable || r.spriteEnable;
}

auto PPU::nametableAddress() const -> uint {
  return 0x2000 + (r.vaddr & 0x0c00);
}

auto PPU::scrollX() const -> uint {
  return ((r.vaddr & 0x1f) << 3) | r.xaddr;
}

auto PPU::scrollY() const -> uint {
  return (((r.vaddr >> 5) & 0x1f) << 3) | ((r.vaddr >> 12) & 7);
}

//

auto PPU::loadCHR(uint16 addr) -> uint8 {
  if(!enable()) return 0x00;
  return cartridge.readCHR(addr);
}

//

auto PPU::scrollX_increment() -> void {
  if(!enable()) return;
  r.vaddr = (r.vaddr & 0x7fe0) | ((r.vaddr + 0x0001) & 0x001f);
  if((r.vaddr & 0x001f) == 0x0000) {
    r.vaddr ^= 0x0400;
  }
}

auto PPU::scrollY_increment() -> void {
  if(!enable()) return;
  r.vaddr = (r.vaddr & 0x0fff) | ((r.vaddr + 0x1000) & 0x7000);
  if((r.vaddr & 0x7000) == 0x0000) {
    r.vaddr = (r.vaddr & 0x7c1f) | ((r.vaddr + 0x0020) & 0x03e0);
    if((r.vaddr & 0x03e0) == 0x03c0) {  //0x03c0 == 30 << 5; 30 * 8 = 240
      r.vaddr &= 0x7c1f;
      r.vaddr ^= 0x0800;
    }
  }
}

//

auto PPU::renderPixel() -> void {
  uint32* output = buffer + r.ly * 256;

  uint mask = 0x8000 >> (r.xaddr + (r.lx & 7));
  uint palette = 0;
  uint objectPalette = 0;
  bool objectPriority = 0;
  palette |= (l.tiledataLo & mask) ? 1 : 0;
  palette |= (l.tiledataHi & mask) ? 2 : 0;
  if(palette) {
    uint attr = l.attribute;
    if(mask >= 256) attr >>= 2;
    palette |= (attr & 3) << 2;
  }

  if(!r.bgEnable) palette = 0;
  if(!r.bgEdgeEnable && r.lx < 8) palette = 0;

  if(r.spriteEnable)
  for(int sprite = 7; sprite >= 0; sprite--) {
    if(!r.spriteEdgeEnable && r.lx < 8) continue;
    if(l.oam[sprite].id == 64) continue;

    uint spriteX = r.lx - l.oam[sprite].x;
    if(spriteX >= 8) continue;

    if(l.oam[sprite].attr & 0x40) spriteX ^= 7;
    uint mask = 0x80 >> spriteX;
    uint spritePalette = 0;
    spritePalette |= (l.oam[sprite].tiledataLo & mask) ? 1 : 0;
    spritePalette |= (l.oam[sprite].tiledataHi & mask) ? 2 : 0;
    if(spritePalette == 0) continue;

    if(l.oam[sprite].id == 0 && palette && r.lx != 255) r.spriteZeroHit = 1;
    spritePalette |= (l.oam[sprite].attr & 3) << 2;

    objectPriority = l.oam[sprite].attr & 0x20;
    objectPalette = 16 + spritePalette;
  }

  if(objectPalette) {
    if(palette == 0 || objectPriority == 0) palette = objectPalette;
  }

  if(!enable()) palette = 0;
  output[r.lx] = r.emphasis << 6 | readCGRAM(palette);
}

auto PPU::renderSprite() -> void {
  if(!enable()) return;

  uint n = l.oamIterator++;
  int ly = (r.ly == 261 ? -1 : r.ly);
  uint y = ly - oam[(n * 4) + 0];

  if(y >= r.spriteHeight) return;
  if(l.oamCounter == 8) {
    r.spriteOverflow = 1;
    return;
  }

  auto& o = l.soam[l.oamCounter++];
  o.id   = n;
  o.y    = oam[(n * 4) + 0];
  o.tile = oam[(n * 4) + 1];
  o.attr = oam[(n * 4) + 2];
  o.x    = oam[(n * 4) + 3];
}

auto PPU::renderScanline() -> void {
  if(r.ly >= 240 && r.ly <= 260) {
    for(auto x : range(341)) tick();
    return scanline();
  }

  l.oamIterator = 0;
  l.oamCounter = 0;

  for(auto n : range(8)) l.soam[n] = {};

  for(uint tile : range(32)) {  //  0-255
    uint nametable = loadCHR(0x2000 | (r.vaddr & 0x0fff));
    uint tileaddr = r.bgAddress + (nametable << 4) + (scrollY() & 7);
    renderPixel();
    tick();

    renderPixel();
    tick();

    uint attribute = loadCHR(0x23c0 | (r.vaddr & 0x0fc0) | ((scrollY() >> 5) << 3) | (scrollX() >> 5));
    if(scrollY() & 16) attribute >>= 4;
    if(scrollX() & 16) attribute >>= 2;
    renderPixel();
    tick();

    scrollX_increment();
    if(tile == 31) scrollY_increment();
    renderPixel();
    renderSprite();
    tick();

    uint tiledataLo = loadCHR(tileaddr + 0);
    renderPixel();
    tick();

    renderPixel();
    tick();

    uint tiledataHi = loadCHR(tileaddr + 8);
    renderPixel();
    tick();

    renderPixel();
    renderSprite();
    tick();

    l.nametable = l.nametable << 8 | nametable;
    l.attribute = l.attribute << 2 | (attribute & 3);
    l.tiledataLo = l.tiledataLo << 8 | tiledataLo;
    l.tiledataHi = l.tiledataHi << 8 | tiledataHi;
  }

  for(auto n : range(8)) l.oam[n] = l.soam[n];

  for(uint sprite : range(8)) {  //256-319
    uint nametable = loadCHR(0x2000 | (r.vaddr & 0x0fff));
    tick();

    if(enable() && sprite == 0) r.vaddr = (r.vaddr & 0x7be0) | (r.taddr & 0x041f);  //257
    tick();

    uint attribute = loadCHR(0x23c0 | (r.vaddr & 0x0fc0) | ((scrollY() >> 5) << 3) | (scrollX() >> 5));
    uint tileaddr = (r.spriteHeight == 8)
    ? r.spriteAddress + l.oam[sprite].tile * 16
    : ((l.oam[sprite].tile & ~1) * 16) + ((l.oam[sprite].tile & 1) * 0x1000);
    tick();
    tick();

    uint spriteY = (r.ly - l.oam[sprite].y) & (r.spriteHeight - 1);
    if(l.oam[sprite].attr & 0x80) spriteY ^= (r.spriteHeight - 1);
    tileaddr += spriteY + (spriteY & 8);

    l.oam[sprite].tiledataLo = loadCHR(tileaddr + 0);
    tick();
    tick();

    l.oam[sprite].tiledataHi = loadCHR(tileaddr + 8);
    tick();
    tick();

    if(enable() && sprite == 6 && r.ly == 261) r.vaddr = r.taddr;  //304
  }

  for(uint tile : range(2)) {  //320-335
    uint nametable = loadCHR(0x2000 | (r.vaddr & 0x0fff));
    uint tileaddr = r.bgAddress + (nametable << 4) + (scrollY() & 7);
    tick();
    tick();

    uint attribute = loadCHR(0x23c0 | (r.vaddr & 0x0fc0) | ((scrollY() >> 5) << 3) | (scrollX() >> 5));
    if(scrollY() & 16) attribute >>= 4;
    if(scrollX() & 16) attribute >>= 2;
    tick();

    scrollX_increment();
    tick();

    uint tiledataLo = loadCHR(tileaddr + 0);
    tick();
    tick();

    uint tiledataHi = loadCHR(tileaddr + 8);
    tick();
    tick();

    l.nametable = l.nametable << 8 | nametable;
    l.attribute = l.attribute << 2 | (attribute & 3);
    l.tiledataLo = l.tiledataLo << 8 | tiledataLo;
    l.tiledataHi = l.tiledataHi << 8 | tiledataHi;
  }

  //336-339
  loadCHR(0x2000 | (r.vaddr & 0x0fff));
  tick();
  bool skip = enable() && r.field == 1 && r.ly == 261;
  tick();

  loadCHR(0x2000 | (r.vaddr & 0x0fff));
  tick();
  tick();

  //340
  if(!skip) tick();

  return scanline();
}
