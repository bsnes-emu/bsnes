#define D3DVERTEX D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1

bool VideoD3D::update_video_profile() {
HRESULT hr;
  term();

  update_video_settings();

  lpd3d = Direct3DCreate9(D3D_SDK_VERSION);
  if(!lpd3d) {
    alert("Failed to create Direct3D9 interface");
    return false;
  }

  memset(&presentation, 0, sizeof(presentation));
  presentation.Flags                  = D3DPRESENTFLAG_VIDEO;
  presentation.SwapEffect             = (settings.triple_buffering == true) ?
                                        D3DSWAPEFFECT_FLIP : D3DSWAPEFFECT_DISCARD;
  presentation.hDeviceWindow          = hwnd;
  presentation.BackBufferCount        = (settings.triple_buffering == true) ? 2 : 1;
  presentation.MultiSampleType        = D3DMULTISAMPLE_NONE;
  presentation.MultiSampleQuality     = 0;
  presentation.EnableAutoDepthStencil = false;
  presentation.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
  presentation.PresentationInterval   = (settings.triple_buffering == true) ?
                                        D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
  presentation.FullScreen_RefreshRateInHz = settings.refresh_rate;

  if(settings.fullscreen == false) {
    presentation.Windowed         = true;
    presentation.BackBufferFormat = D3DFMT_UNKNOWN;
    presentation.BackBufferWidth  = 0;
    presentation.BackBufferHeight = 0;
  } else {
    presentation.Windowed         = false;
    presentation.BackBufferFormat = D3DFMT_R5G6B5;
    presentation.BackBufferWidth  = settings.resolution_width;
    presentation.BackBufferHeight = settings.resolution_height;
  }

  hr = lpd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
    D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentation, &device);
  if(hr != D3D_OK) {
    alert("Failed to create Direct3D9 device");
    return false;
  }

//detect device capabilities
  device->GetDeviceCaps(&d3dcaps);
  if(d3dcaps.MaxTextureWidth < 1024 || d3dcaps.MaxTextureWidth < 1024) {
    alert("Error: Direct3D renderer requires that the video card supports textures up to 1024x1024.\n"
      "However, this display adapter only supports %dx%d-sized textures.\n"
      "", d3dcaps.MaxTextureWidth, d3dcaps.MaxTextureHeight);
    return false;
  }
  caps.dynamic     = bool(d3dcaps.Caps2 & D3DCAPS2_DYNAMICTEXTURES);
  caps.addresswrap = bool(d3dcaps.TextureAddressCaps & D3DPTADDRESSCAPS_WRAP);
  caps.stretchrect = (d3dcaps.DevCaps2 & D3DDEVCAPS2_CAN_STRETCHRECT_FROM_TEXTURES) &&
    (d3dcaps.StretchRectFilterCaps & D3DPTFILTERCAPS_MINFPOINT)  &&
    (d3dcaps.StretchRectFilterCaps & D3DPTFILTERCAPS_MAGFPOINT)  &&
    (d3dcaps.StretchRectFilterCaps & D3DPTFILTERCAPS_MINFLINEAR) &&
    (d3dcaps.StretchRectFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR);

  if(bool(config::video.use_vram) == true && caps.dynamic == true) {
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

  device->SetDialogBoxMode(false);

  device->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
  device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

  device->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

  if(caps.addresswrap == true) {
    device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
    device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
  }

  device->SetRenderState(D3DRS_LIGHTING, false);
  device->SetRenderState(D3DRS_ZENABLE,  false);
  device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

  device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

  device->SetVertexShader(NULL);
  device->SetFVF(D3DVERTEX);

  update_hardware_filter();
  update_scanlines();

  if(caps.stretchrect == true) {
    device->CreateOffscreenPlainSurface(1024, 1024, D3DFMT_R5G6B5,
      D3DPOOL_DEFAULT, &surface, NULL);
  } else {
    device->CreateTexture(1024, 1024, 1, flags.t_usage, D3DFMT_R5G6B5,
      static_cast<D3DPOOL>(flags.t_pool), &texture, NULL);
  }

  device->CreateVertexBuffer(sizeof(d3dvertex) * 4, flags.v_usage, D3DVERTEX,
    static_cast<D3DPOOL>(flags.v_pool), &vertex_buffer, NULL);

  clear_video();
  return true;
}

void VideoD3D::update_hardware_filter() {
  if(!device)return;

  switch(settings.hardware_filter) {
  default:
  case HARDWARE_FILTER_PIXEL:    flags.filter = D3DTEXF_POINT;  break;
  case HARDWARE_FILTER_BILINEAR: flags.filter = D3DTEXF_LINEAR; break;
  }

  device->SetSamplerState(0, D3DSAMP_MINFILTER, flags.filter);
  device->SetSamplerState(0, D3DSAMP_MAGFILTER, flags.filter);
}

