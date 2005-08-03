DDRenderer::DDRenderer() {
  lpdd    = 0;
  lpdd7   = 0;
  lpdds   = 0;
  lpddsb  = 0;
  lpddc   = 0;

int i, c;
  for(i=0,c=0;i<16;i++) {
    color_curve_table[i] = c;
    c = c + i + 1;
  }
  for(;i<31;i++) {
    color_curve_table[i] = c;
    c += 8;
  }
  color_curve_table[i] = 0xff;
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
      if(cfg.video.color_curve) {
        r = color_curve_table[r] >> 3;
        g = color_curve_table[g] >> 3;
        b = color_curve_table[b] >> 3;
      }
      color_lookup_table[i] = (r << 10) | (g << 5) | (b);
    }
  } else if(color_depth == 16) {
    for(i=0;i<65536;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;
      if(cfg.video.color_curve) {
        r = color_curve_table[r] >> 3;
        g = color_curve_table[g] >> 2;
        b = color_curve_table[b] >> 3;
      } else {
        g = (g << 1) | (g >> 4);
      }
      color_lookup_table[i] = (r << 11) | (g << 5) | (b);
    }
  } else if(color_depth == 32) {
    for(i=0;i<65536;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;
      if(cfg.video.color_curve) {
        r = color_curve_table[r];
        g = color_curve_table[g];
        b = color_curve_table[b];
      } else {
        r = (r << 3) | (r >> 2);
        g = (g << 3) | (g >> 2);
        b = (b << 3) | (b >> 2);
      }
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

  The SNES resolution is 256x224. The top scanline is never drawn, so
  256x223 is used. Hires mode doubles the screen width, and hires+interlace
  double the screen height, making the resolution 512x446.
  There is one more problem, however. On some video cards, when blitting
  from video memory from 256x223->512x446, a bilinear filter is applied by
  the video card, and sometimes this filter tries to read past the source
  video memory to get interpolation data. This results in a line of garble
  on the right and bottom edges of the screen. Therefore, an additional
  4 pixels in each direction is added to the backbuffer.
  The backbuffer is thusly 512+4 * 476+4
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

  ddsd.dwWidth  = 512 + 4;
  ddsd.dwHeight = 476 + 4;

  ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
  ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
  ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
  ddsd.ddpfPixelFormat.dwRBitMask = 0xf800;
  ddsd.ddpfPixelFormat.dwGBitMask = 0x07e0;
  ddsd.ddpfPixelFormat.dwBBitMask = 0x001f;

  if(lpdd->CreateSurface(&ddsd, &lpddsb, 0) == DD_OK)goto clear_backbuffer;

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

  ddsd.dwWidth  = 512 + 4;
  ddsd.dwHeight = 476 + 4;

  lpdd->CreateSurface(&ddsd, &lpddsb, 0);

clear_backbuffer:
  if(!lpddsb) {
    alert("Error: Failed to create DirectDraw backbuffer, cannot continue");
    exit(0);
  }

DDBLTFX fx;
  fx.dwSize = sizeof(DDBLTFX);
  fx.dwFillColor = 0x00000000;
  lpddsb->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
}

void DDRenderer::to_windowed() {
  destroy();
  fullscreen = false;
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
  redraw();
}

void DDRenderer::to_fullscreen(int _width, int _height) {
  destroy();
  fullscreen = true;
  width      = _width;
  height     = _height;
  DirectDrawCreate(0, &lpdd, 0);
/*
  lpdd->QueryInterface(IID_IDirectDraw7, (void**)&lpdd7);
  lpdd7->SetCooperativeLevel(hwnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
  lpdd7->SetDisplayMode(width, height, 16, 60, 0);
*/
  lpdd->SetCooperativeLevel(hwnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
  lpdd->SetDisplayMode(width, height, 16);

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
  redraw();
}

void DDRenderer::set_source_window() {
  SetRect(&lpddrc, 0, 0,
    (ppu->output->hires     == false)?256:512,
    (ppu->output->interlace == false)?223:446);
}

void DDRenderer::redraw() {
RECT rd;
POINT p;
HRESULT hr;
int rx, ry;
  if(fullscreen == false) {
    p.x = p.y = 0;
    ClientToScreen(hwnd, &p);
    GetClientRect(hwnd, &rd);
    OffsetRect(&rd, p.x, p.y);
  } else {
    switch(width) {
    case 640: //x480
      SetRect(&rd, 0, 2, 512, 448);
      OffsetRect(&rd, (640 - 512) / 2, (480 - 446) / 2);
      break;
    case 1024: //x768
      SetRect(&rd, 0, 0, 1024, 768);
      break;
    }
  }
  if(cfg.video.vblank) {
    lpdd->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);
  }
  hr = lpdds->Blt(&rd, lpddsb, &lpddrc, DDBLT_WAIT, 0);
  if(hr == DDERR_SURFACELOST) {
    lpdds->Restore();
    lpddsb->Restore();
  }

  if(cfg.gui.show_fps == false || bsnes->get_status() == bSNES::STOP)return;
uint32 fps;
char s[256], t[256];
  fps_timer->tick();
  if(fps_timer->second_passed() == true) {
    sprintf(s, BSNES_TITLE);
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

#include "dd_renderer16.cpp"
void DDRenderer::update16() {
HRESULT hr;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;

  set_source_window();
  if(ppu->output->hires == false) {
    if(ppu->output->interlace == false) {
      update16_256x224();
    } else {
      update16_256x448();
    }
  } else {
    if(ppu->output->interlace == false) {
      update16_512x224();
    } else {
      update16_512x448();
    }
  }

  lpddsb->Unlock(0);
}

#include "dd_renderer32.cpp"
void DDRenderer::update32() {
HRESULT hr;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;

  set_source_window();
  if(ppu->output->hires == false) {
    if(ppu->output->interlace == false) {
      update32_256x224();
    } else {
      update32_256x448();
    }
  } else {
    if(ppu->output->interlace == false) {
      update32_512x224();
    } else {
      update32_512x448();
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
  if(lpdd7) {
    lpdd7->Release();
    lpdd7 = 0;
  }
  if(lpdd) {
    lpdd->Release();
    lpdd = 0;
  }
}
