
void PPU::renderBgs(unsigned y) {
  unsigned bpp[4] = {};
  unsigned mapw[4], maph[4];  // tiled dimensions
  unsigned affw[4], affh[4];  // Affine dimensions
  unsigned bitw[4], bith[4];  // Bitmap dimensions
  
  for(unsigned n = 0; n < 4; n++) {
    // Tiled dimensions
    mapw[n]   = bg[n].size & 1? 64 : 32;
    maph[n]   = bg[n].size & 2? 64 : 32;
    
    // Affine settings
    affw[n]   =  16 << bg[n].size;
    affh[n]   =  16 << bg[n].size;
    bitw[n]   = 128 << (bg[n].size - (bg[n].size >= 3));  // 128,256,512,512
    bith[n]   = 128 << (bg[n].size - (bg[n].size >= 2));  // 128,256,256,512
    bpp[n]    =   8 << (bg[n].tiles & 1);
  }
  
  if(video.line == 0) {
    bg[2].linex = bg[2].originx;
    bg[2].liney = bg[2].originy;
    bg[3].linex = bg[3].originx;
    bg[3].liney = bg[3].originy;
  }
  
  if(bgMode == 6) {
    // Large 512 x 1024 x 8bpp bitmap - uses all 512K BG
    //   Supposedly 3D is still available on BG0, though
    //   you'd have no VRAM leftover for textures...
    bitw[2] =  512 << (bg[2].size & 1);
    bith[2] = 1024 >> (bg[2].size & 1);
    
    renderBitmapBg(2, 8, bitw[2], bith[2], y);
  }
  else {
    // BG0 is either tiled or 3D render output
    // BG1 is always tiled
    if(bg0FromGPU == 0)  renderTiledBg (0,         mapw[0], maph[0], y);
                         renderTiledBg (1,         mapw[1], maph[1], y);
    
    // BG2-3 are a selectable mix of:
    // - Tiled: 16-bit screen blocks with tile attributes
    // - GBA:   8-bit affine maps, only tile number
    // - NDS:   16-bit affine maps with tile attributes;
    //          these also work as 8/16-bpp bitmaps
    //
    if(bgMode == 0) {      // 2 x tiled
                           renderTiledBg (2,         mapw[2], maph[2], y);
                           renderTiledBg (3,         mapw[3], maph[3], y);
    }
    if(bgMode == 1) {      // Tiled + GBA
                           renderTiledBg (2,         mapw[2], maph[2], y);
                           renderAffineBg(3,      8, affw[3], affh[3], y);
    }
    if(bgMode == 2) {      // 2 x GBA
                           renderAffineBg(2,      8, affw[2], affh[2], y);
                           renderAffineBg(3,      8, affw[3], affh[3], y);
    }
    if(bgMode == 3) {      // Tiled + NDS
                           renderTiledBg (2,         mapw[2], maph[2], y);
      if(bg[3].depth == 0) renderAffineBg(3,     16, affw[3], affh[3], y);
      if(bg[3].depth == 1) renderBitmapBg(3, bpp[3], bitw[3], bith[3], y);
    }
    if(bgMode == 4) {      // GBA + NDS
                           renderAffineBg(2,      8, affw[2], affh[2], y);
      if(bg[3].depth == 0) renderAffineBg(3,     16, affw[3], affh[3], y);
      if(bg[3].depth == 1) renderBitmapBg(3, bpp[3], bitw[3], bith[3], y);
    }
    if(bgMode == 5) {      // 2 x NDS
      if(bg[2].depth == 0) renderAffineBg(2,     16, affw[2], affh[2], y);
      if(bg[3].depth == 0) renderAffineBg(3,     16, affw[3], affh[3], y);
      if(bg[2].depth == 1) renderBitmapBg(2, bpp[2], bitw[2], bith[2], y);
      if(bg[3].depth == 1) renderBitmapBg(3, bpp[3], bitw[3], bith[3], y);
    }
  }
  
  bg[2].linex += bg[2].transform.dx_dv;
  bg[2].liney += bg[2].transform.dy_dv;
  bg[3].linex += bg[3].transform.dx_dv;
  bg[3].liney += bg[3].transform.dy_dv;
}

