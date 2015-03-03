#undef interface
#define interface struct
#include <d3d9.h>
#include <d3dx9.h>
#undef interface

#define D3DVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

typedef HRESULT (__stdcall* EffectProc)(LPDIRECT3DDEVICE9, LPCVOID, UINT, D3DXMACRO const*, LPD3DXINCLUDE, DWORD, LPD3DXEFFECTPOOL, LPD3DXEFFECT*, LPD3DXBUFFER*);
typedef HRESULT (__stdcall* TextureProc)(LPDIRECT3DDEVICE9, LPCTSTR, LPDIRECT3DTEXTURE9*);

namespace ruby {

class pVideoD3D {
public:
  LPDIRECT3D9              lpd3d;
  LPDIRECT3DDEVICE9        device;
  LPDIRECT3DVERTEXBUFFER9  vertex_buffer;
  LPDIRECT3DVERTEXBUFFER9* vertex_ptr;
  D3DPRESENT_PARAMETERS    presentation;
  D3DSURFACE_DESC          d3dsd;
  D3DLOCKED_RECT           d3dlr;
  D3DRASTER_STATUS         d3drs;
  D3DCAPS9                 d3dcaps;
  LPDIRECT3DTEXTURE9       texture;
  LPDIRECT3DSURFACE9       surface;
  LPD3DXEFFECT             effect;
  string                   shader_source_markup;

  bool lost;
  unsigned iwidth, iheight;

  struct d3dvertex {
    float x, y, z, rhw;  //screen coords
    float u, v;          //texture coords
  };

  struct {
    uint32_t t_usage, v_usage;
    uint32_t t_pool,  v_pool;
    uint32_t lock;
    uint32_t filter;
  } flags;

  struct {
    bool dynamic;      //device supports dynamic textures
    bool shader;       //device supports pixel shaders
  } caps;

  struct {
    HWND handle;
    bool synchronize;
    unsigned filter;

    unsigned width;
    unsigned height;
  } settings;

  struct {
    unsigned width;
    unsigned height;
  } state;

  bool cap(const string& name) {
    if(name == Video::Handle) return true;
    if(name == Video::Synchronize) return true;
    if(name == Video::Filter) return true;
    if(name == Video::Shader) return false;
    return false;
  }

  any get(const string& name) {
    if(name == Video::Handle) return (uintptr_t)settings.handle;
    if(name == Video::Synchronize) return settings.synchronize;
    if(name == Video::Filter) return settings.filter;
    return false;
  }

  bool set(const string& name, const any& value) {
    if(name == Video::Handle) {
      settings.handle = (HWND)any_cast<uintptr_t>(value);
      return true;
    }

    if(name == Video::Synchronize) {
      settings.synchronize = any_cast<bool>(value);
      return true;
    }

    if(name == Video::Filter) {
      settings.filter = any_cast<unsigned>(value);
      if(lpd3d) update_filter();
      return true;
    }

    if(name == Video::Shader) {
      return false;
      set_shader(any_cast<const char*>(value));
      return true;
    }

    return false;
  }

  bool recover() {
    if(!device) return false;

    if(lost) {
      release_resources();
      if(device->Reset(&presentation) != D3D_OK) return false;
    }

    lost = false;

    device->SetDialogBoxMode(false);

    device->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
    device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    device->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
    device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

    device->SetRenderState(D3DRS_LIGHTING, false);
    device->SetRenderState(D3DRS_ZENABLE,  false);
    device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
    device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

    device->SetVertexShader(NULL);
    device->SetFVF(D3DVERTEX);

    device->CreateVertexBuffer(sizeof(d3dvertex) * 4, flags.v_usage, D3DVERTEX, (D3DPOOL)flags.v_pool, &vertex_buffer, NULL);
    iwidth  = 0;
    iheight = 0;
    resize(settings.width = 256, settings.height = 256);
    update_filter();
    clear();
    return true;
  }

  unsigned rounded_power_of_two(unsigned n) {
    n--;
    n |= n >>  1;
    n |= n >>  2;
    n |= n >>  4;
    n |= n >>  8;
    n |= n >> 16;
    return n + 1;
  }

  void resize(unsigned width, unsigned height) {
    if(iwidth >= width && iheight >= height) return;

    iwidth  = rounded_power_of_two(max(width,  iwidth ));
    iheight = rounded_power_of_two(max(height, iheight));

    if(d3dcaps.MaxTextureWidth < iwidth || d3dcaps.MaxTextureWidth < iheight) {
      //TODO: attempt to handle this more gracefully
      return;
    }

    if(texture) texture->Release();
    device->CreateTexture(iwidth, iheight, 1, flags.t_usage, D3DFMT_X8R8G8B8, (D3DPOOL)flags.t_pool, &texture, NULL);
  }

