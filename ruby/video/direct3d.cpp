#undef interface
#define interface struct
#include <d3d9.h>
#undef interface

static LRESULT CALLBACK VideoDirect3D_WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

struct VideoDirect3D : VideoDriver {
  VideoDirect3D& self = *this;
  VideoDirect3D(Video& super) : VideoDriver(super) {}
  ~VideoDirect3D() { terminate(); }

  auto create() -> bool override {
    return initialize();
  }

  auto driver() -> string override { return "Direct3D 9.0"; }
  auto ready() -> bool override { return _ready; }

  auto hasExclusive() -> bool override { return true; }
  auto hasContext() -> bool override { return true; }
  auto hasBlocking() -> bool override { return true; }
  auto hasShader() -> bool override { return true; }

  auto setExclusive(bool exclusive) -> bool override { return initialize(); }
  auto setContext(uintptr context) -> bool override { return initialize(); }
  auto setBlocking(bool blocking) -> bool override { return true; }
  auto setShader(string shader) -> bool override { return updateFilter(); }

  auto clear() -> void override {
    if(_lost && !recover()) return;

    D3DSURFACE_DESC surfaceDescription;
    _texture->GetLevelDesc(0, &surfaceDescription);
    _texture->GetSurfaceLevel(0, &_surface);

    if(_surface) {
      D3DLOCKED_RECT lockedRectangle;
      _surface->LockRect(&lockedRectangle, 0, D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD);
      memory::fill(lockedRectangle.pBits, lockedRectangle.Pitch * surfaceDescription.Height);
      _surface->UnlockRect();
      _surface->Release();
      _surface = nullptr;
    }

    //clear primary display and all backbuffers
    for(uint n : range(3)) {
      _device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0f, 0);
      _device->Present(0, 0, 0, 0);
    }
  }

  auto acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool override {
    if(_lost && !recover()) return false;

    //if output size changed, driver must be re-initialized.
    //failure to do so causes scaling issues on some video drivers.
    RECT rectangle;
    GetClientRect((HWND)self.context, &rectangle);
    if(_windowWidth != rectangle.right || _windowHeight != rectangle.bottom) initialize();

    if(width != _inputWidth || height != _inputHeight) {
      resize(_inputWidth = width, _inputHeight = height);
    }

    D3DSURFACE_DESC surfaceDescription;
    _texture->GetLevelDesc(0, &surfaceDescription);
    _texture->GetSurfaceLevel(0, &_surface);

    D3DLOCKED_RECT lockedRectangle;
    _surface->LockRect(&lockedRectangle, 0, D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD);
    pitch = lockedRectangle.Pitch;
    return data = (uint32_t*)lockedRectangle.pBits;
  }

  auto release() -> void override {
    _surface->UnlockRect();
    _surface->Release();
    _surface = nullptr;
  }

  auto output() -> void override {
    if(_lost && !recover()) return;

    _device->BeginScene();
    uint x = 0, y = 0;
    if(self.exclusive) {
      //center output in exclusive mode fullscreen window
      x = (_monitorWidth - _windowWidth) / 2;
      y = (_monitorHeight - _windowHeight) / 2;
    }
    setVertex(0, 0, _inputWidth, _inputHeight, _textureWidth, _textureHeight, x, y, _windowWidth, _windowHeight);
    _device->SetTexture(0, _texture);
    _device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    _device->EndScene();

    if(self.blocking) {
      D3DRASTER_STATUS status;
      while(true) {  //wait for a previous vblank to finish, if necessary
        _device->GetRasterStatus(0, &status);
        if(!status.InVBlank) break;
      }
      while(true) {  //wait for next vblank to begin
        _device->GetRasterStatus(0, &status);
        if(status.InVBlank) break;
      }
    }

    if(_device->Present(0, 0, 0, 0) == D3DERR_DEVICELOST) _lost = true;
  }

