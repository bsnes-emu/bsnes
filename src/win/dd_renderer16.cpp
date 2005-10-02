inline void DDRenderer::update16_256x224() {
uint16 *src, *dest;
uint32 pitch;
int x, y;
  dest = (uint16*)ddsd.lpSurface;
  src  = (uint16*)vi.data;

#ifdef USE_X86_ASM
  pitch = (ddsd.lPitch) - 512;
  __asm {
    mov edi,dest
    mov esi,src
    mov edx,224
  ly:
    mov ecx,32
  lx:
    movsd
    movsd
    movsd
    movsd
    loopnz lx
    add edi,pitch
    dec edx
    jnz ly
  }
#else
  pitch = (ddsd.lPitch >> 1);
  for(y=0;y<224;y++) {
    memcpy(dest, src, 512);
    dest += pitch;
    src  += 256;
  }
#endif
}

inline void DDRenderer::update16_512x224() {
uint16 *src, *dest;
uint32 pitch;
int x, y;
  pitch = (ddsd.lPitch >> 1);
  dest  = (uint16*)ddsd.lpSurface;
  src   = (uint16*)vi.data;

  for(y=0;y<224;y++) {
    memcpy(dest, src, 1024);
    dest += pitch;
    src  += 512;
  }
}

inline void DDRenderer::update16_256x448() {
uint16 *src, *dest;
uint32 pitch;
int x, y;
  pitch = (ddsd.lPitch >> 1);
  dest  = (uint16*)ddsd.lpSurface;
  src   = (uint16*)vi.data;

  for(y=0;y<448;y++) {
    memcpy(dest, src, 512);
    dest += pitch;
    src  += 256;
  }
}

inline void DDRenderer::update16_512x448() {
uint16 *src, *dest;
uint32 pitch;
int x, y;
  pitch = (ddsd.lPitch >> 1);
  dest  = (uint16*)ddsd.lpSurface;
  src   = (uint16*)vi.data;

  for(y=0;y<448;y++) {
    memcpy(dest, src, 1024);
    dest += pitch;
    src  += 512;
  }
}
