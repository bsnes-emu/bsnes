#include <ddraw.h>
#undef interface

struct VideoDirectDraw : Video {
  VideoDirectDraw() { initialize(); }
  ~VideoDirectDraw() { terminate(); }

  auto driver() -> string override { return "DirectDraw"; }
  auto ready() -> bool override { return _ready; }

  auto hasContext() -> bool override { return true; }
  auto hasBlocking() -> bool override { return true; }

  auto setContext(uintptr context) -> bool override {
    if(context == Video::context()) return true;
    if(!Video::setContext(context)) return false;
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool override {
    if(blocking == Video::blocking()) return true;
    if(!Video::setBlocking(blocking)) return false;
    return true;
  }

  auto clear() -> void override {
    if(!ready()) return;
    DDBLTFX fx = {};
    fx.dwSize = sizeof(DDBLTFX);
    fx.dwFillColor = 0x00000000;
    _screen->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
    _raster->Blt(0, 0, 0, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
  }

  auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool override {
    if(!ready()) return false;
    if(width != _width || height != _height) resize(_width = width, _height = height);
    DDSURFACEDESC2 description = {};
    description.dwSize = sizeof(DDSURFACEDESC2);
    if(_raster->Lock(0, &description, DDLOCK_WAIT, 0) != DD_OK) {
      _raster->Restore();
      if(_raster->Lock(0, &description, DDLOCK_WAIT, 0) != DD_OK) return false;
    }
    pitch = description.lPitch;
    return data = (uint32_t*)description.lpSurface;
  }

  auto release() -> void override {
    if(!ready()) return;
    _raster->Unlock(0);
  }

  auto output() -> void override {
    if(!ready()) return;
    if(_blocking) while(true) {
      BOOL vblank;
      _interface->GetVerticalBlankStatus(&vblank);
      if(vblank) break;
    }

    RECT source;
    SetRect(&source, 0, 0, _width, _height);

    POINT point = {0, 0};
    ClientToScreen((HWND)_context, &point);

    RECT target;
    GetClientRect((HWND)_context, &target);
    OffsetRect(&target, point.x, point.y);

    if(_screen->Blt(&target, _raster, &source, DDBLT_WAIT, 0) == DDERR_SURFACELOST) {
      _screen->Restore();
      _raster->Restore();
    }
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!_context) return false;

    LPDIRECTDRAW interface = nullptr;
    DirectDrawCreate(0, &interface, 0);
    interface->QueryInterface(IID_IDirectDraw7, (void**)&_interface);
    interface->Release();

    _interface->SetCooperativeLevel((HWND)_context, DDSCL_NORMAL);

    DDSURFACEDESC2 description = {};
    description.dwSize = sizeof(DDSURFACEDESC2);
    description.dwFlags = DDSD_CAPS;
    description.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    _interface->CreateSurface(&description, &_screen, 0);

    _interface->CreateClipper(0, &_clipper, 0);
    _clipper->SetHWnd(0, (HWND)_context);
    _screen->SetClipper(_clipper);

    _raster = nullptr;
    _surfaceWidth = 0;
    _surfaceHeight = 0;
    resize(_width = 256, _height = 256);
    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;
    if(_clipper) { _clipper->Release(); _clipper = nullptr; }
    if(_raster) { _raster->Release(); _raster = nullptr; }
    if(_screen) { _screen->Release(); _screen = nullptr; }
    if(_interface) { _interface->Release(); _interface = nullptr; }
  }

  auto resize(uint width, uint height) -> void {
    if(_surfaceWidth >= width && _surfaceHeight >= height) return;

    _surfaceWidth = max(width, _surfaceWidth);
    _surfaceHeight = max(height, _surfaceHeight);

    if(_raster) _raster->Release();

    DDSURFACEDESC2 description = {};
    description.dwSize = sizeof(DDSURFACEDESC2);
    _screen->GetSurfaceDesc(&description);
    int depth = description.ddpfPixelFormat.dwRGBBitCount;
    if(depth == 32) goto tryNativeSurface;

    memory::fill(&description, sizeof(DDSURFACEDESC2));
    description.dwSize = sizeof(DDSURFACEDESC2);
    description.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
    description.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;  //DDSCAPS_SYSTEMMEMORY
    description.dwWidth = _surfaceWidth;
    description.dwHeight = _surfaceHeight;

    description.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    description.ddpfPixelFormat.dwFlags = DDPF_RGB;
    description.ddpfPixelFormat.dwRGBBitCount = 32;
    description.ddpfPixelFormat.dwRBitMask = 0xff0000;
    description.ddpfPixelFormat.dwGBitMask = 0x00ff00;
    description.ddpfPixelFormat.dwBBitMask = 0x0000ff;

    if(_interface->CreateSurface(&description, &_raster, 0) == DD_OK) return clear();

  tryNativeSurface:
    memory::fill(&description, sizeof(DDSURFACEDESC2));
    description.dwSize = sizeof(DDSURFACEDESC2);
    description.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    description.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;  //DDSCAPS_SYSTEMMEMORY
    description.dwWidth = _surfaceWidth;
    description.dwHeight = _surfaceHeight;

    if(_interface->CreateSurface(&description, &_raster, 0) == DD_OK) return clear();
  }

  bool _ready = false;

  uint _width = 0;
  uint _height = 0;

  LPDIRECTDRAW7 _interface = nullptr;
  LPDIRECTDRAWSURFACE7 _screen = nullptr;
  LPDIRECTDRAWSURFACE7 _raster = nullptr;
  LPDIRECTDRAWCLIPPER _clipper = nullptr;
  uint _surfaceWidth = 0;
  uint _surfaceHeight = 0;
};
