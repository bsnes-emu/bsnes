bool VideoDD::update_video_profile() {
  term();

  update_video_settings();

  DirectDrawCreate(0, &lpdd, 0);
  lpdd->QueryInterface(IID_IDirectDraw7, (void**)&lpdd7);
  SafeRelease(lpdd);

  if(settings.fullscreen == true) {
    lpdd7->SetCooperativeLevel(hwnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
    lpdd7->SetDisplayMode(settings.resolution_width, settings.resolution_height, 16, settings.refresh_rate, 0);
  } else {
    lpdd7->SetCooperativeLevel(hwnd, DDSCL_NORMAL);
  }

  create_presentation();

  lpdd7->CreateClipper(0, &clipper, 0);
  clipper->SetHWnd(0, hwnd);
  screen->SetClipper(clipper);

  create_render_target();
  clear_video();
  return true;
}

void VideoDD::create_render_target() {
int depth;
  screen->GetSurfaceDesc(&ddsd);
  depth = ddsd.ddpfPixelFormat.dwRGBBitCount;
  if(depth == 15 || depth == 16 || (bool)config::video.use_vram == false) {
    goto try_native_surface;
  }

  memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
  ddsd.dwSize = sizeof(DDSURFACEDESC2);
  ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
  if(bool(config::video.use_vram) == true) {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
  } else {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
  }

  ddsd.dwWidth  = 1024;
  ddsd.dwHeight = 1024;

  ddsd.ddpfPixelFormat.dwSize        = sizeof(DDPIXELFORMAT);
  ddsd.ddpfPixelFormat.dwFlags       = DDPF_RGB;
  ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
  ddsd.ddpfPixelFormat.dwRBitMask    = 0xf800;
  ddsd.ddpfPixelFormat.dwGBitMask    = 0x07e0;
  ddsd.ddpfPixelFormat.dwBBitMask    = 0x001f;

  if(lpdd7->CreateSurface(&ddsd, &surface, 0) == DD_OK)goto clear_surface;

try_native_surface:
  memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
  ddsd.dwSize = sizeof(DDSURFACEDESC2);
  ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
  if((bool)config::video.use_vram == true) {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
  } else {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
  }

  ddsd.dwWidth  = 1024;
  ddsd.dwHeight = 1024;

  lpdd7->CreateSurface(&ddsd, &surface, 0);

clear_surface:
  if(!surface) {
    alert("Error: Failed to create DirectDraw backbuffer, cannot continue");
    exit(0);
  }

DDBLTFX fx;
  fx.dwSize = sizeof(DDBLTFX);
  fx.dwFillColor = 0x00000000;
  surface->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
}

bool VideoDD::lock(uint16 *&data, uint &pitch) {
  if(surface->Lock(0, &ddsd, DDLOCK_WAIT, 0) != DD_OK)return 0;
  pitch = ddsd.lPitch;
  data = (uint16*)ddsd.lpSurface;
  return data;
}

void VideoDD::unlock() {
  surface->Unlock(0);
}

void VideoDD::clear_video() {
DDBLTFX fx;
  fx.dwSize = sizeof(DDBLTFX);
  fx.dwFillColor = 0x00000000;
  screen->Blt    (0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
  backbuffer->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
  surface->Blt   (0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);

  if(settings.fullscreen == true) {
    screen->Flip(0, DDFLIP_WAIT);
    backbuffer->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
    screen->Flip(0, DDFLIP_WAIT);
  }
}

//create screen and backbuffer surfaces
void VideoDD::create_presentation() {
  memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
  ddsd.dwSize = sizeof(DDSURFACEDESC2);

  if(settings.fullscreen == true) {
    ddsd.dwFlags           = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.dwBackBufferCount = (settings.triple_buffering == true) ? 2 : 1;
    ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    lpdd7->CreateSurface(&ddsd, &screen, 0);

    memset(&ddscaps, 0, sizeof(DDSCAPS2));
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    screen->GetAttachedSurface(&ddscaps, &backbuffer);
  } else {
    ddsd.dwFlags        = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    lpdd7->CreateSurface(&ddsd, &screen, 0);

    memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    if(bool(config::video.use_vram) == true) {
      ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
    } else {
      ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
    }

    ddsd.dwWidth  = settings.render_width;
    ddsd.dwHeight = settings.render_height;
    lpdd7->CreateSurface(&ddsd, &backbuffer, 0);
  }
}

void VideoDD::redraw() {
HRESULT hr;
RECT rd, rs;
  snes.get_video_info(&vi);
  SetRect(&rs, 0, 0, vi.width, vi.height);

  if(settings.fullscreen == true) {
    SetRect(&rd, settings.rx, settings.ry, settings.rx + settings.rw, settings.ry + settings.rh);
    backbuffer->Blt(&rd, surface, &rs, DDBLT_WAIT, 0);
    hr = screen->Flip(0, DDFLIP_WAIT);
  } else {
  POINT p = { 0, 0 };
    ClientToScreen(hwnd, &p);
    GetClientRect(hwnd, &rd);
    OffsetRect(&rd, p.x, p.y);

    hr = screen->Blt(&rd, surface, &rs, DDBLT_WAIT, 0);
  }

  if(hr == DDERR_SURFACELOST) {
    screen->Restore();
    backbuffer->Restore();
    surface->Restore();
  }
}

void VideoDD::update() {
  redraw();
}

void VideoDD::init() {}

void VideoDD::term() {
  SafeRelease(clipper);
  SafeRelease(surface);
  SafeRelease(backbuffer);
  SafeRelease(screen);
  SafeRelease(lpdd7);
  SafeRelease(lpdd);
}

VideoDD::VideoDD(HWND handle) {
  hwnd = handle;

  lpdd       = 0;
  lpdd7      = 0;
  screen     = 0;
  backbuffer = 0;
  surface    = 0;
  clipper    = 0;
}
