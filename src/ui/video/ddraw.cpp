void VideoDD::clear_video() {
DDBLTFX fx;
  fx.dwSize = sizeof(DDBLTFX);
  fx.dwFillColor = 0x00000000;
  screen->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
  raster->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
}

bool VideoDD::lock(uint16 *&data, uint &pitch) {
  if(raster->Lock(0, &ddsd, DDLOCK_WAIT, 0) != DD_OK) return false;
  pitch = ddsd.lPitch;
  data  = (uint16*)ddsd.lpSurface;
  return data;
}

void VideoDD::unlock() {
  raster->Unlock(0);
}

void VideoDD::refresh(uint r_width, uint r_height) {
  if(settings.synchronize) {
    for(;;) {
    BOOL in_vblank;
      lpdd7->GetVerticalBlankStatus(&in_vblank);
      if(bool(in_vblank) == true) break;
    //Sleep(1);
    }
  }

HRESULT hr;
RECT rd, rs;
  SetRect(&rs, 0, 0, r_width, r_height);

POINT p = { 0, 0 };
  ClientToScreen(hwnd, &p);
  GetClientRect(hwnd, &rd);
  OffsetRect(&rd, p.x, p.y);

  if(screen->Blt(&rd, raster, &rs, DDBLT_WAIT, 0) == DDERR_SURFACELOST) {
    screen->Restore();
    raster->Restore();
  }
}

void VideoDD::init() {
  term();

//
  DirectDrawCreate(0, &lpdd, 0);
  lpdd->QueryInterface(IID_IDirectDraw7, (void**)&lpdd7);
  safe_release(lpdd);

  lpdd7->SetCooperativeLevel(hwnd, DDSCL_NORMAL);

  memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
  ddsd.dwSize = sizeof(DDSURFACEDESC2);

  ddsd.dwFlags        = DDSD_CAPS;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
  lpdd7->CreateSurface(&ddsd, &screen, 0);

  lpdd7->CreateClipper(0, &clipper, 0);
  clipper->SetHWnd(0, hwnd);
  screen->SetClipper(clipper);
//

  create_raster();
  clear_video();
}

void VideoDD::create_raster() {
int depth;
  screen->GetSurfaceDesc(&ddsd);
  depth = ddsd.ddpfPixelFormat.dwRGBBitCount;
  if(depth == 15 || depth == 16 || config::video.use_vram == false) {
    goto try_native_surface;
  }

  memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
  ddsd.dwSize = sizeof(DDSURFACEDESC2);
  ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN |
    (config::video.use_vram == true ? DDSCAPS_VIDEOMEMORY : DDSCAPS_SYSTEMMEMORY);
  ddsd.dwWidth  = 1024;
  ddsd.dwHeight = 1024;

  ddsd.ddpfPixelFormat.dwSize        = sizeof(DDPIXELFORMAT);
  ddsd.ddpfPixelFormat.dwFlags       = DDPF_RGB;
  ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
  ddsd.ddpfPixelFormat.dwRBitMask    = 0xf800;
  ddsd.ddpfPixelFormat.dwGBitMask    = 0x07e0;
  ddsd.ddpfPixelFormat.dwBBitMask    = 0x001f;

  if(lpdd7->CreateSurface(&ddsd, &raster, 0) == DD_OK) return;

try_native_surface:
  memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
  ddsd.dwSize = sizeof(DDSURFACEDESC2);
  ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN |
    (config::video.use_vram == true ? DDSCAPS_VIDEOMEMORY : DDSCAPS_SYSTEMMEMORY);
  ddsd.dwWidth  = 1024;
  ddsd.dwHeight = 1024;

  lpdd7->CreateSurface(&ddsd, &raster, 0);
}

void VideoDD::term() {
  safe_release(clipper);
  safe_release(raster);
  safe_release(screen);
  safe_release(lpdd7);
  safe_release(lpdd);
}

VideoDD::VideoDD(HWND handle) {
  hwnd = handle;

  lpdd    = 0;
  lpdd7   = 0;
  screen  = 0;
  raster  = 0;
  clipper = 0;
}
