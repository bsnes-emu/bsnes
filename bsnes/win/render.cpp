#include <ddraw.h>

LPDIRECTDRAW        lpdd  = 0;
LPDIRECTDRAWSURFACE lpdds = 0, lpddsb = 0;
LPDIRECTDRAWCLIPPER lpddc = 0;
DDSURFACEDESC       ddsd;
DDSCAPS             ddscaps;

void DestroyDDraw(void) {
  if(lpddc) {
    lpddc->Release();
    lpddc = 0;
  }
  if(lpddsb) {
    lpddsb->Release();
    lpddsb = 0;
  }
  if(lpdds) {
    lpdds->Release();
    lpdds = 0;
  }
  if(lpdd) {
    lpdd->Release();
    lpdd = 0;
  }
}

void CreateDDraw_Win(void) {
  DestroyDDraw();

  DirectDrawCreate(0, &lpdd, 0);
  lpdd->SetCooperativeLevel(hwndMain, DDSCL_NORMAL);

  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof(DDSURFACEDESC);
  ddsd.dwFlags = DDSD_CAPS;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
  lpdd->CreateSurface(&ddsd, &lpdds, 0);

  lpdd->CreateClipper(0, &lpddc, 0);
  lpddc->SetHWnd(0, hwndMain);
  lpdds->SetClipper(lpddc);

  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof(DDSURFACEDESC);
  ddsd.dwFlags  = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
  ddsd.dwWidth  = 512;
  ddsd.dwHeight = 478;
  lpdd->CreateSurface(&ddsd, &lpddsb, 0);
}

void CreateDDraw_Full(void) {
  DestroyDDraw();

  DirectDrawCreate(0, &lpdd, 0);
  lpdd->SetCooperativeLevel(hwndMain, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
  lpdd->SetDisplayMode(640, 480, 16);

  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof(DDSURFACEDESC);
  ddsd.dwFlags = DDSD_CAPS;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
  lpdd->CreateSurface(&ddsd, &lpdds, 0);

  lpdd->CreateClipper(0, &lpddc, 0);
  lpddc->SetHWnd(0, hwndMain);
  lpdds->SetClipper(lpddc);

  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof(DDSURFACEDESC);
  ddsd.dwFlags  = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
  ddsd.dwWidth  = 512;
  ddsd.dwHeight = 478;
  lpdd->CreateSurface(&ddsd, &lpddsb, 0);
}

void InitDisplay(void) {
  CreateDDraw_Win();
}

//size is doubled (mirrored) to prevent having to mask the top unused bit of snes color data
ulong render_color_lookup[65536];

void InitColorTable16(void) {
int i, r, g, b;
  for(i=0;i<65536;i++) {
    r = (i      ) & 31;
    g = (i >>  5) & 31;
    b = (i >> 10) & 31;
    render_color_lookup[i] = (r << 11) | (g << 6) | (b);
  }
}

void InitColorTable24(void) {
int i, r, g, b;
  for(i=0;i<65536;i++) {
    r = (i      ) & 31;
    g = (i >>  5) & 31;
    b = (i >> 10) & 31;
    render_color_lookup[i] = (r << 19) | (g << 11) | (b << 3);
  }
}

vfunc RenderScene;

#include "render_modes.cpp"

//sets up color table and sets render proc
void CreateColorTable(void) {
  lpdds->GetSurfaceDesc(&ddsd);
  switch(ddsd.ddpfPixelFormat.dwRGBBitCount) {
  case 16:
    InitColorTable16();
    RenderScene = RenderScene16;
    break;
  case 32:
    InitColorTable24();
    RenderScene = RenderScene32;
    break;
  default:
    alert("Error: Bit depth [%d] unsupported (supported depths: 16, 32)", ddsd.ddpfPixelFormat.dwRGBBitCount);
    exit(0);
    break;
  }
}

void DrawScene(void) {
RECT  rsrc, rdest;
POINT p;
HRESULT hr;
  p.x = p.y = 0;
  if(render.fullscreen == true) {
    SetRect(&rdest, 0, 0, 512, 448);
    OffsetRect(&rdest, (640 - 512) / 2, (480 - 448) / 2);
  } else {
    ClientToScreen(hwndMain, &p);
    GetClientRect(hwndMain, &rdest);
    OffsetRect(&rdest, p.x, p.y);
  }

  if(ppu.overscan == false) {
    SetRect(&rsrc, 0, 2, 512, 448);
  } else {
    SetRect(&rsrc, 0, 2 + 15, 512, 448 + 15);
  }

  hr = lpdds->Blt(&rdest, lpddsb, &rsrc, DDBLT_WAIT, 0);
  if(hr == DDERR_SURFACELOST) {
    lpdds->Restore();
    lpddsb->Restore();
  }
}

void UpdateDisplay(void) {
  RenderScene();
  DrawScene();
}

void video_setmode(bool fullscreen, word width, word height) {
bool prev_mode = render.fullscreen;
  if(debug_get_state() != DEBUGMODE_DISABLED)return;
  render.fullscreen     = fullscreen;
  render.display_width  = width;
  render.display_height = height;

//remove top scanline that is not rendered
  if     (height == 224)height = 223;
  else if(height == 239)height = 238;
  else if(height == 448)height = 446;
  else if(height == 478)height = 476;

  FixWindowSize(hwndMain, width, height);
  ShowWindow(hwndMain, SW_NORMAL);

  if(prev_mode != fullscreen) {
    if(fullscreen == true) {
      render.show_menu = false;
      UpdateMainWindowStyle(true);
      CreateDDraw_Full();
      ShowWindow(hwndMain, SW_NORMAL);
    } else {
      render.show_menu = true;
      UpdateMainWindowStyle(true);
      CreateDDraw_Win();
      FixWindowSize(hwndMain, width, height);
      ShowWindow(hwndMain, SW_NORMAL);
    }
    CreateColorTable();
  }
}

void video_setsnesmode(void) {
  if(ppu.bg_mode == 5 || ppu.bg_mode == 6) {
    render.snes_width = 512;
  } else {
    render.snes_width = 256;
  }
  if(ppu.interlace == false) {
    if(ppu.overscan == false) {
      render.snes_height = 224;
    } else {
      render.snes_height = 239;
    }
  } else {
    if(ppu.overscan == false) {
      render.snes_height = 448;
    } else {
      render.snes_height = 478;
    }
  }
}
