DDRenderer::DDRenderer() {
  lpdd    = 0;
  lpdds   = 0;
  lpddsb  = 0;
  lpddc   = 0;
}

void DDRenderer::update_color_lookup_table() {
int i, r, g, b;
  lpddsb->GetSurfaceDesc(&ddsd);
  color_depth = ddsd.ddpfPixelFormat.dwRGBBitCount;
  if(color_depth == 15) {
    for(i=0;i<65536;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;
      color_lookup_table[i] = (r << 10) | (g << 5) | (b);
    }
  } else if(color_depth == 16) {
    for(i=0;i<65536;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;
      g = (g << 1) | (g >> 4);
      color_lookup_table[i] = (r << 11) | (g << 5) | (b);
    }
  } else if(color_depth == 32) {
    for(i=0;i<65536;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;
      r = (r << 3) | (r >> 2);
      g = (g << 3) | (g >> 2);
      b = (b << 3) | (b >> 2);
      color_lookup_table[i] = (r << 16) | (g << 8) | (b);
    }
  } else {
    alert("Error: Unsupported color depth [%d]", color_depth);
  }
}

void DDRenderer::set_window(HWND hwnd_handle) { hwnd = hwnd_handle; }

/*
  This function tries to create a 16-bit backbuffer whenever possible,
  even in 24/32-bit mode. This is possible because DDraw automatically
  handles conversion from 16bpp->32bpp in hardware. This only works
  when both the source and dest buffers are in VRAM, though.
*/
void DDRenderer::create_backbuffer() {
int color_depth;
  lpdds->GetSurfaceDesc(&ddsd);
  color_depth = ddsd.ddpfPixelFormat.dwRGBBitCount;
  if(color_depth == 15 || color_depth == 16) {
    goto try_native_backbuffer;
  } else {
    if(cfg.video.use_vram == false) {
      goto try_native_backbuffer;
    }
  }

  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof(DDSURFACEDESC);
  ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
  if(cfg.video.use_vram) {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
  } else {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
  }

  ddsd.dwWidth  = 512;
  ddsd.dwHeight = 478;

  ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
  ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
  ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
  ddsd.ddpfPixelFormat.dwRBitMask = 0xf800;
  ddsd.ddpfPixelFormat.dwGBitMask = 0x07e0;
  ddsd.ddpfPixelFormat.dwBBitMask = 0x001f;

  if(lpdd->CreateSurface(&ddsd, &lpddsb, 0) == DD_OK)return;

try_native_backbuffer:
  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof(DDSURFACEDESC);
  ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
  if(cfg.video.use_vram) {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
  } else {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
  }

  ddsd.dwWidth  = 512;
  ddsd.dwHeight = 478;

  lpdd->CreateSurface(&ddsd, &lpddsb, 0);
}

void DDRenderer::to_windowed() {
  destroy();
  DirectDrawCreate(0, &lpdd, 0);
  lpdd->SetCooperativeLevel(hwnd, DDSCL_NORMAL);

  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof(DDSURFACEDESC);
  ddsd.dwFlags = DDSD_CAPS;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
  lpdd->CreateSurface(&ddsd, &lpdds, 0);

  lpdd->CreateClipper(0, &lpddc, 0);
  lpddc->SetHWnd(0, hwnd);
  lpdds->SetClipper(lpddc);

  create_backbuffer();
  update_color_lookup_table();
  update();
}

