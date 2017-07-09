#undef interface
#define interface struct
#include <d3d9.h>
#undef interface

static LRESULT CALLBACK VideoDirect3D_WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

struct VideoDirect3D : Video {
  VideoDirect3D() {
    POINT point = {0, 0};
    HMONITOR monitor = MonitorFromPoint(point, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFOEX information = {};
    information.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(monitor, &information);
    monitorWidth = information.rcMonitor.right - information.rcMonitor.left;
    monitorHeight = information.rcMonitor.bottom - information.rcMonitor.top;

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

    settings.exclusiveHandle = CreateWindowEx(WS_EX_TOPMOST, L"VideoDirect3D_Window", L"", WS_POPUP,
      information.rcMonitor.left, information.rcMonitor.top, monitorWidth, monitorHeight,
      nullptr, nullptr, GetModuleHandle(0), nullptr);
  }

  ~VideoDirect3D() {
    term();
    DestroyWindow(settings.exclusiveHandle);
  }

  LPDIRECT3D9 context = nullptr;
  LPDIRECT3DDEVICE9 device = nullptr;
  LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;
  D3DPRESENT_PARAMETERS presentation = {};
  D3DCAPS9 capabilities = {};
  LPDIRECT3DTEXTURE9 texture = nullptr;
  LPDIRECT3DSURFACE9 surface = nullptr;

  bool lost = true;
  uint windowWidth;
  uint windowHeight;
  uint textureWidth;
  uint textureHeight;
  uint monitorWidth;
  uint monitorHeight;

  struct Vertex {
    float x, y, z, rhw;  //screen coordinates
    float u, v;          //texture coordinates
  };

  struct {
    uint32_t textureUsage;
    uint32_t texturePool;
    uint32_t vertexUsage;
    uint32_t vertexPool;
    uint32_t filter;
  } flags;

  struct {
    bool exclusive = false;
    HWND handle = nullptr;
    bool synchronize = false;
    uint filter = Video::FilterLinear;

    HWND exclusiveHandle = nullptr;
    uint width;
    uint height;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Video::Exclusive) return true;
    if(name == Video::Handle) return true;
    if(name == Video::Synchronize) return true;
    if(name == Video::Filter) return true;
    if(name == Video::Shader) return false;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Video::Exclusive) return settings.exclusive;
    if(name == Video::Handle) return (uintptr_t)settings.handle;
    if(name == Video::Synchronize) return settings.synchronize;
    if(name == Video::Filter) return settings.filter;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Video::Exclusive && value.is<bool>()) {
      settings.exclusive = value.get<bool>();
      if(context) init();
      return true;
    }

    if(name == Video::Handle && value.is<uintptr>()) {
      settings.handle = (HWND)value.get<uintptr>();
      return true;
    }

    if(name == Video::Synchronize && value.is<bool>()) {
      settings.synchronize = value.get<bool>();
      return true;
    }

    if(name == Video::Filter && value.is<uint>()) {
      settings.filter = value.get<uint>();
      if(context) updateFilter();
      return true;
    }

    return false;
  }

  auto recover() -> bool {
    if(!device) return false;

    if(lost) {
      if(vertexBuffer) { vertexBuffer->Release(); vertexBuffer = nullptr; }
      if(surface) { surface->Release(); surface = nullptr; }
      if(texture) { texture->Release(); texture = nullptr; }
      if(device->Reset(&presentation) != D3D_OK) return false;
    }
    lost = false;

    device->SetDialogBoxMode(false);

    device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

    device->SetRenderState(D3DRS_LIGHTING, false);
    device->SetRenderState(D3DRS_ZENABLE, false);
    device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

    device->SetVertexShader(nullptr);
    device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

    device->CreateVertexBuffer(sizeof(Vertex) * 4, flags.vertexUsage, D3DFVF_XYZRHW | D3DFVF_TEX1,
      (D3DPOOL)flags.vertexPool, &vertexBuffer, nullptr);
    textureWidth = 0;
    textureHeight = 0;
    resize(settings.width = 256, settings.height = 256);
    updateFilter();
    clear();
    return true;
  }

  auto resize(uint width, uint height) -> void {
    if(textureWidth >= width && textureHeight >= height) return;

    textureWidth = bit::round(max(width, textureWidth));
    textureHeight = bit::round(max(height, textureHeight));

    if(capabilities.MaxTextureWidth < textureWidth || capabilities.MaxTextureWidth < textureHeight) return;

    if(texture) texture->Release();
    device->CreateTexture(textureWidth, textureHeight, 1, flags.textureUsage, D3DFMT_X8R8G8B8,
      (D3DPOOL)flags.texturePool, &texture, nullptr);
  }

  auto updateFilter() -> void {
    if(!device) return;
    if(lost && !recover()) return;

    flags.filter = settings.filter == Video::FilterNearest ? D3DTEXF_POINT : D3DTEXF_LINEAR;
    device->SetSamplerState(0, D3DSAMP_MINFILTER, flags.filter);
    device->SetSamplerState(0, D3DSAMP_MAGFILTER, flags.filter);
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
    vertexBuffer->Lock(0, sizeof(Vertex) * 4, (void**)&vertexPointer, 0);
    memory::copy(vertexPointer, vertex, sizeof(Vertex) * 4);
    vertexBuffer->Unlock();

    device->SetStreamSource(0, vertexBuffer, 0, sizeof(Vertex));
  }

  auto clear() -> void {
    if(lost && !recover()) return;

    D3DSURFACE_DESC surfaceDescription;
    texture->GetLevelDesc(0, &surfaceDescription);
    texture->GetSurfaceLevel(0, &surface);

    if(surface) {
      device->ColorFill(surface, 0, D3DCOLOR_XRGB(0x00, 0x00, 0x00));
      surface->Release();
      surface = nullptr;
    }

    //clear primary display and all backbuffers
    for(uint n : range(3)) {
      device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0f, 0);
      device->Present(0, 0, 0, 0);
    }
  }

  auto lock(uint32_t*& data, uint& pitch, uint width, uint height) -> bool {
    if(lost && !recover()) return false;

    if(width != settings.width || height != settings.height) {
      resize(settings.width = width, settings.height = height);
    }

    D3DSURFACE_DESC surfaceDescription;
    texture->GetLevelDesc(0, &surfaceDescription);
    texture->GetSurfaceLevel(0, &surface);

    D3DLOCKED_RECT lockedRectangle;
    surface->LockRect(&lockedRectangle, 0, D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD);
    pitch = lockedRectangle.Pitch;
    return data = (uint32_t*)lockedRectangle.pBits;
  }

  auto unlock() -> void {
    surface->UnlockRect();
    surface->Release();
    surface = nullptr;
  }

  auto refresh() -> void {
    if(lost && !recover()) return;

    RECT rectangle;
    GetClientRect(settings.handle, &rectangle);

    //if output size changed, driver must be re-initialized.
    //failure to do so causes scaling issues on some video drivers.
    if(windowWidth != rectangle.right || windowHeight != rectangle.bottom) init();

    device->BeginScene();
    uint x = 0, y = 0;
    if(settings.exclusive) {
      //center output in exclusive mode fullscreen window
      x = (monitorWidth - windowWidth) / 2;
      y = (monitorHeight - windowHeight) / 2;
    }
    setVertex(0, 0, settings.width, settings.height, textureWidth, textureHeight, x, y, windowWidth, windowHeight);
    device->SetTexture(0, texture);
    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    device->EndScene();

    if(settings.synchronize) {
      D3DRASTER_STATUS status;
      while(true) {  //wait for a previous vblank to finish, if necessary
        device->GetRasterStatus(0, &status);
        if(!status.InVBlank) break;
      }
      while(true) {  //wait for next vblank to begin
        device->GetRasterStatus(0, &status);
        if(status.InVBlank) break;
      }
    }

    if(device->Present(0, 0, 0, 0) == D3DERR_DEVICELOST) lost = true;
  }

  auto init() -> bool {
    term();

    RECT rectangle;
    GetClientRect(settings.handle, &rectangle);
    windowWidth = rectangle.right;
    windowHeight = rectangle.bottom;

    context = Direct3DCreate9(D3D_SDK_VERSION);
    if(!context) return false;

    memory::fill(&presentation, sizeof(presentation));
    presentation.Flags = D3DPRESENTFLAG_VIDEO;
    presentation.SwapEffect = D3DSWAPEFFECT_DISCARD;
    presentation.BackBufferCount = 1;
    presentation.MultiSampleType = D3DMULTISAMPLE_NONE;
    presentation.MultiSampleQuality = 0;
    presentation.EnableAutoDepthStencil = false;
    presentation.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
    presentation.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    if(!settings.exclusive) {
      presentation.hDeviceWindow = settings.handle;
      presentation.Windowed = true;
      presentation.BackBufferFormat = D3DFMT_UNKNOWN;
      presentation.BackBufferWidth = 0;
      presentation.BackBufferHeight = 0;

      ShowWindow(settings.exclusiveHandle, SW_HIDE);
      if(context->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, settings.handle,
        D3DCREATE_FPU_PRESERVE | D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentation, &device) != D3D_OK) {
        return false;
      }
    } else {
      presentation.hDeviceWindow = settings.exclusiveHandle;
      presentation.Windowed = false;
      presentation.BackBufferFormat = D3DFMT_X8R8G8B8;
      presentation.BackBufferWidth = monitorWidth;
      presentation.BackBufferHeight = monitorHeight;
      presentation.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

      ShowWindow(settings.exclusiveHandle, SW_SHOWNORMAL);
      if(context->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, settings.exclusiveHandle,
        D3DCREATE_FPU_PRESERVE | D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentation, &device) != D3D_OK) {
        return false;
      }
    }

    device->GetDeviceCaps(&capabilities);

    if(capabilities.Caps2 & D3DCAPS2_DYNAMICTEXTURES) {
      flags.textureUsage = D3DUSAGE_DYNAMIC;
      flags.texturePool = D3DPOOL_DEFAULT;
      flags.vertexUsage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
      flags.vertexPool = D3DPOOL_DEFAULT;
    } else {
      flags.textureUsage = 0;
      flags.texturePool = D3DPOOL_MANAGED;
      flags.vertexUsage = D3DUSAGE_WRITEONLY;
      flags.vertexPool = D3DPOOL_MANAGED;
    }

    lost = false;
    return recover();
  }

  auto term() -> void {
    if(vertexBuffer) { vertexBuffer->Release(); vertexBuffer = nullptr; }
    if(surface) { surface->Release(); surface = nullptr; }
    if(texture) { texture->Release(); texture = nullptr; }
    if(device) { device->Release(); device = nullptr; }
    if(context) { context->Release(); context = nullptr; }
  }
};
