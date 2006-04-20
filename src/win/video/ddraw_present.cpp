//create screen and backbuffer surfaces
void VideoDD::create_presentation() {
  memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
  ddsd.dwSize = sizeof(DDSURFACEDESC2);

  if(settings.fullscreen == true) {
    ddsd.dwFlags           = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.dwBackBufferCount = 2;
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
  snes->get_video_info(&vi);
  SetRect(&rs, 0, 0, vi.width, vi.height);

  if(settings.fullscreen == true) {
    SetRect(&rd, settings.rx, settings.ry, settings.rx + settings.rw, settings.ry + settings.rh);
    backbuffer->Blt(&rd, surface, &rs, DDBLT_WAIT, 0);
    hr = screen->Flip(0, DDFLIP_WAIT);
  } else {
  POINT p = { 0, 0 };
    ClientToScreen(wMain.hwnd, &p);
    GetClientRect(wMain.hwnd, &rd);
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
