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
  ddsd.dwHeight = 480;
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

  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof(DDSURFACEDESC);
  ddsd.dwFlags  = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
  ddsd.dwWidth  = 512;
  ddsd.dwHeight = 480;
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

//sets up color table and sets render proc
void CreateColorTable(void) {
  lpdds->GetSurfaceDesc(&ddsd);
  switch(ddsd.ddpfPixelFormat.dwRGBBitCount) {
  case 16:
    InitColorTable16();
    break;
  case 32:
    InitColorTable24();
    break;
  default:
    alert("Error: Bit depth [%d] unsupported (supported depths: 16, 32)", ddsd.ddpfPixelFormat.dwRGBBitCount);
    exit(0);
    break;
  }
}

#include "render_modes.cpp"

void SelectRenderer(void) {
  lpdds->GetSurfaceDesc(&ddsd);
  switch(ddsd.ddpfPixelFormat.dwRGBBitCount) {
  case 16:
    if       (render.snes_width == 512 && render.snes_height == 480) {
      RenderScene = RenderScene16_512x480;
    } else if(render.snes_width == 512 && render.snes_height == 448) {
      RenderScene = RenderScene16_512x448;
    } else if(render.snes_width == 512 && render.snes_height == 240) {
      RenderScene = RenderScene16_512x240;
    } else if(render.snes_width == 512 && render.snes_height == 224) {
      RenderScene = RenderScene16_512x224;
    } else if(render.snes_width == 256 && render.snes_height == 480) {
      RenderScene = RenderScene16_256x480;
    } else if(render.snes_width == 256 && render.snes_height == 448) {
      RenderScene = RenderScene16_256x448;
    } else if(render.snes_width == 256 && render.snes_height == 240) {
      RenderScene = RenderScene16_256x240;
    } else if(render.snes_width == 256 && render.snes_height == 224) {
      RenderScene = RenderScene16_256x224;
    } else {
      alert("Error: Unsupported SNES Resolution: %dx%d", render.snes_width, render.snes_height);
    }
    break;
  case 32:
    if       (render.snes_width == 512 && render.snes_height == 480) {
      RenderScene = RenderScene32_512x480;
    } else if(render.snes_width == 512 && render.snes_height == 448) {
      RenderScene = RenderScene32_512x448;
    } else if(render.snes_width == 512 && render.snes_height == 240) {
      RenderScene = RenderScene32_512x240;
    } else if(render.snes_width == 512 && render.snes_height == 224) {
      RenderScene = RenderScene32_512x224;
    } else if(render.snes_width == 256 && render.snes_height == 480) {
      RenderScene = RenderScene32_256x480;
    } else if(render.snes_width == 256 && render.snes_height == 448) {
      RenderScene = RenderScene32_256x448;
    } else if(render.snes_width == 256 && render.snes_height == 240) {
      RenderScene = RenderScene32_256x240;
    } else if(render.snes_width == 256 && render.snes_height == 224) {
      RenderScene = RenderScene32_256x224;
    } else {
      alert("Error: Unsupported SNES Resolution: %dx%d", render.snes_width, render.snes_height);
    }
    break;
  default:
    alert("Error: Bit depth [%d] unsupported (supported depths: 16, 32)", ddsd.ddpfPixelFormat.dwRGBBitCount);
    exit(0);
    break;
  }
}

void UpdateDisplay(void) {
RECT  rsrc, rdest;
POINT p;
HRESULT hr;
  RenderScene();

  p.x = p.y = 0;
  if(render.fullscreen == true) {
    SetRect(&rdest, 0, 0, 512, 448);
    OffsetRect(&rdest, (640 - 512) / 2, (480 - 448) / 2);
  } else {
    ClientToScreen(hwndMain, &p);
    GetClientRect(hwndMain, &rdest);
    OffsetRect(&rdest, p.x, p.y);
  }

  SetRect(&rsrc, 0, 0, render.snes_width, render.snes_height);

  hr = lpdds->Blt(&rdest, lpddsb, &rsrc, DDBLT_WAIT, 0);
  if(hr == DDERR_SURFACELOST) {
    lpdds->Restore();
    lpddsb->Restore();
  }
}

void UpdateDisplay_NoRender(void) {
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

  SetRect(&rsrc, 0, 0, render.snes_width, render.snes_height);

  hr = lpdds->Blt(&rdest, lpddsb, &rsrc, DDBLT_WAIT, 0);
  if(hr == DDERR_SURFACELOST) {
    lpdds->Restore();
    lpddsb->Restore();
  }
}

void video_setmode(bool fullscreen, word width, word height) {
bool prev_mode = render.fullscreen;
  render.fullscreen = fullscreen;
  render.width      = width;
  render.height     = height;

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
  }
}

void video_setsnesmode(void) {
  if(ppu.bg_mode == 5 || ppu.bg_mode == 6) {
    render.snes_width = 512;
  } else {
    render.snes_width = 256;
  }
  if(ppu.interlace == true) {
    if(ppu.visible_scanlines == 240) {
      render.snes_height = 480;
    } else {
      render.snes_height = 448;
    }
  } else {
    if(ppu.visible_scanlines == 240) {
      render.snes_height = 240;
    } else {
      render.snes_height = 224;
    }
  }

  SelectRenderer();
}