  void update_filter() {
    if(!device) return;
    if(lost && !recover()) return;

    flags.filter = (settings.filter == Video::FilterNearest ? D3DTEXF_POINT : D3DTEXF_LINEAR);
    device->SetSamplerState(0, D3DSAMP_MINFILTER, flags.filter);
    device->SetSamplerState(0, D3DSAMP_MAGFILTER, flags.filter);
  }

  //  Vertex format:
  //
  //  0----------1
  //  |         /|
  //  |       /  |
  //  |     /    |
  //  |   /      |
  //  | /        |
  //  2----------3
  //
  //  (x,y) screen coords, in pixels
  //  (u,v) texture coords, betweeen 0.0 (top, left) to 1.0 (bottom, right)
  void set_vertex(
    uint32_t px, uint32_t py, uint32_t pw, uint32_t ph,
    uint32_t tw, uint32_t th,
    uint32_t x, uint32_t y, uint32_t w, uint32_t h
  ) {
    d3dvertex vertex[4];
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

    vertex_buffer->Lock(0, sizeof(d3dvertex) * 4, (void**)&vertex_ptr, 0);
    memcpy(vertex_ptr, vertex, sizeof(d3dvertex) * 4);
    vertex_buffer->Unlock();

    device->SetStreamSource(0, vertex_buffer, 0, sizeof(d3dvertex));
  }

  void clear() {
    if(lost && !recover()) return;

    texture->GetLevelDesc(0, &d3dsd);
    texture->GetSurfaceLevel(0, &surface);

    if(surface) {
      device->ColorFill(surface, 0, D3DCOLOR_XRGB(0x00, 0x00, 0x00));
      surface->Release();
      surface = nullptr;
    }

    //clear primary display and all backbuffers
    for(unsigned i = 0; i < 3; i++) {
      device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0f, 0);
      device->Present(0, 0, 0, 0);
    }
  }