void PPU::renderTiledBg(unsigned no, unsigned mapw, unsigned maph, unsigned y) {
  auto &bg  = this->bg[no];
  if(bg.enable == false)
    return;
  
  uint32   flags = pxPriority*bg.priority + pxLayer*(2u+no);
  unsigned line  = bg.voffs + y;
  unsigned finex = bg.hoffs % 8;
  auto     above = &this->above[8 - finex];
  auto     below = &this->below[8 - finex];
  
  if(blendAbove & 1<<no) flags |= pxBlendAbove;
  if(blendBelow & 1<<no) flags |= pxBlendBelow;
  
  unsigned mapx  = bg.hoffs / 8;
  unsigned mapy  = line / 8;
  unsigned maddr = 0x8000*bgMapBase + 32*32*bg.map + 32*(mapy%32) + (mapx%32);
  
  if(maph > 32 && (mapy & 32)) maddr += 32*mapw;
  if(mapw > 32 && (mapx & 32)) maddr += 32*32;
  
  bool     ext   = bg.depth && bgLargePal;
  unsigned depth = bg.depth? 0x100 : 0x10;
  unsigned imask = depth-1;
  
  unsigned extpalbase = 0x1000*no;
  if(bg.affineWrap)        // special case for BG0/BG1 - this bit
    extpalbase |= 0x2000;  //   makes them share BG2/BG3's palettes
  
  for(unsigned x = 0; x < 256+8; ) {
    uint16 attr = system.vmap.bg(which, 2*maddr>>14)[2*maddr];
    int    pal  = (attr>>12)*depth + extpalbase;
    uint1 vflip = attr>>11;
    uint1 hflip = attr>>10;
    uint10 tile = attr;
    
    int row = line%8;
    if(vflip) row ^= 7;
    
    uint32 taddr = 0x10000*bgTileBase + 0x4000*bg.tiles + ((32*tile + 4*row) << bg.depth);
    auto   tref  = &system.vmap.bg(which, taddr>>14)[taddr];
    
    uint64 slice = 0;
    for(int n = 3; n >= 0; --n)
      slice = slice<<16 | tref[n];
    
    int dir = +1;
    if(hflip) dir = -1, x += 7;
    
    for(unsigned n = 0; n < 8; n++, x += dir) {
      if(window[x + 8 - finex] & 1<<no) {
        if(unsigned index = slice & imask) {
          uint32 bgr = bgPal[pal+index & 0xff];
          if(ext) {
            bgr = system.vmap.bgpal[which][pal+index >> 13][pal+index << 1];
            bgr = (bgr<<3 & 0760000) | (bgr<<2 & 0007600) | (bgr<<1 & 0000076);
          }
               if(flags < above[x]) below[x] = above[x], above[x] = flags + bgr;
          else if(flags < below[x]) below[x]                      = flags + bgr;
        }
      }
      slice >>= (4 << bg.depth);
    }
    
    if(hflip) x += 9;
    
    if((++maddr & 31) == 0) {
      maddr -= 32;
      if(mapw > 32)  // wrap to next screen
        maddr += mapx & 32? -32*32 : +32*32;
    }
  }
}

void PPU::renderAffineBg(unsigned no, unsigned mapDepth, unsigned mapW, unsigned mapH, unsigned y) {
  auto &bg  = this->bg[no];
  if(bg.enable == false)
    return;
  
  bool   ext   = mapDepth==16 && bgLargePal;
  uint32 flags = pxPriority*bg.priority + pxLayer*(2+no);
  int32  fx    = bg.linex;
  int32  fy    = bg.liney;
  
  if(blendAbove & 1<<no) flags |= pxBlendAbove;
  if(blendBelow & 1<<no) flags |= pxBlendBelow;
  
  for(unsigned x = 8; x < 256+8; x++) {
    if(window[x] & 1<<no) {
      unsigned mx = fx >> 8;
      unsigned my = fy >> 8;
      
      if(bg.affineWrap)
        mx &= 8*mapW-1, my &= 8*mapH-1;
      
      if(mx < 8*mapW && my < 8*mapH) {
        uint32 addr = 0x10000*bgMapBase + 2*32*32*bg.map + ((mapW*(my/8) + (mx/8)) << (mapDepth == 16));
        uint16 attr = system.vmap.bg(which, addr>>14)[addr];
        
        if(mapDepth == 8) {
          // no special handling - pal, vflip, hflip will all be 0
          attr = attr >> 8*(mx/8 & 1) & 0xff;
        }
        int    pal  = (attr>>12)*0x100 + no*0x1000;
        uint1 vflip = attr>>11;
        uint1 hflip = attr>>10;
        uint10 tile = attr;
        
        int row = my%8 ^ 7*vflip;
        int col = mx%8 ^ 7*hflip;
        
        uint32 taddr = 0x10000*bgTileBase + 0x4000*bg.tiles + 64*tile + 8*row + col;
        uint8  index = system.vmap.bg(which, taddr>>14)[taddr] >> 8*(col & 1);
        
        if(index) {
          uint32 bgr = bgPal[pal+index & 0xff];
          if(ext) {
            bgr = system.vmap.bgpal[which][pal>>13][pal+index << 1];
            bgr = (bgr<<3 & 0760000) | (bgr<<2 & 0007600) | (bgr<<1 & 0000076);
          }
               if(flags < above[x]) below[x] = above[x], above[x] = flags + bgr;
          else if(flags < below[x]) below[x]                      = flags + bgr;
        }
      }
    }
    fx += bg.transform.dx_dh;
    fy += bg.transform.dy_dh;
  }
}

void PPU::renderBitmapBg(unsigned no, unsigned mapDepth, unsigned mapW, unsigned mapH, unsigned y) {
  auto &bg  = this->bg[no];
  if(bg.enable == false)
    return;
  
  uint32 flags = pxPriority*bg.priority + pxLayer*(2+no);
  int32  fx    = bg.linex;
  int32  fy    = bg.liney;
  
  if(blendAbove & 1<<no) flags |= pxBlendAbove;
  if(blendBelow & 1<<no) flags |= pxBlendBelow;
  
  for(unsigned x = 8; x < 256+8; x++) {
    if(window[x] & 1<<no) {
      unsigned mx = fx>>8, my = fy>>8;
      
      if(bg.affineWrap)
        mx &= mapW-1, my &= mapH-1;
      
      if(mx < mapW && my < mapH) {
        uint32 addr = 0x4000*bg.map + ((mapW*my + mx) << (mapDepth == 16));
        uint16 data = system.vmap.bg(which, addr>>14)[addr] >> 8*(addr & 1);
        uint32 bgr;
        
        if(mapDepth== 8) bgr = bgPal[data &= 0xff];
        if(mapDepth==16) bgr = (data<<3&62<<12)|(data<<2&62<<6)|(data<<1&62), data &= 0x8000;
        
        if(data) {
               if(flags < above[x]) below[x] = above[x], above[x] = flags + bgr;
          else if(flags < below[x]) below[x]                      = flags + bgr;
        }
      }
    }
    fx += bg.transform.dx_dh;
    fy += bg.transform.dy_dh;
  }
}
