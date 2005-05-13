//This is in the global namespace for inline assembly optimizations.
//If it were in a class, I would have to cast a pointer to it, and
//to read from a pointer array requires an extra opcode:
//mov eax,[array+eax*4] -> lea ebx,[parray] ; mov ebx,[ebx+eax*4]
uint32 color_lookup_table[65536];

render::render() {
  lpdd    = 0;
  lpdds   = 0;
  lpddsb  = 0;
  lpddc   = 0;
}

void render::update_color_lookup_table() {
int i, r, g, b;
  lpdds->GetSurfaceDesc(&ddsd);
  color_depth = ddsd.ddpfPixelFormat.dwRGBBitCount;
  if(color_depth == 15) {
    for(i=0;i<65536;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;
      color_lookup_table[i] = (r << 10) | (g << 5) | (b);
    }
  } else if(color_depth == 16) {
    for(i=0;i<65536;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;
      g = (g << 1) | (g >> 4);
      color_lookup_table[i] = (r << 11) | (g << 5) | (b);
    }
  } else if(color_depth == 32) {
    for(i=0;i<65536;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;
      r = (r << 3) | (r >> 2);
      g = (g << 3) | (g >> 2);
      b = (b << 3) | (b >> 2);
      color_lookup_table[i] = (r << 16) | (g << 8) | (b);
    }
  } else {
    alert("Error: Unsupported color depth [%d]", color_depth);
  }
}

void render::set_window(HWND hwnd_handle) { hwnd = hwnd_handle; }

void render::to_windowed() {
  destroy();
  DirectDrawCreate(0, &lpdd, 0);
  lpdd->SetCooperativeLevel(hwnd, DDSCL_NORMAL);

  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof(DDSURFACEDESC);
  ddsd.dwFlags = DDSD_CAPS;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
  lpdd->CreateSurface(&ddsd, &lpdds, 0);

  lpdd->CreateClipper(0, &lpddc, 0);
  lpddc->SetHWnd(0, hwnd);
  lpdds->SetClipper(lpddc);

  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof(DDSURFACEDESC);
  ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
  ddsd.dwWidth  = 512;
  ddsd.dwHeight = 478;
  lpdd->CreateSurface(&ddsd, &lpddsb, 0);

  update_color_lookup_table();
  update();
}

void render::to_fullscreen() {
  destroy();
  DirectDrawCreate(0, &lpdd, 0);
  lpdd->SetCooperativeLevel(hwnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);
  lpdd->SetDisplayMode(640, 480, 16);

  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof(DDSURFACEDESC);
  ddsd.dwFlags = DDSD_CAPS;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
  lpdd->CreateSurface(&ddsd, &lpdds, 0);

  lpdd->CreateClipper(0, &lpddc, 0);
  lpddc->SetHWnd(0, hwnd);
  lpdds->SetClipper(lpddc);

  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
  ddsd.dwSize = sizeof(DDSURFACEDESC);
  ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
  ddsd.dwWidth  = 512;
  ddsd.dwHeight = 478;
  lpdd->CreateSurface(&ddsd, &lpddsb, 0);

  update_color_lookup_table();
  update();
}

void render::redraw() {
RECT rd, rs;
POINT p;
HRESULT hr;
  p.x = p.y = 0;
  ClientToScreen(hwnd, &p);
  GetClientRect(hwnd, &rd);
  OffsetRect(&rd, p.x, p.y);
  if(clock->overscan() == false) {
    SetRect(&rs, 0, 2, 512, 448);
  } else {
    SetRect(&rs, 0, 2 + 15, 512, 448 + 15);
  }
  hr = lpdds->Blt(&rd, lpddsb, &rs, DDBLT_WAIT, 0);
  if(hr == DDERR_SURFACELOST) {
    lpdds->Restore();
    lpddsb->Restore();
  }
}

void render::update16() {
HRESULT hr;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;

uint16 *_src_data  = (uint16*)ppu->output;
uint16 *_dest_data = (uint16*)ddsd.lpSurface;
uint32  _pitch     = ddsd.lPitch;
  __asm {
	mov edi,_dest_data
	mov edx,_pitch
	add edi,edx
	add edi,edx
	sub edx,1024
	mov esi,_src_data
	add esi,2048
	mov ebx,478-2
	xor eax,eax
	loop_y:
		mov ecx,512
		loop_x:
			lodsw
			mov eax,dword ptr[color_lookup_table+eax*4]
			stosw
			dec ecx
			jnz loop_x
		add edi,edx
		dec ebx
		jnz loop_y
  }

  lpddsb->Unlock(0);
}

void render::update32() {
HRESULT hr;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;

uint16 *_src_data  = (uint16*)ppu->output;
uint32 *_dest_data = (uint32*)ddsd.lpSurface;
uint32  _pitch     = ddsd.lPitch;
  __asm {
	mov edi,_dest_data
	mov edx,_pitch
	add edi,edx
	add edi,edx
	sub edx,2048
	mov esi,_src_data
	add esi,2048
	mov ebx,478-2
	loop_y:
		mov ecx,512
		loop_x:
			lodsw
			and eax,0xffff
			mov eax,dword ptr[color_lookup_table+eax*4]
			stosd
			dec ecx
			jnz loop_x
		add edi,edx
		dec ebx
		jnz loop_y
  }

  lpddsb->Unlock(0);
}

void render::update() {
  switch(color_depth) {
  case 15:
  case 16:
    update16();
    break;
  case 32:
    update32();
    break;
  }
  redraw();
}

void render::destroy() {
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