void DDRenderer::to_fullscreen() {
  destroy();
  DirectDrawCreate(0, &lpdd, 0);
  lpdd->SetCooperativeLevel(hwnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
  lpdd->SetDisplayMode(640, 480, 16);

  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof(DDSURFACEDESC);
  ddsd.dwFlags = DDSD_CAPS;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
  lpdd->CreateSurface(&ddsd, &lpdds, 0);

  lpdd->CreateClipper(0, &lpddc, 0);
  lpddc->SetHWnd(0, hwnd);
  lpdds->SetClipper(lpddc);

  create_backbuffer();
  update_color_lookup_table();
  update();
}

void DDRenderer::set_source_window(RECT *rs) {
  switch(ppu->output->frame_mode) {
  case 0:SetRect(rs, 0, 0, 256, 223);break;
  case 1:SetRect(rs, 0, 0, 256, 446);break;
  case 2:SetRect(rs, 0, 0, 512, 223);break;
  case 3:SetRect(rs, 0, 0, 512, 446);break;
  }
}

void DDRenderer::redraw() {
RECT rd, rs;
POINT p;
HRESULT hr;
  p.x = p.y = 0;
  ClientToScreen(hwnd, &p);
  GetClientRect(hwnd, &rd);
  OffsetRect(&rd, p.x, p.y);
  set_source_window(&rs);
  hr = lpdds->Blt(&rd, lpddsb, &rs, DDBLT_WAIT, 0);
  if(hr == DDERR_SURFACELOST) {
    lpdds->Restore();
    lpddsb->Restore();
  }

  if(cfg.gui.show_fps == false)return;
uint32 fps;
char s[256], t[256];
  fps_timer->tick();
  if(fps_timer->second_passed() == true) {
    sprintf(s, "bsnes v" BSNES_VERSION " ~byuu");
    if(rom_image->loaded() == true) {
      fps = fps_timer->get_ticks();
      if(w_main->frameskip == 0) {
        sprintf(t, " : %d fps", fps);
      } else {
        sprintf(t, " : %d fps [fs: %d]", fps * (1 + w_main->frameskip), w_main->frameskip);
      }
      strcat(s, t);
    }
    SetWindowText(w_main->hwnd, s);
    fps_timer->reset();
  }
}

void DDRenderer::update16() {
HRESULT hr;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;

uint16 *src  = (uint16*)ppu->output->buffer;
uint16 *dest = (uint16*)ddsd.lpSurface;
uint32 pitch;
int x, y;
/* skip first scanline */
  src  += 1024;

  if(clock->overscan() == true) {
    if(ppu->output->frame_mode & PPUOutput::INTERLACE) {
      src += 1024 * 16;
    } else {
      src += 1024 *  8;
    }
  }

  if(ppu->output->frame_mode == PPUOutput::NORMAL) {
  /* 256x223 */
    pitch = (ddsd.lPitch >> 1) - 256;
    y = 223;
    while(y--) {
      x = 256;
      while(x--) {
        *dest++ = color_lookup_table[*src];
        src += 2;
      }
      dest += pitch;
      src  += 512;
    }
  } else if(ppu->output->frame_mode == PPUOutput::INTERLACE) {
  /* 256x446 */
    pitch = (ddsd.lPitch >> 1) - 256;
    y = 446;
    while(y--) {
      x = 256;
      while(x--) {
        *dest++ = color_lookup_table[*src];
        src += 2;
      }
      dest += pitch;
    }
  } else if(ppu->output->frame_mode == PPUOutput::DOUBLEWIDTH) {
  /* 512x223 */
    pitch = (ddsd.lPitch >> 1) - 512;
    y = 223;
    while(y--) {
      x = 512;
      while(x--) {
        *dest++ = color_lookup_table[*src++];
      }
      dest += pitch;
      src  += 512;
    }
  } else {
  /* 512x446 */
    pitch = (ddsd.lPitch >> 1) - 512;
    y = 446;
    while(y--) {
      x = 512;
      while(x--) {
        *dest++ = color_lookup_table[*src++];
      }
      dest += pitch;
    }
  }
  lpddsb->Unlock(0);
}

void DDRenderer::update32() {
HRESULT hr;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;

uint16 *src  = (uint16*)ppu->output->buffer;
uint32 *dest = (uint32*)ddsd.lpSurface;
uint32 pitch;
int x, y;
/* skip first scanline */
  src  += 1024;

  if(clock->overscan() == true) {
    if(ppu->output->frame_mode & PPUOutput::INTERLACE) {
      src += 1024 * 16;
    } else {
      src += 1024 *  8;
    }
  }

  if(ppu->output->frame_mode == PPUOutput::NORMAL) {
  /* 256x223 */
    pitch = (ddsd.lPitch >> 2) - 256;
    y = 223;
    while(y--) {
      x = 256;
      while(x--) {
        *dest++ = color_lookup_table[*src];
        src += 2;
      }
      dest += pitch;
      src  += 512;
    }
  } else if(ppu->output->frame_mode == PPUOutput::INTERLACE) {
  /* 256x446 */
    pitch = (ddsd.lPitch >> 2) - 256;
    y = 446;
    while(y--) {
      x = 256;
      while(x--) {
        *dest++ = color_lookup_table[*src];
        src += 2;
      }
      dest += pitch;
    }
  } else if(ppu->output->frame_mode == PPUOutput::DOUBLEWIDTH) {
  /* 512x223 */
    pitch = (ddsd.lPitch >> 2) - 512;
    y = 223;
    while(y--) {
      x = 512;
      while(x--) {
        *dest++ = color_lookup_table[*src++];
      }
      dest += pitch;
      src  += 512;
    }
  } else {
  /* 512x446 */
    pitch = (ddsd.lPitch >> 2) - 512;
    y = 446;
    while(y--) {
      x = 512;
      while(x--) {
        *dest++ = color_lookup_table[*src++];
      }
      dest += pitch;
    }
  }
  lpddsb->Unlock(0);
}

void DDRenderer::update() {
  switch(color_depth) {
  case 15:
  case 16:
    update16();
    break;
  case 32:
    update32();
    break;
  }
  redraw();
}

void DDRenderer::destroy() {
  if(lpddc) {
    lpddc->Release();
    lpddc = 0;
  }
  if(lpddsb) {
    lpddsb->Release();
    lpddsb = 0;
  }
  if(lpdds) {
    lpdds->Release();
    lpdds = 0;
  }
  if(lpdd) {
    lpdd->Release();
    lpdd = 0;
  }
}
