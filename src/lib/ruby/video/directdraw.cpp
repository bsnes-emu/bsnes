#include <windows.h>
#include <ddraw.h>

#include <ruby/ruby.h>

namespace ruby {

#include "directdraw.h"

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

  void clear() {
    DDBLTFX fx;
    fx.dwSize = sizeof(DDBLTFX);
    fx.dwFillColor = 0x00000000;
    screen->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
    raster->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
  }

  bool lock(uint32_t *&data, unsigned &pitch) {
    if(raster->Lock(0, &ddsd, DDLOCK_WAIT, 0) != DD_OK) return false;
    pitch = ddsd.lPitch;
    return data = (uint32_t*)ddsd.lpSurface;
  }

  void unlock() {
    raster->Unlock(0);
  }

  void refresh(unsigned r_width, unsigned r_height) {
    if(settings.synchronize) {
      while(true) {
        BOOL in_vblank;
        lpdd7->GetVerticalBlankStatus(&in_vblank);
        if(in_vblank == true) break;
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

  bool init() {
    term();

    DirectDrawCreate(0, &lpdd, 0);
    lpdd->QueryInterface(IID_IDirectDraw7, (void**)&lpdd7);
    if(lpdd) { lpdd->Release(); lpdd = 0; }

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
    clear();
    return true;
  }

  void create_raster() {
    screen->GetSurfaceDesc(&ddsd);
    int depth = ddsd.ddpfPixelFormat.dwRGBBitCount;
    if(depth == 32) goto try_native_surface;

    memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY; //DDSCAPS_SYSTEMMEMORY
    ddsd.dwWidth  = 1024;
    ddsd.dwHeight = 1024;

    ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
    ddsd.ddpfPixelFormat.dwRGBBitCount = 32;
    ddsd.ddpfPixelFormat.dwRBitMask = 0xff0000;
    ddsd.ddpfPixelFormat.dwGBitMask = 0x00ff00;
    ddsd.ddpfPixelFormat.dwBBitMask = 0x0000ff;

    if(lpdd7->CreateSurface(&ddsd, &raster, 0) == DD_OK) return;

    try_native_surface:
    memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY; //DDSCAPS_SYSTEMMEMORY
    ddsd.dwWidth  = 1024;
    ddsd.dwHeight = 1024;

    if(lpdd7->CreateSurface(&ddsd, &raster, 0) == DD_OK) return;
  }

  void term() {
    if(clipper) { clipper->Release(); clipper = 0; }
    if(raster) { raster->Release(); raster = 0; }
    if(screen) { screen->Release(); screen = 0; }
    if(lpdd7) { lpdd7->Release(); lpdd7 = 0; }
    if(lpdd) { lpdd->Release(); lpdd = 0; }
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
bool VideoDD::lock(uint32_t *&data, unsigned &pitch) { return p.lock(data, pitch); }
void VideoDD::unlock() { p.unlock(); }
void VideoDD::clear() { p.clear(); }
void VideoDD::refresh(unsigned width, unsigned height) { p.refresh(width, height); }
bool VideoDD::init() { return p.init(); }
void VideoDD::term() { p.term(); }
VideoDD::VideoDD() : p(*new pVideoDD(*this)) {}
VideoDD::~VideoDD() { delete &p; }

} //namespace ruby
