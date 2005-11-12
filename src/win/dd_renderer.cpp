DDRenderer::DDRenderer() {
  lpdd    = 0;
  lpdd7   = 0;
  lpdds   = 0;
  lpddsb  = 0;
  lpddc   = 0;
}

void DDRenderer::set_window(HWND hwnd_handle) { hwnd = hwnd_handle; }

/*
  This function tries to create a 16-bit backbuffer whenever possible,
  even in 24/32-bit mode. This is possible because DDraw automatically
  handles conversion from 16bpp->32bpp in hardware. This only works
  when both the source and dest buffers are in VRAM, though.

  The SNES resolution is 256x224. Hires mode doubles the screen width, and
  hires+interlace   double the screen height, making the resolution 512x448.
  There is one more problem, however. On some video cards, when blitting
  from video memory from 256x224->512x448, a bilinear filter is applied by
  the video card, and sometimes this filter tries to read past the source
  video memory to get interpolation data. This results in a line of garble
  on the right and bottom edges of the screen. Therefore, an additional
  4 pixels in each direction is added to the backbuffer.
  The backbuffer is thusly 512+4 * 480+4
*/
void DDRenderer::create_backbuffer() {
int depth;
  lpdds->GetSurfaceDesc(&ddsd);
  depth = ddsd.ddpfPixelFormat.dwRGBBitCount;
  if(depth == 15 || depth == 16) {
    goto try_native_backbuffer;
  } else {
    if((int)config::video.use_vram == false) {
      goto try_native_backbuffer;
    }
  }

  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof(DDSURFACEDESC);
  ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
  if(config::video.use_vram) {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
  } else {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
  }

  ddsd.dwWidth  = 512 + 4;
  ddsd.dwHeight = 480 + 4;

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
  if(config::video.use_vram) {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
  } else {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
  }

  ddsd.dwWidth  = 512 + 4;
  ddsd.dwHeight = 480 + 4;

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

  lpddsb->GetSurfaceDesc(&ddsd);
  color_depth = ddsd.ddpfPixelFormat.dwRGBBitCount;
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
  redraw();
}

void DDRenderer::set_source_window() {
//skip first scanline
int i = (vi.height == 224) ? 1 : 2;
  SetRect(&lpddrc, 0, i, vi.width, vi.height);
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

  if(config::video.vblank) {
    lpdd->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);
  }

  hr = lpdds->Blt(&rd, lpddsb, &lpddrc, DDBLT_WAIT, 0);
  if(hr == DDERR_SURFACELOST) {
    lpdds->Restore();
    lpddsb->Restore();
  }
}

uint16 *DDRenderer::lock(uint32 &pitch) {
  if(lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0) != DD_OK)return 0;
  pitch = ddsd.lPitch;
  return (uint16*)ddsd.lpSurface;
}

void DDRenderer::unlock() {
  lpddsb->Unlock(0);
}

void DDRenderer::update() {
  snes->get_video_info(&vi);
  set_source_window();
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