void VideoD3D::update_scanlines() {
LPDIRECT3DSURFACE9 surface;
D3DSURFACE_DESC    d3dsd;
D3DLOCKED_RECT     d3dlr;
  for(int32 i = 0; i < 3; i++) {
    SafeRelease(scanline[i]);
    device->CreateTexture(8, 512, 1, flags.t_usage, D3DFMT_A8R8G8B8,
      static_cast<D3DPOOL>(flags.t_pool), &scanline[i], NULL);
    if(!scanline[i]) {
      alert("Failed to create scanline surface %d", i);
      break;
    }
    scanline[i]->GetLevelDesc(0, &d3dsd);
    scanline[i]->GetSurfaceLevel(0, &surface);
    surface->LockRect(&d3dlr, 0, flags.lock);
  uint32 *ptr = (uint32*)d3dlr.pBits, sd;
    if(i == 0) {
      sd = uint(double(config::video.pscanline_intensity) / 100.0 * 256.0);
    } else {
      sd = uint(double(config::video.iscanline_intensity) / 100.0 * 256.0);
    }
    sd = ((sd > 255) ? 255 : ((sd < 0) ? 0 : sd)) << 24;
    for(int32 y = 0; y < 512; y++) {
      for(int32 x = 0; x < 8; x++) {
        switch(i) {
        case 0: *ptr++ = !(y & 1) ? 0 : sd; break;
        case 1: *ptr++ = !(y & 1) ? 0 : sd; break;
        case 2: *ptr++ =  (y & 1) ? 0 : sd; break;
        }
      }
    }
    surface->UnlockRect();
    surface->Release();
  }
}

bool VideoD3D::lock(uint16 *&data, uint &pitch) {
  if(caps.stretchrect == false) {
    texture->GetLevelDesc(0, &d3dsd);
    texture->GetSurfaceLevel(0, &surface);
  }
  surface->LockRect(&d3dlr, 0, flags.lock);
  pitch = d3dlr.Pitch;
  data = (uint16*)d3dlr.pBits;
  return data;
}

void VideoD3D::unlock() {
  surface->UnlockRect();
  if(caps.stretchrect == false) {
    surface->Release();
  }
}

/* Vertex format:

   0----------1
   |         /|
   |       /  |
   |     /    |
   |   /      |
   | /        |
   2----------3

  (x,y) screen coords, in pixels (-0.5 for texel / pixel correction)
  (u,v) texture coords, betweeen 0.0 (top, left) to 1.0 (bottom, right)
*/

void VideoD3D::set_vertex(uint32 px, uint32 py, uint32 pw, uint32 ph,
uint32 tw, uint32 th, uint32 x, uint32 y, uint32 w, uint32 h) {
d3dvertex vertex[4];
  vertex[0].x = vertex[2].x = (float)(x    ) - 0.5;
  vertex[1].x = vertex[3].x = (float)(x + w) - 0.5;
  vertex[0].y = vertex[1].y = (float)(y    ) - 0.5;
  vertex[2].y = vertex[3].y = (float)(y + h) - 0.5;

//unused
  vertex[0].z   = vertex[1].z   = 1.0;
  vertex[2].z   = vertex[3].z   = 1.0;
  vertex[0].rhw = vertex[1].rhw = 1.0;
  vertex[2].rhw = vertex[3].rhw = 1.0;

//setup a gradient fade for when pause is enabled
  vertex[0].color = 0x80000000;
  vertex[1].color = 0xa8000000;
  vertex[2].color = 0xa8000000;
  vertex[3].color = 0xd0000000;

float rw = (float)w / (float)pw * (float)tw;
float rh = (float)h / (float)ph * (float)th;
  vertex[0].u = vertex[2].u = (float)(px    ) / rw;
  vertex[1].u = vertex[3].u = (float)(px + w) / rw;
  vertex[0].v = vertex[1].v = (float)(py    ) / rh;
  vertex[2].v = vertex[3].v = (float)(py + h) / rh;

  vertex_buffer->Lock(0, sizeof(d3dvertex) * 4, (void**)&vertex_ptr, 0);
  memcpy(vertex_ptr, vertex, sizeof(d3dvertex) * 4);
  vertex_buffer->Unlock();

  device->SetStreamSource(0, vertex_buffer, 0, sizeof(d3dvertex));
}

void VideoD3D::pause_enable() {
  if(!device)return;

  if(texture) {
    device->BeginScene();
    device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE,  true);
    device->SetTexture(0, texture);
    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    device->EndScene();
    device->Present(0, 0, 0, 0);
  }
}

void VideoD3D::pause_disable() {
  if(!device)return;

  if(texture) {
    device->BeginScene();
    device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE,  true);
    device->SetTexture(0, texture);
    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    device->EndScene();
    device->Present(0, 0, 0, 0);
  }
}

void VideoD3D::clear_video() {
  if(!device)return;

  if(caps.stretchrect == false && !texture)return;

  if(caps.stretchrect == false) {
    texture->GetLevelDesc(0, &d3dsd);
    texture->GetSurfaceLevel(0, &surface);
  }

  if(surface) {
    device->ColorFill(surface, 0, D3DCOLOR_XRGB(0x00, 0x00, 0x00));
    if(caps.stretchrect == false) {
      surface->Release();
    }
  }

//clear primary display and all backbuffers
  for(int i = 0; i < 3; i++) {
    device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0f, 0);
    device->Present(0, 0, 0, 0);
  }
}

