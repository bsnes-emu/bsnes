#define D3DVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

bool VideoD3D::update_video_mode() {
HRESULT hr;
  term();

  lpd3d = Direct3DCreate9(D3D_SDK_VERSION);
  if(!lpd3d) {
    alert("Failed to create Direct3D9 interface");
    return false;
  }

  memset(&presentation, 0, sizeof(presentation));
  presentation.Flags                  = D3DPRESENTFLAG_VIDEO;
  presentation.SwapEffect             = D3DSWAPEFFECT_FLIP;
  presentation.hDeviceWindow          = hwnd;
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

  if(lpd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
    D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentation, &device) != D3D_OK) {
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

  if(config::video.use_vram == true && caps.dynamic == true) {
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

  update_settings();

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

void VideoD3D::update_settings() {
  if(!device)return;

  switch(settings.filter) {
  case Video::FilterPoint:
    flags.filter = D3DTEXF_POINT;
    break;
  case Video::FilterLinear:
  default:
    flags.filter = D3DTEXF_LINEAR;
    break;
  }

  device->SetSamplerState(0, D3DSAMP_MINFILTER, flags.filter);
  device->SetSamplerState(0, D3DSAMP_MAGFILTER, flags.filter);
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

bool VideoD3D::lock(uint16 *&data, uint &pitch) {
  if(caps.stretchrect == false) {
    texture->GetLevelDesc(0, &d3dsd);
    texture->GetSurfaceLevel(0, &surface);
  }
  surface->LockRect(&d3dlr, 0, flags.lock);
  pitch = d3dlr.Pitch;
  data  = (uint16*)d3dlr.pBits;
  return data;
}

void VideoD3D::unlock() {
  surface->UnlockRect();
  if(caps.stretchrect == false)surface->Release();
}

void VideoD3D::refresh(uint r_width, uint r_height) {
  if(!device)return;

  device->BeginScene();

  if(caps.stretchrect == true) {
  RECT rs;
    SetRect(&rs, 0, 0, r_width, r_height);
  LPDIRECT3DSURFACE9 temp;
    device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &temp);
    device->StretchRect(surface, &rs, temp, 0, static_cast<D3DTEXTUREFILTERTYPE>(flags.filter));
    temp->Release();
  } else {
  RECT rd;
    GetClientRect(hwnd, &rd);
    set_vertex(0, 0, r_width, r_height, 1024, 1024, 0, 0, rd.right, rd.bottom);
    device->SetTexture(0, texture);
    device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
  }

  device->EndScene();

  if(settings.synchronize == true) {
  D3DRASTER_STATUS status;
    for(;;) {
      device->GetRasterStatus(0, &status);
      if(bool(status.InVBlank) == true)break;
    //Sleep(1);
    }
  }

  device->Present(0, 0, 0, 0);
}

void VideoD3D::init() {
  term();
  update_video_mode();
}

void VideoD3D::term() {
  safe_release(vertex_buffer);
  safe_release(surface);
  safe_release(texture);
  safe_release(device);
  safe_release(lpd3d);
}

VideoD3D::VideoD3D(HWND handle) {
  hwnd = handle;

  vertex_buffer = 0;
  surface       = 0;
  texture       = 0;
  device        = 0;
  lpd3d         = 0;
}

VideoD3D::~VideoD3D() {
}

#undef D3DVERTEX
