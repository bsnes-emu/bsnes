#include <nds/nds.hpp>

namespace NintendoDS {

Video video;

void Video::scanline(unsigned y) {
  gpu.scanline();
  ppu[0].scanline();
  ppu[1].scanline();
  
  if(line < 192) {
    bool display = source[0] == srcFrameBuffer;
    bool read = write && sourceB == srcFrameBuffer;
    
    unsigned srcline  = y + 64*(frameBuffer & (display? 0xc : 0xf));
    unsigned destline = y + 64*targetBuffer;
    
    uint16 *src  = &system.vmap.display(srcline / 32)[512*(srcline % 32)];
    uint16 *dest = &system.vmap.display(destline / 32)[512*(destline % 32)];
    
    if(read || display) {
      for(unsigned x = 0; x < 256; x++) {
        uint16 c = src[x];
        uint5 r = c>>0, g = c>>5, b = c>>10;
        
        frameBufData[x] = b<<13 | g<<7 | r<<1;
      }
    }
    if(write) {
      uint32 *aData = sourceA == srcGPU? &gpu.output[256*line] : ppu[0].output;
      uint32 *bData = sourceB == srcFifo?  fifoData : frameBufData;
      
      unsigned af = min((unsigned)blendAf, 16u);
      unsigned bf = min((unsigned)blendBf, 16u);
      
      if(targetSource == 0) af = 16, bf = 0;
      if(targetSource == 1) af = 0, bf = 16;
      
      // Need to see what happens with alpha bit..
      for(unsigned x = 0; x < 256; x++) {
        uint32 a = aData[x], b = bData[x];
        
        uint6 ar = a>>0, ag = a>>6, ab = a>>12;
        uint6 br = b>>0, bg = b>>6, bb = b>>12;
        
        ar = (ar*af + br*bf + 8) / 32u;
        ag = (ag*af + bg*bf + 8) / 32u;
        ab = (ab*af + bb*bf + 8) / 32u;
        
        dest[x] = 0x8000 | ab<<10 | ag<<5 | ar<<0;
      }
    }
    
    uint32* line[2] = { &output[256*(y + 192*(ppu0Screen == 0))],
                        &output[256*(y + 192*(ppu0Screen == 1))] };
    
    if(source[0] == srcNone)        memcpy(line[0], blankData, 256*4);
    if(source[0] == srcPPU)         memcpy(line[0], ppu[0].output, 256*4);
    if(source[0] == srcFrameBuffer) memcpy(line[0], frameBufData, 256*4);
    if(source[0] == srcFifo)        memcpy(line[0], fifoData, 256*4);
    
    if(source[1] == srcNone)        memcpy(line[1], blankData, 256*4);
    if(source[1] == srcPPU)         memcpy(line[1], ppu[1].output, 256*4);
    if(source[1] == srcFrameBuffer) memcpy(line[1], blankData, 256*4);
    if(source[1] == srcFifo)        memcpy(line[1], blankData, 256*4);
    
    for(unsigned s = 0; s < 2; s++) {
      if(!fade[s] || !blendYf[s]) continue;
      
      uint64 yf = min(32, 2*blendYf[s]);
      uint64 y = 02000200020;
      if(fade[s] == lighten)
        y += 07700770077*yf;
      
      for(unsigned x = 0; x < 256; x++) {
        uint64 bgr = (line[s][x] & 0777777)*01000001ull & 07700770077ull;
        
        bgr = (bgr*(32-yf) + y)/32;
        
        if(bgr & 010000000000) bgr |= 007700000000;
        if(bgr & 000001000000) bgr |= 000000770000;
        if(bgr & 000000000100) bgr |= 000000000077;
        bgr &= 007700770077;
        
        line[s][x] = (bgr | bgr>>18) & 0777777;
      }
    }
  }
}

uint32 Video::regCapture() {
  return blendAf<<0 | blendBf<<8
       | (targetBuffer<<14 & 3<<16)
       | (targetBuffer<<18 & 3<<18)
       | (frameBuffer<<26 & 3<<26)
       | (sourceA == srcGPU? 1<<24 : 0)
       | (sourceB == srcFifo? 1<<25 : 0)
       | targetSource<<29 | write<<31;
}

void Video::regCapture(uint32 data, uint32 mask) {
  if(mask & 0x000000ff) {
    blendAf = data>>0;
  }
  if(mask & 0x0000ff00) {
    blendBf = data>>8;
  }
  if(mask & 0x00ff0000) {
    targetBuffer  = data>>14 & 0xc;
    targetBuffer += data>>18 & 0x3;
    targetSize    = data>>20;
  }
  if(mask & 0xff000000) {
    frameBuffer &= ~3;
    frameBuffer += data>>26 & 0x3;
    sourceA      = data & 1<<24? srcGPU : srcPPU;
    sourceB      = data & 1<<25? srcFifo : srcFrameBuffer;
    targetSource = data>>29;
    write        = data>>31;
  }
}

void Video::regFifo(uint32 data) {
}

uint32 Video::regBrightness(unsigned index) {
  return fade[index]<<14 | blendYf[index]<<0;
}

void Video::regBrightness(unsigned index, uint32 data, uint32 mask) {
  if(mask & 0x001f) blendYf[index] = data>>0;
  if(mask & 0xc000) fade[index]    = data>>14;
}


void Video::power() {
  for(unsigned x = 0; x < 256; x++)
    blankData[x] = 0x3ffff;
  
  line = 262;
  ppu0Screen = 0;
  source[0] = srcNone;
  source[1] = srcNone;
  frameBuffer = 0;
  fade[0] = 0;
  fade[1] = 0;
  blendYf[0] = 0;
  blendYf[1] = 0;
  
  write = false;
  sourceA = srcNone;
  sourceB = srcNone;
  blendAf = blendBf = 0;
  targetBuffer = 0;
  
  hdrawEvent.action = [&]() {
    if(++line == 263) {
      line = 0;
    }
    if(line == 192) {
      write = false;
      system.frame();
    }
    scanline(line);
    arm9.event.queue.add(12*256, hblankEvent);
    arm7.hdraw();
    arm9.hdraw();
  };
  
  hblankEvent.action = [&]() {
    arm9.event.queue.add(12*99, hdrawEvent);
    arm7.hblank();
    arm9.hblank();
  };
  
  arm9.event.queue.add(0, hdrawEvent);
}

}
