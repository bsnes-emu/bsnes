auto PPU::Background::clip(int n) -> int {
  //13-bit sign extend: --s---nnnnnnnnnn -> ssssssnnnnnnnnnn
  return n & 0x2000 ? (n | ~1023) : (n & 1023);
}

//H = 28
auto PPU::Background::beginMode7() -> void {
  latch.hoffset = ppu.io.hoffsetMode7;
  latch.voffset = ppu.io.voffsetMode7;
}

auto PPU::Background::runMode7() -> void {
  int a = (int16)ppu.io.m7a;
  int b = (int16)ppu.io.m7b;
  int c = (int16)ppu.io.m7c;
  int d = (int16)ppu.io.m7d;

  int cx = (int13)ppu.io.m7x;
  int cy = (int13)ppu.io.m7y;
  int hoffset = (int13)latch.hoffset;
  int voffset = (int13)latch.voffset;

  if(Background::x++ & ~255) return;
  uint x = mosaic.hoffset;
  uint y = ppu.bg1.mosaic.voffset;  //BG2 vertical mosaic uses BG1 mosaic size

  if(--mosaic.hcounter == 0) {
    mosaic.hcounter = mosaic.size + 1;
    mosaic.hoffset += mosaic.size + 1;
  }

  if(ppu.io.hflipMode7) x = 255 - x;
  if(ppu.io.vflipMode7) y = 255 - y;

  int psx = ((a * clip(hoffset - cx)) & ~63) + ((b * clip(voffset - cy)) & ~63) + ((b * y) & ~63) + (cx << 8);
  int psy = ((c * clip(hoffset - cx)) & ~63) + ((d * clip(voffset - cy)) & ~63) + ((d * y) & ~63) + (cy << 8);

  int px = psx + (a * x);
  int py = psy + (c * x);

  //mask pseudo-FP bits
  px >>= 8;
  py >>= 8;

  uint tile;
  uint palette;
  switch(ppu.io.repeatMode7) {
  //screen repetition outside of screen area
  case 0:
  case 1:
    px &= 1023;
    py &= 1023;
    tile = bit8(ppu.vram[(py >> 3) * 128 + (px >> 3)],0);
    palette = bit8(ppu.vram[(tile << 6) + ((py & 7) << 3) + (px & 7)],1);
    break;

  //palette color 0 outside of screen area
  case 2:
    if((px | py) & ~1023) {
      palette = 0;
    } else {
      px &= 1023;
      py &= 1023;
      tile = bit8(ppu.vram[(py >> 3) * 128 + (px >> 3)],0);
      palette = bit8(ppu.vram[(tile << 6) + ((py & 7) << 3) + (px & 7)],1);
    }
    break;

  //character 0 repetition outside of screen area
  case 3:
    if((px | py) & ~1023) {
      tile = 0;
    } else {
      px &= 1023;
      py &= 1023;
      tile = bit8(ppu.vram[(py >> 3) * 128 + (px >> 3)],0);
    }
    palette = bit8(ppu.vram[(tile << 6) + ((py & 7) << 3) + (px & 7)],1);
    break;
  }

  uint priority;
  if(id == ID::BG1) {
    priority = io.priority[0];
  } else if(id == ID::BG2) {
    priority = io.priority[bool(palette & 0x80)];
    palette &= 0x7f;
  }

  if(palette == 0) return;

  if(io.aboveEnable) {
    output.above.palette = palette;
    output.above.priority = priority;
    output.above.tile = 0;
  }

  if(io.belowEnable) {
    output.below.palette = palette;
    output.below.priority = priority;
    output.below.tile = 0;
  }
}