void VideoD3D::redraw() {
  if(!device)return;

  device->BeginScene();
  snes.get_video_info(&vi);

  if(caps.stretchrect == true) {
  RECT rs, rd;
    SetRect(&rs, 0, 0, vi.width, vi.height);
    SetRect(&rd, settings.rx, settings.ry, settings.rx + settings.rw, settings.ry + settings.rh);
    device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &temp_surface);
    device->StretchRect(surface, &rs, temp_surface, &rd, static_cast<D3DTEXTUREFILTERTYPE>(flags.filter));
    temp_surface->Release();
  } else {
    set_vertex(0, 0, vi.width, vi.height, 1024, 1024, settings.rx, settings.ry, settings.rw, settings.rh);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

    device->SetTexture(0, texture);
    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
  }

  if(settings.enable_scanlines == true && settings.rh >= 448 && (
    (!r_cpu->interlace() && uint(config::video.pscanline_intensity) > 0) ||
    ( r_cpu->interlace() && uint(config::video.iscanline_intensity) > 0)
  )) {
  int32 i = (!r_cpu->interlace()) ? 0 : ((!r_cpu->interlace_field()) ? 1 : 2);
    set_vertex(0, 0, 512, 448, 8, 512, settings.rx, settings.ry, settings.rw, settings.rh);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    device->SetTexture(0, scanline[i]);
    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
  }

  device->EndScene();
  device->Present(0, 0, 0, 0);
}

void VideoD3D::update() {
  redraw();
}

bool VideoD3D::capture_screenshot() {
  if(!d3dx || !pD3DXSaveSurfaceToFileA) { return false; }

//fill both backbuffers with currently rendered image
  redraw();
  redraw();

  if(device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &temp_surface) != D3D_OK) {
    return false;
  }

FILE *fp;
char fn[4096];
int i;
  for(i = 0; i <= 999; i++) {
    sprintf(fn, "image%0.3d.png", i);
    fp = fopen(fn, "rb");
    if(!fp)break;
    fclose(fp);
    fp = 0;
  }
  if(i >= 1000)return false;

  pD3DXSaveSurfaceToFileA(fn, D3DXIFF_PNG, temp_surface, NULL, NULL);
  temp_surface->Release();
  return true;
}

void VideoD3D::init() {
  term();
  update_video_profile();
}

void VideoD3D::term() {
  SafeRelease(vertex_buffer);
  SafeRelease(surface);
  SafeRelease(texture);
  SafeRelease(scanline[0]);
  SafeRelease(scanline[1]);
  SafeRelease(scanline[2]);
  SafeRelease(device);
  SafeRelease(lpd3d);
}

VideoD3D::VideoD3D(HWND handle) {
  hwnd = handle;

  vertex_buffer = 0;
  surface       = 0;
  texture       = 0;
  scanline[0]   = 0;
  scanline[1]   = 0;
  scanline[2]   = 0;
  device        = 0;
  lpd3d         = 0;

//Below code dynamically loads d3dx9 DLL at runtime
//to map D3DXSaveSurfaceToFileA function, used to
//capture screenshots.
//
//We are forced to link against this DLL dynamically
//because Microsoft decided that releasing 10+ versions
//of this DLL, all with different filenames, rather than
//making one DLL that was backwards-compatible.
//
//As a result of this blatant stupidity, if the application
//statically links against d3dx9.lib, then the application
//will then *require* the version of the d3dx9 DLL included
//with the DX9 SDK used to build said application.
//
//And since most end users generally have not downloaded
//every single bimonthly DX9 update, the application will
//completely fail to load when it cannot find said dx9 DLL.
//
//Hillariously enough, most (if not all) of the d3dx9
//functionality appears to be backwards-compatible anyway,
//including this function.
//
//Therefore, as a workaround, we search through all possible
//DLL filenames. If a d3dx9 DLL is found, we dynamically map
//the D3DXSaveSurfaceToFileA function. Otherwise, screenshot
//support will fail silently.
//
//The benefit to this approach is that it allows all users
//to run this application, without explicitly requiring said
//d3dx9 DLL to run the application at all.

  d3dx = 0;
  pD3DXSaveSurfaceToFileA = 0;

  for(int i = 0; i < 256; i++) {
  char t[256];
    sprintf(t, "d3dx9_%d.dll", i);
    d3dx = LoadLibrary(t);
    if(d3dx) { break; }
  }
  if(!d3dx) { d3dx = LoadLibrary("d3dx9.dll"); }
  if(!d3dx) { return; }

  pD3DXSaveSurfaceToFileA = (HRESULT (WINAPI*)(LPCSTR, DWORD, LPDIRECT3DSURFACE9, CONST PALETTEENTRY*, CONST RECT*))
    GetProcAddress(d3dx, "D3DXSaveSurfaceToFileA");
}

VideoD3D::~VideoD3D() {
  if(d3dx)FreeLibrary(d3dx);
}

#undef D3DVERTEX
