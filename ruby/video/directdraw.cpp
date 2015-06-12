#include <ddraw.h>

namespace ruby {

struct pVideoDD {
  LPDIRECTDRAW lpdd = nullptr;
  LPDIRECTDRAW7 lpdd7 = nullptr;
  LPDIRECTDRAWSURFACE7 screen = nullptr;
  LPDIRECTDRAWSURFACE7 raster = nullptr;
  LPDIRECTDRAWCLIPPER clipper = nullptr;
  DDSURFACEDESC2 ddsd;
  DDSCAPS2 ddscaps;
  unsigned iwidth;
  unsigned iheight;

  struct {
    HWND handle = nullptr;
    bool synchronize = false;

    unsigned width;
    unsigned height;
  } settings;

  ~pVideoDD() {
    term();
  }

  auto cap(const string& name) -> bool {
    if(name == Video::Handle) return true;
    if(name == Video::Synchronize) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Video::Handle) return (uintptr_t)settings.handle;
    if(name == Video::Synchronize) return settings.synchronize;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Video::Handle && value.is<uintptr_t>()) {
      settings.handle = (HWND)value.get<uintptr_t>();
      return true;
    }

    if(name == Video::Synchronize && value.is<bool>()) {
      settings.synchronize = value.get<bool>();
      return true;
    }

    return false;
  }

  auto resize(unsigned width, unsigned height) -> void {
    if(iwidth >= width && iheight >= height) return;

    iwidth  = max(width,  iwidth);
    iheight = max(height, iheight);

    if(raster) raster->Release();

    screen->GetSurfaceDesc(&ddsd);
    int depth = ddsd.ddpfPixelFormat.dwRGBBitCount;
    if(depth == 32) goto try_native_surface;

    memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;  //DDSCAPS_SYSTEMMEMORY
    ddsd.dwWidth  = iwidth;
    ddsd.dwHeight = iheight;

    ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
    ddsd.ddpfPixelFormat.dwRGBBitCount = 32;
    ddsd.ddpfPixelFormat.dwRBitMask = 0xff0000;
    ddsd.ddpfPixelFormat.dwGBitMask = 0x00ff00;
    ddsd.ddpfPixelFormat.dwBBitMask = 0x0000ff;

    if(lpdd7->CreateSurface(&ddsd, &raster, 0) == DD_OK) return clear();

    try_native_surface:
    memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
    ddsd.dwSize = sizeof(DDSURFACEDESC2);
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;  //DDSCAPS_SYSTEMMEMORY
    ddsd.dwWidth  = iwidth;
    ddsd.dwHeight = iheight;

    if(lpdd7->CreateSurface(&ddsd, &raster, 0) == DD_OK) return clear();
  }

  auto clear() -> void {
    DDBLTFX fx;
    fx.dwSize = sizeof(DDBLTFX);
    fx.dwFillColor = 0x00000000;
    screen->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
    raster->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
  }

  auto lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) -> bool {
    if(width != settings.width || height != settings.height) {
      resize(settings.width = width, settings.height = height);
    }

    if(raster->Lock(0, &ddsd, DDLOCK_WAIT, 0) != DD_OK) {
      raster->Restore();
      if(raster->Lock(0, &ddsd, DDLOCK_WAIT, 0) != DD_OK) return false;
    }
    pitch = ddsd.lPitch;
    return data = (uint32_t*)ddsd.lpSurface;
  }

  auto unlock() -> void {
    raster->Unlock(0);
  }

  auto refresh() -> void {
    if(settings.synchronize) {
      while(true) {
        BOOL in_vblank;
        lpdd7->GetVerticalBlankStatus(&in_vblank);
        if(in_vblank == true) break;
      }
    }

    HRESULT hr;
    RECT rd, rs;
    SetRect(&rs, 0, 0, settings.width, settings.height);

    POINT p = {0, 0};
    ClientToScreen(settings.handle, &p);
    GetClientRect(settings.handle, &rd);
    OffsetRect(&rd, p.x, p.y);

    if(screen->Blt(&rd, raster, &rs, DDBLT_WAIT, 0) == DDERR_SURFACELOST) {
      screen->Restore();
      raster->Restore();
    }
  }

  auto init() -> bool {
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

    raster  = 0;
    iwidth  = 0;
    iheight = 0;
    resize(settings.width = 256, settings.height = 256);

    return true;
  }

  auto term() -> void {
    if(clipper) { clipper->Release(); clipper = 0; }
    if(raster) { raster->Release(); raster = 0; }
    if(screen) { screen->Release(); screen = 0; }
    if(lpdd7) { lpdd7->Release(); lpdd7 = 0; }
    if(lpdd) { lpdd->Release(); lpdd = 0; }
  }
};

DeclareVideo(DD)

};
