#include "video.directdraw.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ddraw.h>

class pVideoDD {
public:
  VideoDD &self;

  LPDIRECTDRAW lpdd;
  LPDIRECTDRAW7 lpdd7;
  LPDIRECTDRAWSURFACE7 screen, raster;
  LPDIRECTDRAWCLIPPER clipper;
  DDSURFACEDESC2 ddsd;
  DDSCAPS2 ddscaps;

  struct {
    HWND handle;
    bool synchronize;
  } settings;

  bool cap(Video::Setting setting) {
    if(setting == Video::Handle) return true;
    if(setting == Video::Synchronize) return true;
    return false;
  }

  uintptr_t get(Video::Setting setting) {
    if(setting == Video::Handle) return (uintptr_t)settings.handle;
    if(setting == Video::Synchronize) return settings.synchronize;
    return false;
  }

  bool set(Video::Setting setting, uintptr_t param) {
    if(setting == Video::Handle) {
      settings.handle = (HWND)param;
      return true;
    }
    if(setting == Video::Synchronize) {
      settings.synchronize = param;
      return true;
    }
    return false;
  }

  void clear_video() {
  DDBLTFX fx;
    fx.dwSize = sizeof(DDBLTFX);
    fx.dwFillColor = 0x00000000;
    screen->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
    raster->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
  }

  bool lock(uint16 *&data, uint &pitch) {
    if(raster->Lock(0, &ddsd, DDLOCK_WAIT, 0) != DD_OK) return false;
    pitch = ddsd.lPitch;
    data = (uint16*)ddsd.lpSurface;
    return data;
  }

  void unlock() {
    raster->Unlock(0);
  }

  void refresh(uint r_width, uint r_height) {
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
    ClientToScreen(settings.handle, &p);
    GetClientRect(settings.handle, &rd);
    OffsetRect(&rd, p.x, p.y);

    if(screen->Blt(&rd, raster, &rs, DDBLT_WAIT, 0) == DDERR_SURFACELOST) {
      screen->Restore();
      raster->Restore();
    }
  }

  void init() {
    term();

    DirectDrawCreate(0, &lpdd, 0);
    lpdd->QueryInterface(IID_IDirectDraw7, (void**)&lpdd7);
    safe_release(lpdd);

    lpdd7->SetCooperativeLevel(settings.handle, DDSCL_NORMAL);

    memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
    ddsd.dwSize = sizeof(DDSURFACEDESC2);

    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    lpdd7->CreateSurface(&ddsd, &screen, 0);

    lpdd7->CreateClipper(0, &clipper, 0);
    clipper->SetHWnd(0, settings.handle);
    screen->SetClipper(clipper);

    create_raster();
    clear_video();
  }

  void create_raster() {
  int depth;
    screen->GetSurfaceDesc(&ddsd);
    depth = ddsd.ddpfPixelFormat.dwRGBBitCount;
    if(depth == 15 || depth == 16) goto try_native_surface;

    memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY; //DDSCAPS_SYSTEMMEMORY
    ddsd.dwWidth  = 1024;
    ddsd.dwHeight = 1024;

    ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
    ddsd.ddpfPixelFormat.dwRGBBitCount = 16;
    ddsd.ddpfPixelFormat.dwRBitMask = 0xf800;
    ddsd.ddpfPixelFormat.dwGBitMask = 0x07e0;
    ddsd.ddpfPixelFormat.dwBBitMask = 0x001f;

    if(lpdd7->CreateSurface(&ddsd, &raster, 0) == DD_OK) return;

  try_native_surface:
    memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY; //DDSCAPS_SYSTEMMEMORY
    ddsd.dwWidth  = 1024;
    ddsd.dwHeight = 1024;

    lpdd7->CreateSurface(&ddsd, &raster, 0);
  }

  void term() {
    safe_release(clipper);
    safe_release(raster);
    safe_release(screen);
    safe_release(lpdd7);
    safe_release(lpdd);
  }

  pVideoDD(VideoDD &self_) : self(self_) {
    lpdd = 0;
    lpdd7 = 0;
    screen = 0;
    raster = 0;
    clipper = 0;

    settings.handle = 0;
  }
};

bool VideoDD::cap(Setting setting) { return p.cap(setting); }
uintptr_t VideoDD::get(Setting setting) { return p.get(setting); }
bool VideoDD::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
bool VideoDD::lock(uint16 *&data, uint &pitch) { return p.lock(data, pitch); }
void VideoDD::unlock() { p.unlock(); }
void VideoDD::clear_video() { p.clear_video(); }
void VideoDD::refresh(uint width, uint height) { p.refresh(width, height); }
void VideoDD::init() { p.init(); }
void VideoDD::term() { p.term(); }
VideoDD::VideoDD() : p(*new pVideoDD(*this)) {}
VideoDD::~VideoDD() { delete &p; }
