//OB attributes:
//0x80: 0 = OBJ above BG, 1 = BG above OBJ
//0x40: vertical flip
//0x20: horizontal flip
//0x10: palette#

auto PPU::readTileDMG(bool select, uint x, uint y, uint& data) -> void {
  uint tmaddr = 0x1800 + (select << 10), tdaddr;
  tmaddr += (((y >> 3) << 5) + (x >> 3)) & 0x03ff;
  if(status.bgTiledataSelect == 0) {
    tdaddr = 0x1000 + ((int8)vram[tmaddr] << 4);
  } else {
    tdaddr = 0x0000 + (vram[tmaddr] << 4);
  }
  tdaddr += (y & 7) << 1;
  data  = vram[tdaddr + 0] << 0;
  data |= vram[tdaddr + 1] << 8;
}

auto PPU::scanlineDMG() -> void {
  px = 0;
  if(!enabled()) return;

  const uint Height = (status.obSize == 0 ? 8 : 16);
  sprites = 0;

  //find first ten sprites on this scanline
  for(uint n = 0; n < 40 * 4; n += 4) {
    Sprite& s = sprite[sprites];
    s.y = oam[n + 0] - 16;
    s.x = oam[n + 1] -  8;
    s.tile = oam[n + 2] & ~status.obSize;
    s.attr = oam[n + 3];

    s.y = status.ly - s.y;
    if(s.y >= Height) continue;

    if(s.attr & 0x40) s.y ^= (Height - 1);
    uint tdaddr = (s.tile << 4) + (s.y << 1);
    s.data  = vram[tdaddr + 0] << 0;
    s.data |= vram[tdaddr + 1] << 8;
    if(s.attr & 0x20) s.data = hflip(s.data);

    if(++sprites == 10) break;
  }

  //sort by X-coordinate
  for(uint lo = 0; lo < sprites; lo++) {
    for(uint hi = lo + 1; hi < sprites; hi++) {
      if(sprite[hi].x < sprite[lo].x) swap(sprite[lo], sprite[hi]);
    }
  }
}

auto PPU::runDMG() -> void {
  bg.color = 0;
  bg.palette = 0;

  ob.color = 0;
  ob.palette = 0;

  uint color = 0;
  if(enabled()) {
    if(status.bgEnable) runBackgroundDMG();
    if(status.windowDisplayEnable) runWindowDMG();
    if(status.obEnable) runObjectsDMG();

    if(ob.palette == 0) {
      color = bg.color;
    } else if(bg.palette == 0) {
      color = ob.color;
    } else if(ob.priority) {
      color = ob.color;
    } else {
      color = bg.color;
    }
  }

  uint32* output = screen + status.ly * 160 + px++;
  *output = color;
  if(Model::SuperGameBoy()) superGameBoy->lcdOutput(color);
}

auto PPU::runBackgroundDMG() -> void {
  uint scrolly = (status.ly + status.scy) & 255;
  uint scrollx = (px + status.scx) & 255;
  uint tx = scrollx & 7;
  if(tx == 0 || px == 0) readTileDMG(status.bgTilemapSelect, scrollx, scrolly, background.data);

  uint index = 0;
  index |= (background.data & (0x0080 >> tx)) ? 1 : 0;
  index |= (background.data & (0x8000 >> tx)) ? 2 : 0;

  bg.color = bgp[index];
  bg.palette = index;
}

auto PPU::runWindowDMG() -> void {
  uint scrolly = status.ly - status.wy;
  uint scrollx = px + 7 - status.wx;
  if(scrolly >= 144u) return;  //also matches underflow (scrolly < 0)
  if(scrollx >= 160u) return;  //also matches underflow (scrollx < 0)
  uint tx = scrollx & 7;
  if(tx == 0 || px == 0) readTileDMG(status.windowTilemapSelect, scrollx, scrolly, window.data);

  uint index = 0;
  index |= (window.data & (0x0080 >> tx)) ? 1 : 0;
  index |= (window.data & (0x8000 >> tx)) ? 2 : 0;

  bg.color = bgp[index];
  bg.palette = index;
}

auto PPU::runObjectsDMG() -> void {
  //render backwards, so that first sprite has priority
  for(int n = sprites - 1; n >= 0; n--) {
    Sprite& s = sprite[n];

    int tx = px - s.x;
    if(tx < 0 || tx > 7) continue;

    uint index = 0;
    index |= (s.data & (0x0080 >> tx)) ? 1 : 0;
    index |= (s.data & (0x8000 >> tx)) ? 2 : 0;
    if(index == 0) continue;

    ob.color = obp[(bool)(s.attr & 0x10)][index];
    ob.palette = index;
    ob.priority = !(s.attr & 0x80);
  }
}
