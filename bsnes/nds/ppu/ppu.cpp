#include <nds/nds.hpp>

namespace NintendoDS {

PPU ppu[2];



PPU::PPU() {
}

void PPU::power() {
  forceBlank = false;
  
  bgLargePal = false;
  bg0FromGPU = false;
  bgMode = 0;
  bgTileBase = 0;
  bgMapBase = 0;
  
  for(unsigned n = 0; n < 4; n++) {
    bg[n].enable = false;
    bg[n].mosaic = false;
    bg[n].affineWrap = false;
    bg[n].depth = 0;
    bg[n].priority = 0;
    bg[n].size = 0;
    bg[n].map = 0;
    bg[n].tiles = 0;
    bg[n].palette = 0;
    bg[n].hoffs = 0;
    bg[n].voffs = 0;
    bg[n].originx = 0;
    bg[n].originy = 0;
    bg[n].linex = 0;
    bg[n].liney = 0;
    bg[n].transform.dx_dh = 0;
    bg[n].transform.dy_dh = 0;
    bg[n].transform.dx_dv = 0;
    bg[n].transform.dy_dv = 0;
  }
  objEnable   = false;
  objInHBlank = false;
  objLargePal = false;
  objTileMode = 0;
  objTileStep = 0;
  objBitmapMode = 0;
  objBitmapStep = 0;
  
  for(unsigned n = 0; n < 128; n++) {
    obj[n].kind = objinfo::none;
    obj[n].renderMode = objinfo::normal;
    obj[n].mosaic = false;
    obj[n].depth = 0;
    obj[n].size = 0;
    obj[n].shape = objinfo::square;
    obj[n].transform = 0;
    obj[n].x = 0;
    obj[n].y = 0;
    obj[n].index = 0;
    obj[n].priority = 0;
    obj[n].palette = 0;
  }
  for(unsigned n = 0; n < 32; n++) {
    objTransform[n].dx_dh = 0;
    objTransform[n].dy_dh = 0;
    objTransform[n].dx_dv = 0;
    objTransform[n].dy_dv = 0;
  }
  
  blendMode = 0;
  blendAbove = 0;
  blendBelow = 0;
  blendAf = 0;
  blendBf = 0;
  blendYf = 0;
  
  for(unsigned n = 0; n < 2; n++) {
    winX[n][0] = 0;  winX[n][1] = 0xff;
    winY[n][0] = 0;  winY[n][1] = 0xff;
  }
  winArea[0] = 0;  winEnable[0] = false;
  winArea[1] = 0;  winEnable[1] = false;
  winArea[2] = 0;  winEnable[2] = false;
  winArea[3] = 0;
}


void PPU::scanline() {
  if(video.line < 192) {
    // Set up backdrop
    for(unsigned x = 0; x < 256+16; x++) {
      above[x]    = pxPriority*3u + pxLayer*6u + bgPal[0];
      below[x]    = pxPriority*3u + pxLayer*7u;
      objLayer[x] = pxPriority*3u + pxLayer*7u;
      
      if(blendAbove & 1<<5) above[x] |= pxBlendAbove;  // can shade backdrop
      if(blendBelow & 1<<5) above[x] |= pxBlendBelow;  // can blend over backdrop
    }
    
    // Set up window buffer
    bool useWindowing = winEnable[0] || winEnable[1] || winEnable[2];
    
    if(useWindowing) memset(window, 0xc0+winArea[3], 256+16);  // initialize with winout
    else             memset(window, 0x3f,            256+16);  // draw everything
    
    for(int w = 1; w >= 0; w--)
      if(winEnable[w] && winY[w][0] <= video.line && video.line <= winY[w][1] && winX[w][1] > winX[w][0])
        memset(&window[winX[w][0] + 8], 0x40+winArea[w], winX[w][1] - winX[w][0]);
    
    // Draw layers
    if(objEnable)
      renderObjs(video.line);
    
    renderBgs(video.line);
    
    
    // Feed in 3D layer, if requested
    if(bg[0].enable && bg0FromGPU && which == 0) {
      auto *gpuOut = &gpu.output[256*video.line];
      
      uint32 depth = pxPriority*bg[0].priority + pxLayer*2;
      uint32 flags = pxBlendForce;
      
      if(blendAbove & 1<<0) flags |= pxBlendAbove;
      if(blendBelow & 1<<0) flags |= pxBlendBelow;
      
      for(unsigned x = 0; x < 256; x++)  {
        if(!(window[x+8] & 1<<0)) continue;
        if(gpuOut[x] < 1*pxAlpha) continue;
        
        if(depth < above[8+x]) {
          below[8+x] = above[8+x];
          above[8+x] = depth | flags | gpuOut[x];
        }
        else if(depth < below[8+x]) {
          below[8+x] = depth | flags | gpuOut[x];
        }
      }
    }
    
    // Add OBJs, if enabled
    if(objEnable) {
      for(unsigned x = 0; x < 256; x++) {
        if(!(window[x+8] & 1<<4)) continue;
        
        auto &objPx = objLayer[x+8];
        auto &abovePx = above[x+8];
        auto &belowPx = below[x+8];
        
             if(objPx < abovePx) belowPx = abovePx, abovePx = objPx;
        else if(objPx < belowPx) belowPx = objPx;
      }
    }
    
    unsigned yf = min(32, 2*blendYf);
    
    uint64 round = 02000200020;
    uint64 shade = round;
    if(blendMode == lighten)
      shade += 07700770077ull*yf;
    
    for(unsigned x = 0; x < 256; x++) {
      auto &abovePx = above[x+8];
      auto &belowPx = below[x+8];
      
      unsigned af = min(32, 2*blendAf);
      unsigned bf = min(32, 2*blendBf);
      
      bool canShade   = (abovePx &  pxBlendAbove);
      bool canBlend   = (abovePx & (pxBlendAbove|pxBlendForce)) && (belowPx & pxBlendBelow);
      bool forceAlpha = (abovePx &               pxBlendForce ) && (belowPx & pxBlendBelow);
      unsigned mode   = forceAlpha? alphaBlend : blendMode;
      
      if(!(mode && (window[x+8] & 1<<5))) {
        output[x] = abovePx;
        continue;
      }
      
      uint64 a = (abovePx & 0777777)*01000001ull & 07700770077ull;
      uint64 b = (belowPx & 0777777)*01000001ull & 07700770077ull;
      
      if(forceAlpha && (abovePx & 31*pxAlpha))
        af = (abovePx>>18 & 31) + 1,  bf = 32-af;
      
           if(canBlend && mode == alphaBlend) a = (a*af + b*bf + round)/32;
      else if(canShade && mode >  alphaBlend) a = (a*(32 - yf) + shade)/32;
      
      if(uint32 oflow = a & 010001000100)
        a |= oflow - (oflow>>6);
      
      a &= 07700770077;
      output[x] = (a | a>>18) & 0777777;
    }
  }
}


#include "bg.cpp"
#include "obj.cpp"


uint32 PPU::readPalette(uint32 addr) {
  addr &= ~3;
  uint32* p = addr < 0x200? &bgPal[addr>>1 & 0xfe] : &objPal[addr>>1 & 0xfe];
  
  uint5 b0 = p[0]>>13, g0 = p[0]>>7, r0 = p[0]>>1;
  uint5 b1 = p[1]>>13, g1 = p[1]>>7, r1 = p[1]>>1;
  
  return b0<<10 | g0<<5 | r0<<0 | b1<<26 | g1<<21 | r1<<16;
}

void PPU::writePalette(uint32 addr, uint32 size, uint32 data) {
  uint32 mask = 0xffffffff;
  
  if(size == Byte) size = Half;
  if(size == Half) addr &= ~1, mask = 0xffff << 8*(addr & 3);
  if(size == Word) addr &= ~3;
  
  uint32* p = addr < 0x200? &bgPal[addr>>1 & 0xfe] : &objPal[addr>>1 & 0xfe];
  
  uint5 b0 = data>>10, g0 = data>> 5, r0 = data>> 0;
  uint5 b1 = data>>26, g1 = data>>21, r1 = data>>16;
  
  if(mask & 0x0000ffff) p[0] = b0<<13 | g0<<7 | r0<<1;
  if(mask & 0xffff0000) p[1] = b1<<13 | g1<<7 | r1<<1;
}

uint32 PPU::readOam(uint32 addr) {
  addr &= ~3;
  
  objinfo&     obj = this->obj[addr/8];
  affineparam& tf  = this->objTransform[addr/32];
  
  switch(addr & 4) {
  case 0:
    return obj.y<<0 | obj.renderMode<<8 | obj.kind<10
         | obj.mosaic<<12 | obj.depth<<13 | obj.size<<14
         | obj.x<<16 | obj.transform<<25 | obj.shape<<30;
    
  case 4:
    return obj.index<<0 | obj.priority<<10 | obj.palette<<12
         | tf.m[addr>>3 & 3];
  }
}

void PPU::writeOam(uint32 addr, uint32 size, uint32 data) {
  if(size == Byte) return;
  
  if(size == Word) {
    addr &= ~3;
    writeOam(addr+0, Half, data>>0);
    writeOam(addr+2, Half, data>>16);
    return;
  }
  
  objinfo&     obj = this->obj[addr/8];
  affineparam& tf  = this->objTransform[addr/32];
  
  switch(addr & 6) {
  case 0:
    obj.y          = data>>0;
    obj.renderMode = data>>8;
    obj.kind       = data>>10;
    obj.mosaic     = data>>12;
    obj.depth      = data>>13;
    obj.shape      = data>>14;
    return;
    
  case 2:
    obj.x          = data>>0;
    obj.transform  = data>>9;
    obj.size       = data>>14;
    return;
    
  case 4:
    obj.index      = data>>0;
    obj.priority   = data>>10;
    obj.palette    = data>>12;
    return;
    
  case 6:
    tf.m[addr>>3 & 3] = data;
    return;
  }
}



uint32 PPU::regControl() {
  return bgMode<<0 | bg0FromGPU<<3 | forceBlank<<7 | objInHBlank<<23
       | objTileMode<<4 | objBitmapMode<<5 | objTileStep<<20 | objBitmapStep<<22
       | bg[0].enable<<8 | bg[1].enable<<9 | bg[2].enable<<10 | bg[3].enable<<11
       | objEnable<<12 | winEnable[0]<<13 | winEnable[1]<<14 | winEnable[2]<<15
       | bgTileBase<<24 | bgMapBase<<27 | bgLargePal<<30 | objLargePal<<31;
}

void PPU::regControl(uint32 data, uint32 mask) {
  if(mask & 0x000000ff) {
    bgMode          = data>>0;
    bg0FromGPU      = data>>3;
    objTileMode     = data>>4;
    objBitmapMode   = data>>5;
    forceBlank      = data>>7;
  }
  if(mask & 0x0000ff00) {
    bg[0].enable    = data>>8;
    bg[1].enable    = data>>9;
    bg[2].enable    = data>>10;
    bg[3].enable    = data>>11;
    objEnable       = data>>12;
    winEnable[0]    = data>>13;
    winEnable[1]    = data>>14;
    winEnable[2]    = data>>15;
  }
  if(mask & 0x00f00000) {
    objTileStep     = data>>20;
    objBitmapStep   = data>>22;
    objInHBlank     = data>>23;
  }
  if(mask & 0xff000000) {
    bgTileBase      = data>>24;
    bgMapBase       = data>>27;
    bgLargePal      = data>>30;
    objLargePal     = data>>31;
  }
}

uint32 PPU::regBg(unsigned no) {
  return bg[no].priority<<0
       | bg[no].tiles<<2 | bg[no].mosaic<<6 | bg[no].depth<<7
       | bg[no].map<<8 | bg[no].affineWrap<<13 | bg[no].size<<14;
}

void PPU::regBg(unsigned no, uint32 data, uint32 mask) {
  //bg0 = 17 00  map 23, size -, tiles --, pri 0, 3D
  //bg1 = f8 8b  map 24, size 3, tiles 02, pri 3, 256 color
  //bg2 = b0 01  map 16, size 2, tiles 00, pri 1, 16-bit affine
  //bg3 = b4 0a  map 20, size 2, tiles 02, pri 2, 16-bit affine
  if(mask & 0x00ff) {
    bg[no].priority = data>>0;
    bg[no].tiles    = data>>2;
    bg[no].mosaic   = data>>6;
    bg[no].depth    = data>>7;
  }
  if(mask & 0xff00) {
    bg[no].map        = data>>8;
    bg[no].affineWrap = data>>13;
    bg[no].size       = data>>14;
  }
}

uint32 PPU::regBgOffs(unsigned no) {
  return bg[no].hoffs | bg[no].voffs<<16;
}

void PPU::regBgOffs(unsigned no, uint32 data, uint32 mask) {
  bg[no].hoffs ^= (bg[no].hoffs ^ data) & mask; data >>= 16; mask >>= 16;
  bg[no].voffs ^= (bg[no].voffs ^ data) & mask;
}

void PPU::regBgAffine(unsigned no, unsigned index, uint32 data, uint32 mask) {
  if(index < 2) {
    bg[no].transform.m[2*index+0] ^= (bg[no].transform.m[2*index+0] ^ data) & mask; data >>= 16; mask >>= 16;
    bg[no].transform.m[2*index+1] ^= (bg[no].transform.m[2*index+1] ^ data) & mask;
  }
  if(index == 2) {
    bg[no].originx ^= (bg[no].originx ^ data) & mask;
    bg[no].linex    = bg[no].originx;
  }
  if(index == 3) {
    bg[no].originy ^= (bg[no].originy ^ data) & mask;
    bg[no].liney    = bg[no].originy;
  }
}

void PPU::regWinDims(unsigned index, uint32 data, uint32 mask) {
  auto dims = index == 0? winX : winY;
  
  if(mask & 0x000000ff) dims[0][1] = data>>0;
  if(mask & 0x0000ff00) dims[0][0] = data>>8;
  if(mask & 0x00ff0000) dims[1][1] = data>>16;
  if(mask & 0xff000000) dims[1][0] = data>>24;
}

uint32 PPU::regWinArea() {
  return winArea[0] | winArea[1]<<8 | winArea[3]<<16 | winArea[2]<<24;
}

void PPU::regWinArea(uint32 data, uint32 mask) {
  if(mask & 0x0000003f) winArea[0] = data>>0  & 0x3f;
  if(mask & 0x00003f00) winArea[1] = data>>8  & 0x3f;
  if(mask & 0x003f0000) winArea[3] = data>>16 & 0x3f;
  if(mask & 0x3f000000) winArea[2] = data>>24 & 0x3f;
}

uint32 PPU::regBlend() {
  return blendAbove<<0 | blendMode<<6 | blendBelow<<8;
}

void PPU::regBlend(unsigned index, uint32 data, uint32 mask) {
  uint64 maskl = uint64(mask) << 32*index;
  
  if(maskl & 0x000000003full) blendAbove = data>>0;
  if(maskl & 0x00000000c0ull) blendMode  = data>>6;
  if(maskl & 0x0000003f00ull) blendBelow = data>>8;
  if(maskl & 0x00001f0000ull) blendAf    = data>>16;
  if(maskl & 0x001f000000ull) blendBf    = data>>24;
  if(maskl & 0x1f00000000ull) blendYf    = data>>0;
}



}