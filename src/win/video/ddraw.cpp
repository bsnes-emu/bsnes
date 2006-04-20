#include "ddraw_present.cpp"

bool VideoDD::update_video_profile() {
  term();

  update_video_settings();
  update_window();

  DirectDrawCreate(0, &lpdd, 0);
  lpdd->QueryInterface(IID_IDirectDraw7, (void**)&lpdd7);
  SafeRelease(lpdd);

  if(settings.fullscreen) {
    lpdd7->SetCooperativeLevel(wMain.hwnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
    lpdd7->SetDisplayMode(settings.resolution_width, settings.resolution_height, 16, settings.refresh_rate, 0);
  } else {
    lpdd7->SetCooperativeLevel(wMain.hwnd, DDSCL_NORMAL);
  }

  create_presentation();

  lpdd7->CreateClipper(0, &clipper, 0);
  clipper->SetHWnd(0, wMain.hwnd);
  screen->SetClipper(clipper);

  create_render_target();
  clear_video();
  update_window();
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

uint16 *VideoDD::lock(uint32 &pitch) {
  if(surface->Lock(0, &ddsd, DDLOCK_WAIT, 0) != DD_OK)return 0;
  pitch = ddsd.lPitch;
  return (uint16*)ddsd.lpSurface;
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

void VideoDD::init() {}

void VideoDD::term() {
  SafeRelease(clipper);
  SafeRelease(surface);
  SafeRelease(backbuffer);
  SafeRelease(screen);
  SafeRelease(lpdd7);
  SafeRelease(lpdd);
}

VideoDD::VideoDD() {
  lpdd       = 0;
  lpdd7      = 0;
  screen     = 0;
  backbuffer = 0;
  surface    = 0;
  clipper    = 0;
}
