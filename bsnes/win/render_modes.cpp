void RenderScene16(void) {
HRESULT hr;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  __asm {
	mov edi,ddsd.lpSurface
	mov edx,ddsd.lPitch
	add edi,edx
	add edi,edx
	sub edx,1024
	mov esi,ppu.screen
	add esi,2048
	mov ebx,478-2
	xor eax,eax
	loop_y:
		mov ecx,512
		loop_x:
			lodsw
			mov eax,dword ptr[render_color_lookup+eax*4]
			stosw
			dec ecx
			jnz loop_x
		add edi,edx
		dec ebx
		jnz loop_y
  }
/*
word *screen, *src;
word pitch;
int  x, y;
  pitch = ddsd.lPitch >> 1;
  for(y=2;y<478;y++) {
    screen = (word*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<512;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
*/
  lpddsb->Unlock(0);
}

void RenderScene32(void) {
HRESULT hr;
  hr = lpddsb->Lock(0, &ddsd, DDLOCK_WAIT, 0);
  if(hr != DD_OK)return;
  __asm {
	mov edi,ddsd.lpSurface
	mov edx,ddsd.lPitch
	add edi,edx
	add edi,edx
	sub edx,2048
	mov esi,ppu.screen
	add esi,2048
	mov ebx,478-2
	loop_y:
		mov ecx,512
		loop_x:
			lodsw
			and eax,0xffff
			mov eax,dword ptr[render_color_lookup+eax*4]
			stosd
			dec ecx
			jnz loop_x
		add edi,edx
		dec ebx
		jnz loop_y
  }
/*
ulong *screen;
word  *src;
word  pitch;
int x, y;
  pitch = ddsd.lPitch >> 2;
  for(y=2;y<478;y++) {
    screen = (ulong*)ddsd.lpSurface + (pitch * y);
    src    = (word*)ppu.screen + y * 512;
    for(x=0;x<512;x++) {
      *screen++ = render_color_lookup[*src++];
    }
  }
*/
  lpddsb->Unlock(0);
}
