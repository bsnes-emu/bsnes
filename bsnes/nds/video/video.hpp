struct Video {
  void power();
  void scanline(unsigned y);
  
  uint32 regCapture();
  void regCapture(uint32 data, uint32 mask);
  void regFifo(uint32 data);
  
  uint32 regBrightness(unsigned index);
  void regBrightness(unsigned index, uint32 data, uint32 mask);
  
  uint32 output[256*384];
  
  uint32 blankData[256];
  uint32 frameBufData[256];
  uint32 fifoData[256];
  
  uint9 line;
  Event hdrawEvent;
  Event hblankEvent;
  
  // Display and rendering on NDS are more loosely coupled, although
  // the GPU and PPUs are still all locked to 60fps.
  enum { srcNone, srcPPU, srcFrameBuffer, srcFifo, srcGPU };
  
  // For display onscreen
  uint1 screensPowered;
  uint1 ppu0Screen;  // 0 is bottom, 1 is top
  int   source[2];
  int   fade[2]; enum { lighten=1, darken=2 };
  uint5 blendYf[2];
  
  // For rendering to VRAM
  bool  write;
  int   sourceA;   // PPU0 or GPU
  int   sourceB;   // framebuffer or FIFO
  uint5 blendAf, blendBf;  // for blending A + B
  uint2 targetSize;
  uint2 targetSource;
  uint4 targetBuffer;
  uint4 frameBuffer;
};

extern Video video;