private:
  auto recover() -> bool {
    if(!_device) return false;

    if(_lost) {
      if(_vertexBuffer) { _vertexBuffer->Release(); _vertexBuffer = nullptr; }
      if(_surface) { _surface->Release(); _surface = nullptr; }
      if(_texture) { _texture->Release(); _texture = nullptr; }
      if(_device->Reset(&_presentation) != D3D_OK) return false;
    }
    _lost = false;

    _device->SetDialogBoxMode(false);

    _device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    _device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    _device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    _device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    _device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    _device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

    _device->SetRenderState(D3DRS_LIGHTING, false);
    _device->SetRenderState(D3DRS_ZENABLE, false);
    _device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    _device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    _device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    _device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

    _device->SetVertexShader(nullptr);
    _device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

    _device->CreateVertexBuffer(sizeof(Vertex) * 4, _vertexUsage, D3DFVF_XYZRHW | D3DFVF_TEX1,
      (D3DPOOL)_vertexPool, &_vertexBuffer, nullptr);
    _textureWidth = 0;
    _textureHeight = 0;
    resize(_inputWidth = 256, _inputHeight = 256);
    updateFilter();
    clear();
    return true;
  }

  auto resize(uint width, uint height) -> void {
    if(_textureWidth >= width && _textureHeight >= height) return;

    _textureWidth = bit::round(max(width, _textureWidth));
    _textureHeight = bit::round(max(height, _textureHeight));

    if(_capabilities.MaxTextureWidth < _textureWidth || _capabilities.MaxTextureWidth < _textureHeight) return;

    if(_texture) _texture->Release();
    _device->CreateTexture(_textureWidth, _textureHeight, 1, _textureUsage, D3DFMT_X8R8G8B8,
      (D3DPOOL)_texturePool, &_texture, nullptr);
  }

  auto updateFilter() -> bool {
    if(!_device) return false;
    if(_lost && !recover()) return false;

    auto filter = self.shader == "Blur" ? D3DTEXF_LINEAR : D3DTEXF_POINT;
    _device->SetSamplerState(0, D3DSAMP_MINFILTER, filter);
    _device->SetSamplerState(0, D3DSAMP_MAGFILTER, filter);
    return true;
  }

  //(x,y) screen coordinates, in pixels
  //(u,v) texture coordinates, betweeen 0.0 (top, left) to 1.0 (bottom, right)
  auto setVertex(
    uint32_t px, uint32_t py, uint32_t pw, uint32_t ph,
    uint32_t tw, uint32_t th,
    uint32_t x, uint32_t y, uint32_t w, uint32_t h
  ) -> void {
    Vertex vertex[4];
    vertex[0].x = vertex[2].x = (double)(x     - 0.5);
    vertex[1].x = vertex[3].x = (double)(x + w - 0.5);
    vertex[0].y = vertex[1].y = (double)(y     - 0.5);
    vertex[2].y = vertex[3].y = (double)(y + h - 0.5);

    //Z-buffer and RHW are unused for 2D blit, set to normal values
    vertex[0].z = vertex[1].z = vertex[2].z = vertex[3].z = 0.0;
    vertex[0].rhw = vertex[1].rhw = vertex[2].rhw = vertex[3].rhw = 1.0;

    double rw = (double)w / (double)pw * (double)tw;
    double rh = (double)h / (double)ph * (double)th;
    vertex[0].u = vertex[2].u = (double)(px    ) / rw;
    vertex[1].u = vertex[3].u = (double)(px + w) / rw;
    vertex[0].v = vertex[1].v = (double)(py    ) / rh;
    vertex[2].v = vertex[3].v = (double)(py + h) / rh;

    LPDIRECT3DVERTEXBUFFER9* vertexPointer = nullptr;
    _vertexBuffer->Lock(0, sizeof(Vertex) * 4, (void**)&vertexPointer, 0);
    memory::copy<Vertex>(vertexPointer, vertex, 4);
    _vertexBuffer->Unlock();

    _device->SetStreamSource(0, _vertexBuffer, 0, sizeof(Vertex));
  }

  auto initialize() -> bool {
    terminate();
    if(!self.context) return false;

    HMONITOR monitor = MonitorFromWindow((HWND)self.context, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFOEX information = {};
    information.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(monitor, &information);
    _monitorWidth = information.rcMonitor.right - information.rcMonitor.left;
    _monitorHeight = information.rcMonitor.bottom - information.rcMonitor.top;

    WNDCLASS windowClass = {};
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    windowClass.hInstance = GetModuleHandle(0);
    windowClass.lpfnWndProc = VideoDirect3D_WindowProcedure;
    windowClass.lpszClassName = L"VideoDirect3D_Window";
    windowClass.lpszMenuName = 0;
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&windowClass);

    _exclusiveContext = (uintptr)CreateWindow(L"VideoDirect3D_Window", L"", WS_POPUP,
      information.rcMonitor.left, information.rcMonitor.top, _monitorWidth, _monitorHeight,
      nullptr, nullptr, GetModuleHandle(0), nullptr);

    RECT rectangle;
    GetClientRect((HWND)self.context, &rectangle);
    _windowWidth = rectangle.right;
    _windowHeight = rectangle.bottom;

    _instance = Direct3DCreate9(D3D_SDK_VERSION);
    if(!_instance) return false;

    memory::fill(&_presentation, sizeof(_presentation));
    _presentation.Flags = D3DPRESENTFLAG_VIDEO;
    _presentation.SwapEffect = D3DSWAPEFFECT_DISCARD;
    _presentation.BackBufferCount = 1;
    _presentation.MultiSampleType = D3DMULTISAMPLE_NONE;
    _presentation.MultiSampleQuality = 0;
    _presentation.EnableAutoDepthStencil = false;
    _presentation.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
    _presentation.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    if(!self.exclusive) {
      _presentation.hDeviceWindow = (HWND)self.context;
      _presentation.Windowed = true;
      _presentation.BackBufferFormat = D3DFMT_UNKNOWN;
      _presentation.BackBufferWidth = 0;
      _presentation.BackBufferHeight = 0;

      ShowWindow((HWND)_exclusiveContext, SW_HIDE);
      if(_instance->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)self.context,
        D3DCREATE_FPU_PRESERVE | D3DCREATE_SOFTWARE_VERTEXPROCESSING, &_presentation, &_device) != D3D_OK) {
        return false;
      }
    } else {
      _presentation.hDeviceWindow = (HWND)_exclusiveContext;
      _presentation.Windowed = false;
      _presentation.BackBufferFormat = D3DFMT_X8R8G8B8;
      _presentation.BackBufferWidth = _monitorWidth;
      _presentation.BackBufferHeight = _monitorHeight;
      _presentation.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

      ShowWindow((HWND)_exclusiveContext, SW_SHOWNORMAL);
      if(_instance->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)_exclusiveContext,
        D3DCREATE_FPU_PRESERVE | D3DCREATE_SOFTWARE_VERTEXPROCESSING, &_presentation, &_device) != D3D_OK) {
        return false;
      }
    }

    _device->GetDeviceCaps(&_capabilities);

    if(_capabilities.Caps2 & D3DCAPS2_DYNAMICTEXTURES) {
      _textureUsage = D3DUSAGE_DYNAMIC;
      _texturePool = D3DPOOL_DEFAULT;
      _vertexUsage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
      _vertexPool = D3DPOOL_DEFAULT;
    } else {
      _textureUsage = 0;
      _texturePool = D3DPOOL_MANAGED;
      _vertexUsage = D3DUSAGE_WRITEONLY;
      _vertexPool = D3DPOOL_MANAGED;
    }

    _lost = false;
    return _ready = recover();
  }

  auto terminate() -> void {
    _ready = false;
    if(_vertexBuffer) { _vertexBuffer->Release(); _vertexBuffer = nullptr; }
    if(_surface) { _surface->Release(); _surface = nullptr; }
    if(_texture) { _texture->Release(); _texture = nullptr; }
    if(_device) { _device->Release(); _device = nullptr; }
    if(_instance) { _instance->Release(); _instance = nullptr; }
    if(_exclusiveContext) { DestroyWindow((HWND)_exclusiveContext); _exclusiveContext = 0; }
  }

  struct Vertex {
    float x, y, z, rhw;  //screen coordinates
    float u, v;          //texture coordinates
  };

  bool _ready = false;
  uintptr _exclusiveContext = 0;

  LPDIRECT3D9 _instance = nullptr;
  LPDIRECT3DDEVICE9 _device = nullptr;
  LPDIRECT3DVERTEXBUFFER9 _vertexBuffer = nullptr;
  D3DPRESENT_PARAMETERS _presentation = {};
  D3DCAPS9 _capabilities = {};
  LPDIRECT3DTEXTURE9 _texture = nullptr;
  LPDIRECT3DSURFACE9 _surface = nullptr;

  bool _lost = true;
  uint _windowWidth;
  uint _windowHeight;
  uint _textureWidth;
  uint _textureHeight;
  uint _monitorWidth;
  uint _monitorHeight;
  uint _inputWidth;
  uint _inputHeight;

  uint32_t _textureUsage;
  uint32_t _texturePool;
  uint32_t _vertexUsage;
  uint32_t _vertexPool;
};
