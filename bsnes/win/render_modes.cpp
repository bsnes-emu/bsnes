void RenderScene16_256x224(void) {
word *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 1;
  for(y=0;y<224;y++) {
    screen = (word*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<256;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene32_256x224(void) {
ulong *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 2;
  for(y=0;y<224;y++) {
    screen = (ulong*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<256;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene16_256x240(void) {
word *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 1;
  for(y=0;y<240;y++) {
    screen = (word*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<256;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene32_256x240(void) {
ulong *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 2;
  for(y=0;y<240;y++) {
    screen = (ulong*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<256;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene16_256x448(void) {
word *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 1;
  for(y=0;y<448;y++) {
    screen = (word*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<256;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene32_256x448(void) {
ulong *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 2;
  for(y=0;y<448;y++) {
    screen = (ulong*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<256;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene16_256x480(void) {
word *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 1;
  for(y=0;y<480;y++) {
    screen = (word*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<256;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene32_256x480(void) {
ulong *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 2;
  for(y=0;y<480;y++) {
    screen = (ulong*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<256;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene16_512x224(void) {
word *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 1;
  for(y=0;y<224;y++) {
    screen = (word*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<512;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene32_512x224(void) {
ulong *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 2;
  for(y=0;y<224;y++) {
    screen = (ulong*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<512;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene16_512x240(void) {
word *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 1;
  for(y=0;y<240;y++) {
    screen = (word*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<512;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene32_512x240(void) {
ulong *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 2;
  for(y=0;y<240;y++) {
    screen = (ulong*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<512;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene16_512x448(void) {
word *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 1;
  for(y=0;y<448;y++) {
    screen = (word*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<512;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene32_512x448(void) {
ulong *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 2;
  for(y=0;y<448;y++) {
    screen = (ulong*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<512;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene16_512x480(void) {
word *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 1;
  for(y=0;y<480;y++) {
    screen = (word*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<512;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}

void RenderScene32_512x480(void) {
ulong *screen;
word *src;
word pitch;
HRESULT hr;
int x, y;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  pitch  = ddsd.lPitch >> 2;
  for(y=0;y<480;y++) {
    screen = (ulong*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<512;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
  lpddsb->Unlock(0);
}
