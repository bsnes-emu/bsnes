void VideoDD::create_surface(uint8 i) {
int depth;
  lpdds->GetSurfaceDesc(&ddsd);
  depth = ddsd.ddpfPixelFormat.dwRGBBitCount;
  if(depth == 15 || depth == 16 || (bool)config::video.use_vram == false) {
    goto try_native_surface;
  }

  memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
  ddsd.dwSize = sizeof(DDSURFACEDESC2);
  ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
  if((bool)config::video.use_vram == true) {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
  } else {
    ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;
  }

  ddsd.dwWidth  = 512 + 4;
  ddsd.dwHeight = 480 + 4;

  ddsd.ddpfPixelFormat.dwSize        = sizeof(DDPIXELFORMAT);
  ddsd.ddpfPixelFormat.dwFlags       = DDPF_RGB;
  ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
  ddsd.ddpfPixelFormat.dwRBitMask    = 0xf800;
  ddsd.ddpfPixelFormat.dwGBitMask    = 0x07e0;
  ddsd.ddpfPixelFormat.dwBBitMask    = 0x001f;

  if(lpdd7->CreateSurface(&ddsd, &surface[i].lpdds, 0) == DD_OK)goto clear_surface;

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

  ddsd.dwWidth  = 512 + 4;
  ddsd.dwHeight = 480 + 4;

  lpdd7->CreateSurface(&ddsd, &surface[i].lpdds, 0);

clear_surface:
  if(!surface[i].lpdds) {
    alert("Error: Failed to create DirectDraw backbuffer, cannot continue");
    exit(0);
  }

DDBLTFX fx;
  fx.dwSize = sizeof(DDBLTFX);
  fx.dwFillColor = 0x00000000;
  surface[i].lpdds->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);

  surface[i].valid = false;
}

void VideoDD::clear_video() {
DDBLTFX fx;
  fx.dwSize = sizeof(DDBLTFX);
  fx.dwFillColor = 0x00000000;
  lpdds->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
}

void VideoDD::set_mode_windowed() {
  settings.fullscreen = false;
  DirectDrawCreate(0, &lpdd, 0);
  lpdd->QueryInterface(IID_IDirectDraw7, (void**)&lpdd7);
  lpdd->Release();
  lpdd = 0;
  lpdd7->SetCooperativeLevel(wMain.hwnd, DDSCL_NORMAL);
}

void VideoDD::set_mode_fullscreen() {
  settings.fullscreen = true;
  DirectDrawCreate(0, &lpdd, 0);
  lpdd->QueryInterface(IID_IDirectDraw7, (void**)&lpdd7);
  lpdd->Release();
  lpdd = 0;

  lpdd7->SetCooperativeLevel(wMain.hwnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
  lpdd7->SetDisplayMode(settings.width, settings.height, 16, settings.refresh_rate, 0);
}

void VideoDD::set_source_window() {
//skip first scanline
int i = (vi.height == 224) ? 1 : 2;
  SetRect(&lpddrc, 0, i, vi.width, vi.height);
}

void VideoDD::redraw() {
RECT rd;
  if(settings.fullscreen == false) {
  POINT p = { 0, 0 };
    ClientToScreen(wMain.hwnd, &p);
    GetClientRect(wMain.hwnd, &rd);
    OffsetRect(&rd, p.x, p.y);
  } else {
  int rx = (settings.width  - settings.output_width)  >> 1;
  int ry = (settings.height - settings.output_height) >> 1;
    SetRect(&rd, rx, ry, rx + settings.output_width, ry + settings.output_height);
  }

HRESULT hr = lpdds->Blt(&rd, surface[active_surface ^ 1].lpdds, &lpddrc, DDBLT_WAIT, 0);
  if(hr == DDERR_SURFACELOST) {
    lpdds->Restore();
    surface[0].lpdds->Restore();
    surface[1].lpdds->Restore();
  }

  surface[active_surface ^ 1].valid = false;
}

uint16 *VideoDD::lock(uint32 &pitch) {
  if(surface[active_surface].lpdds->Lock(0, &ddsd, DDLOCK_WAIT, 0) != DD_OK)return 0;
  pitch = ddsd.lPitch;
  return (uint16*)ddsd.lpSurface;
}

void VideoDD::unlock() {
  surface[active_surface].lpdds->Unlock(0);
}

/*
  Run periodically to see if the screen needs to be refreshed due to entering
  vblank period. This will only happen when triple buffering is enabled.
*/
void VideoDD::scanline() {
  if((bool)config::video.triple_buffering == false)return;

#if 1
uint32 current_scanline;
  lpdd7->GetScanLine(&current_scanline);

  if(current_scanline >= settings.screen_height && last_scanline < settings.screen_height) {
    if(surface[active_surface ^ 1].valid == true) {
      redraw();
    }
  }

  last_scanline = current_scanline;
#else
BOOL status;
bool vblank_status;
  lpdd7->GetVerticalBlankStatus(&status);
  vblank_status = (status == TRUE);

  if(vblank_status == true && in_vblank == false) {
    if(surface[active_surface ^ 1].valid == true) {
      redraw();
    }
  }

  in_vblank = vblank_status;
#endif
}

void VideoDD::update() {
  snes->get_video_info(&vi);
  set_source_window();

  surface[active_surface].valid = true;
  active_surface ^= 1;

  if((bool)config::video.triple_buffering == false)redraw();
}

bool VideoDD::set_video_mode(int width, int height, bool fullscreen,
int refresh_rate, int output_width, int output_height) {
  settings.width         = width;
  settings.height        = height;
  settings.output_width  = output_width;
  settings.output_height = output_height;
  settings.fullscreen    = fullscreen;
  settings.refresh_rate  = refresh_rate;

  term();

  if(settings.fullscreen) {
    set_mode_fullscreen();
  } else {
    set_mode_windowed();
  }

  wMain.hide();
  wMain.resize(width, height, fullscreen);
  wMain.show();

  settings.screen_height = GetSystemMetrics(SM_CYSCREEN);

  memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
  ddsd.dwSize         = sizeof(DDSURFACEDESC2);
  ddsd.dwFlags        = DDSD_CAPS;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
  lpdd7->CreateSurface(&ddsd, &lpdds, 0);

  lpdd7->CreateClipper(0, &lpddc, 0);
  lpddc->SetHWnd(0, wMain.hwnd);
  lpdds->SetClipper(lpddc);

  active_surface = 0;
  last_scanline  = 0;
  in_vblank      = false;
  create_surface(0);
  create_surface(1);
  clear_video();

  return true;
}

void VideoDD::init() {}

void VideoDD::term() {
  if(lpddc) { lpddc->Release(); lpddc = 0; }
  if(surface[0].lpdds) { surface[0].lpdds->Release(); surface[0].lpdds = 0; }
  if(surface[1].lpdds) { surface[1].lpdds->Release(); surface[1].lpdds = 0; }
  if(lpdds) { lpdds->Release(); lpdds = 0; }
  if(lpdd7) { lpdd7->Release(); lpdd7 = 0; }
  if(lpdd)  { lpdd->Release();  lpdd  = 0; }
}

VideoDD::VideoDD() {
  lpdd             = 0;
  lpdd7            = 0;
  lpdds            = 0;
  surface[0].lpdds = 0;
  surface[1].lpdds = 0;
  lpddc            = 0;
}