  bool lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) {
    if(lost && !recover()) return false;

    if(width != settings.width || height != settings.height) {
      resize(settings.width = width, settings.height = height);
    }

    texture->GetLevelDesc(0, &d3dsd);
    texture->GetSurfaceLevel(0, &surface);

    surface->LockRect(&d3dlr, 0, flags.lock);
    pitch = d3dlr.Pitch;
    return data = (uint32_t*)d3dlr.pBits;
  }

  void unlock() {
    surface->UnlockRect();
    surface->Release();
    surface = nullptr;
  }

  void refresh() {
    if(lost && !recover()) return;

    RECT rd, rs;  //dest, source rectangles
    GetClientRect(settings.handle, &rd);
    SetRect(&rs, 0, 0, settings.width, settings.height);

    //if output size changed, driver must be re-initialized.
    //failure to do so causes scaling issues on some video drivers.
    if(state.width != rd.right || state.height != rd.bottom) {
      init();
      set_shader(shader_source_markup);
      return;
    }

    if(caps.shader && effect) {
      device->BeginScene();
      set_vertex(0, 0, settings.width, settings.height, iwidth, iheight, 0, 0, rd.right, rd.bottom);

      D3DXVECTOR4 rubyTextureSize;
      rubyTextureSize.x = iwidth;
      rubyTextureSize.y = iheight;
      rubyTextureSize.z = 1.0 / iheight;
      rubyTextureSize.w = 1.0 / iwidth;
      effect->SetVector("rubyTextureSize", &rubyTextureSize);

      D3DXVECTOR4 rubyInputSize;
      rubyInputSize.x = settings.width;
      rubyInputSize.y = settings.height;
      rubyInputSize.z = 1.0 / settings.height;
      rubyInputSize.w = 1.0 / settings.width;
      effect->SetVector("rubyInputSize", &rubyInputSize);

      D3DXVECTOR4 rubyOutputSize;
      rubyOutputSize.x = rd.right;
      rubyOutputSize.y = rd.bottom;
      rubyOutputSize.z = 1.0 / rd.bottom;
      rubyOutputSize.w = 1.0 / rd.right;
      effect->SetVector("rubyOutputSize", &rubyOutputSize);

      UINT passes;
      effect->Begin(&passes, 0);
      effect->SetTexture("rubyTexture", texture);
      device->SetTexture(0, texture);
      for(unsigned pass = 0; pass < passes; pass++) {
        effect->BeginPass(pass);
        device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
        effect->EndPass();
      }
      effect->End();
      device->EndScene();
    } else {
      device->BeginScene();
      set_vertex(0, 0, settings.width, settings.height, iwidth, iheight, 0, 0, rd.right, rd.bottom);
      device->SetTexture(0, texture);
      device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
      device->EndScene();
    }

    if(settings.synchronize) {
      D3DRASTER_STATUS status;
      //wait for a previous vblank to finish, if necessary
      while(true) {
        device->GetRasterStatus(0, &status);
        if(status.InVBlank == false) break;
      }
      //wait for next vblank to begin
      while(true) {
        device->GetRasterStatus(0, &status);
        if(status.InVBlank == true) break;
      }
    }

    if(device->Present(0, 0, 0, 0) == D3DERR_DEVICELOST) lost = true;
  }

  void set_shader(const char* source) {
    if(!caps.shader) return;

    if(effect) {
      effect->Release();
      effect = NULL;
    }

    if(!source || !*source) {
      shader_source_markup = "";
      return;
    }
    shader_source_markup = source;

    XML::Document document(shader_source_markup);
    bool is_hlsl = document["shader"]["language"].data == "HLSL";
    string shader_source = document["shader"]["source"].data;
    if(shader_source == "") return;

    HMODULE d3dx;
    for(unsigned i = 0; i < 256; i++) {
      char t[256];
      sprintf(t, "d3dx9_%u.dll", i);
      d3dx = LoadLibraryW(utf16_t(t));
      if(d3dx) break;
    }
    if(!d3dx) d3dx = LoadLibraryW(L"d3dx9.dll");
    if(!d3dx) return;

    EffectProc effectProc = (EffectProc)GetProcAddress(d3dx, "D3DXCreateEffect");
    TextureProc textureProc = (TextureProc)GetProcAddress(d3dx, "D3DXCreateTextureFromFileA");

    LPD3DXBUFFER pBufferErrors = NULL;
    effectProc(device, (const void*)shader_source.data(), lstrlenA(shader_source), NULL, NULL, 0, NULL, &effect, &pBufferErrors);

    D3DXHANDLE hTech;
    effect->FindNextValidTechnique(NULL, &hTech);
    effect->SetTechnique(hTech);
  }

  bool init() {
    term();

    RECT rd;
    GetClientRect(settings.handle, &rd);
    state.width  = rd.right;
    state.height = rd.bottom;

    lpd3d = Direct3DCreate9(D3D_SDK_VERSION);
    if(!lpd3d) return false;

    memset(&presentation, 0, sizeof(presentation));
    presentation.Flags                  = D3DPRESENTFLAG_VIDEO;
    presentation.SwapEffect             = D3DSWAPEFFECT_FLIP;
    presentation.hDeviceWindow          = settings.handle;
    presentation.BackBufferCount        = 1;
    presentation.MultiSampleType        = D3DMULTISAMPLE_NONE;
    presentation.MultiSampleQuality     = 0;
    presentation.EnableAutoDepthStencil = false;
    presentation.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
    presentation.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
    presentation.Windowed               = true;
    presentation.BackBufferFormat       = D3DFMT_UNKNOWN;
    presentation.BackBufferWidth        = 0;
    presentation.BackBufferHeight       = 0;

    if(lpd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, settings.handle,
      D3DCREATE_FPU_PRESERVE | D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentation, &device) != D3D_OK) {
      return false;
    }

    device->GetDeviceCaps(&d3dcaps);

    caps.dynamic = bool(d3dcaps.Caps2 & D3DCAPS2_DYNAMICTEXTURES);
    caps.shader = d3dcaps.PixelShaderVersion > D3DPS_VERSION(1, 4);

    if(caps.dynamic == true) {
      flags.t_usage = D3DUSAGE_DYNAMIC;
      flags.v_usage = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;
      flags.t_pool  = D3DPOOL_DEFAULT;
      flags.v_pool  = D3DPOOL_DEFAULT;
      flags.lock    = D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD;
    } else {
      flags.t_usage = 0;
      flags.v_usage = D3DUSAGE_WRITEONLY;
      flags.t_pool  = D3DPOOL_MANAGED;
      flags.v_pool  = D3DPOOL_MANAGED;
      flags.lock    = D3DLOCK_NOSYSLOCK | D3DLOCK_DISCARD;
    }

    lost = false;
    recover();
    return true;
  }

  void release_resources() {
    if(effect) { effect->Release(); effect = 0; }
    if(vertex_buffer) { vertex_buffer->Release(); vertex_buffer = 0; }
    if(surface) { surface->Release(); surface = 0; }
    if(texture) { texture->Release(); texture = 0; }
  }

  void term() {
    release_resources();
    if(device) { device->Release(); device = 0; }
    if(lpd3d) { lpd3d->Release(); lpd3d = 0; }
  }

  pVideoD3D() {
    effect = 0;
    vertex_buffer = 0;
    surface = 0;
    texture = 0;
    device = 0;
    lpd3d = 0;
    lost = true;

    settings.handle = 0;
    settings.synchronize = false;
    settings.filter = Video::FilterLinear;
  }
};

DeclareVideo(D3D)

};

#undef D3DVERTEX
