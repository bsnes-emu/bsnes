uint8  color_curve_table[32];
uint32 color_lookup_table[65536];

void update_color_lookup_table() {
int i, r, g, b, c;
  for(i=0,c=0;i<16;i++) {
    color_curve_table[i] = c;
    c = c + i + 1;
  }
  for(;i<31;i++) {
    color_curve_table[i] = c;
    c += 8;
  }
  color_curve_table[i] = 0xff;

int color_depth = 16;
  if(color_depth == 15) {
    for(i=0;i<65536;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;
      r = color_curve_table[r] >> 3;
      g = color_curve_table[g] >> 3;
      b = color_curve_table[b] >> 3;
      color_lookup_table[i] = (r << 10) | (g << 5) | (b);
    }
  } else if(color_depth == 16) {
    for(i=0;i<65536;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;
      r = color_curve_table[r] >> 3;
      g = color_curve_table[g] >> 2;
      b = color_curve_table[b] >> 3;
      color_lookup_table[i] = (r << 11) | (g << 5) | (b);
    }
  } else if(color_depth == 32) {
    for(i=0;i<65536;i++) {
      r = (i      ) & 31;
      g = (i >>  5) & 31;
      b = (i >> 10) & 31;
      r = color_curve_table[r];
      g = color_curve_table[g];
      b = color_curve_table[b];
      color_lookup_table[i] = (r << 16) | (g << 8) | (b);
    }
  } else {
    alert("Error: Unsupported color depth [%d]", color_depth);
  }
}

void render16() {
uint16 *src, *dest;
uint32 pitch;
int x, y;
  pitch = (backbuffer->pitch >> 1) - 256;
  src   = (uint16*)ppu->output->buffer + (1 << 10);
  dest  = (uint16*)backbuffer->pixels;
  for(y=0;y<223;y++) {
    for(x=0;x<256;x++) {
      *dest++ = color_lookup_table[*src];
      src += 2;
    }
    dest += pitch;
    src  += 512;
  }
}

void render32() {
uint16 *src;
uint32 *dest, pitch;
int x, y;
  pitch = (screen->pitch >> 2) - 256;
  src   = (uint16*)ppu->output->buffer + (1 << 10);
  dest  = (uint32*)screen->pixels;
  for(y=0;y<223;y++) {
    for(x=0;x<256;x++) {
      *dest++ = color_lookup_table[*src];
      src += 2;
    }
    dest += pitch;
    src  += 512;
  }
}

void render() {
  if(SDL_MUSTLOCK(screen)) {
    SDL_LockSurface(screen);
  }
  if(SDL_MUSTLOCK(backbuffer)) {
    SDL_LockSurface(backbuffer);
  }

  render16();

//documentation says not to use this, but it's rather ridiculous that a graphics
//library wouldn't support simple image scaling... so let's use it anyway and see
//what happens...
  SDL_SoftStretch(backbuffer, &screen_info.rs, screen, &screen_info.rd);
//SDL_BlitSurface(backbuffer, &screen_info.rs, screen, &screen_info.rd);

  if(SDL_MUSTLOCK(backbuffer)) {
    SDL_UnlockSurface(backbuffer);
  }
  if(SDL_MUSTLOCK(screen)) {
    SDL_UnlockSurface(screen);
  }

  SDL_UpdateRect(screen, screen_info.rd.x, screen_info.rd.y, screen_info.rd.w, screen_info.rd.h);
}
