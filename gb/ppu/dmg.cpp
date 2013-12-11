#ifdef PPU_CPP

//OB attributes:
//0x80: 0 = OBJ above BG, 1 = BG above OBJ
//0x40: vertical flip
//0x20: horizontal flip
//0x10: palette#

void PPU::dmg_read_tile(bool select, unsigned x, unsigned y, unsigned& data) {
  unsigned tmaddr = 0x1800 + (select << 10), tdaddr;
  tmaddr += (((y >> 3) << 5) + (x >> 3)) & 0x03ff;
  if(status.bg_tiledata_select == 0) {
    tdaddr = 0x1000 + ((int8)vram[tmaddr] << 4);
  } else {
    tdaddr = 0x0000 + (vram[tmaddr] << 4);
  }
  tdaddr += (y & 7) << 1;
  data  = vram[tdaddr + 0] << 0;
  data |= vram[tdaddr + 1] << 8;
}

void PPU::dmg_scanline() {
  px = 0;

  const unsigned Height = (status.ob_size == 0 ? 8 : 16);
  sprites = 0;

  //find first ten sprites on this scanline
  for(unsigned n = 0; n < 40 * 4; n += 4) {
    Sprite& s = sprite[sprites];
    s.y = oam[n + 0] - 16;
    s.x = oam[n + 1] -  8;
    s.tile = oam[n + 2] & ~status.ob_size;
    s.attr = oam[n + 3];

    s.y = status.ly - s.y;
    if(s.y >= Height) continue;

    if(s.attr & 0x40) s.y ^= (Height - 1);
    unsigned tdaddr = (s.tile << 4) + (s.y << 1);
    s.data  = vram[tdaddr + 0] << 0;
    s.data |= vram[tdaddr + 1] << 8;
    if(s.attr & 0x20) s.data = hflip(s.data);

    if(++sprites == 10) break;
  }

  //sort by X-coordinate
  for(unsigned lo = 0; lo < sprites; lo++) {
    for(unsigned hi = lo + 1; hi < sprites; hi++) {
      if(sprite[hi].x < sprite[lo].x) std::swap(sprite[lo], sprite[hi]);
    }
  }
}

void PPU::dmg_run() {
  bg.color = 0;
  bg.palette = 0;

  ob.color = 0;
  ob.palette = 0;

  unsigned color = 0;
  if(status.display_enable) {
    if(status.bg_enable) dmg_run_bg();
    if(status.window_display_enable) dmg_run_window();
    if(status.ob_enable) dmg_run_ob();

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
  *output = video.palette[color];
}

void PPU::dmg_run_bg() {
  unsigned scrolly = (status.ly + status.scy) & 255;
  unsigned scrollx = (px + status.scx) & 255;
  unsigned tx = scrollx & 7;
  if(tx == 0 || px == 0) dmg_read_tile(status.bg_tilemap_select, scrollx, scrolly, background.data);

  unsigned index = 0;
  index |= (background.data & (0x0080 >> tx)) ? 1 : 0;
  index |= (background.data & (0x8000 >> tx)) ? 2 : 0;

  bg.color = bgp[index];
  bg.palette = index;
}

void PPU::dmg_run_window() {
  if(status.ly - status.wy >= 144u) return;
  if(status.wx >= 167u) return;
  unsigned scrolly = (status.ly - status.wy) & 255;
  unsigned scrollx = (px + 7 - status.wx) & 255;
  unsigned tx = scrollx & 7;
  if(tx == 0 || px == 0) dmg_read_tile(status.window_tilemap_select, scrollx, scrolly, window.data);

  unsigned index = 0;
  index |= (window.data & (0x0080 >> tx)) ? 1 : 0;
  index |= (window.data & (0x8000 >> tx)) ? 2 : 0;

  bg.color = bgp[index];
  bg.palette = index;
}

void PPU::dmg_run_ob() {
  //render backwards, so that first sprite has priority
  for(signed n = sprites - 1; n >= 0; n--) {
    Sprite& s = sprite[n];

    signed tx = px - s.x;
    if(tx < 0 || tx > 7) continue;

    unsigned index = 0;
    index |= (s.data & (0x0080 >> tx)) ? 1 : 0;
    index |= (s.data & (0x8000 >> tx)) ? 2 : 0;
    if(index == 0) continue;

    ob.color = obp[(bool)(s.attr & 0x10)][index];
    ob.palette = index;
    ob.priority = !(s.attr & 0x80);
  }
}

#endif
