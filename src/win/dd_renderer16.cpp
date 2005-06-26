inline void DDRenderer::update16_256x224() {
uint16 *src;
uint16 *dest;
uint32 pitch;
int x, y;
  src   = (uint16*)ppu->output->buffer + (1 << 10);
  dest  = (uint16*)ddsd.lpSurface;
  pitch = (ddsd.lPitch >> 1) - 256;

int overscan_adjust = 0;
  if(clock->overscan() == true) {
    src += 8 << 10;
    overscan_adjust = 8;
  }

  for(y=1+overscan_adjust;y<224+overscan_adjust;y++) {
    x = 256;
    while(x--) {
      *dest++ = color_lookup_table[*src];
      src += 2;
    }
    dest += pitch;
    src  += 512;
  }
}

inline void DDRenderer::update16_256x448() {
uint16 *src;
uint16 *dest;
uint32 pitch;
int x, y;
  src   = (uint16*)ppu->output->buffer + (1 << 10);
  dest  = (uint16*)ddsd.lpSurface;
  pitch = (ddsd.lPitch >> 1) - 256;

int overscan_adjust = 0;
  if(clock->overscan() == true) {
    src += 8 << 10;
    overscan_adjust = 16;
  }

  for(y=2+overscan_adjust;y<448+overscan_adjust;y++) {
    x = 256;
    while(x--) {
      *dest++ = color_lookup_table[*src];
      src += 2;
    }
    dest += pitch;
    if(ppu->output->line[y >> 1].interlace == false) {
      src += (y & 1)?512:-512;
    }
  }
}

inline void DDRenderer::update16_512x224() {
uint16 *src;
uint16 *dest;
uint32 pitch;
int x, y;
  src   = (uint16*)ppu->output->buffer + (1 << 10);
  dest  = (uint16*)ddsd.lpSurface;
  pitch = (ddsd.lPitch >> 1) - 512;

int overscan_adjust = 0;
  if(clock->overscan() == true) {
    src += 8 << 10;
    overscan_adjust = 8;
  }

  for(y=1+overscan_adjust;y<224+overscan_adjust;y++) {
    if(ppu->output->line[y].hires == true) {
      x = 512;
      while(x--) {
        *dest++ = color_lookup_table[*src++];
      }
    } else {
      x = 256;
      while(x--) {
        *dest++ = color_lookup_table[*src];
        *dest++ = color_lookup_table[*src];
        src += 2;
      }
    }
    dest += pitch;
    src  += 512;
  }
}

inline void DDRenderer::update16_512x448() {
uint16 *src;
uint16 *dest;
uint32 pitch;
int x, y;
  src   = (uint16*)ppu->output->buffer + (1 << 10);
  dest  = (uint16*)ddsd.lpSurface;
  pitch = (ddsd.lPitch >> 1) - 512;

int overscan_adjust = 0;
  if(clock->overscan() == true) {
    src += 8 << 10;
    overscan_adjust = 16;
  }

  for(y=2+overscan_adjust;y<448+overscan_adjust;y++) {
    if(ppu->output->line[y >> 1].hires == true) {
      x = 512;
      while(x--) {
        *dest++ = color_lookup_table[*src++];
      }
    } else {
      x = 256;
      while(x--) {
        *dest++ = color_lookup_table[*src];
        *dest++ = color_lookup_table[*src];
        src += 2;
      }
    }
    dest += pitch;
    if(ppu->output->line[y >> 1].interlace == false) {
      src += (y & 1)?512:-512;
    }
  }
}
