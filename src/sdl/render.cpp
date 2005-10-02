void render16() {
uint16 *dest, *src;
uint32 pitch;
int x, y;

SNES::video_info vi;
  snes->get_video_info(&vi);

  pitch = (backbuffer->pitch >> 1);
  dest  = (uint16*)backbuffer->pixels;
  src   = (uint16*)vi.data;

  if(vi.width == 256 && vi.height == 224) {
    for(y=0;y<224;y++) {
      memcpy(dest, src, 512);
      dest += pitch;
      src  += 256;
    }
  } else if(vi.width == 512 && vi.height == 224) {
    for(y=0;y<224;y++) {
      memcpy(dest, src, 1024);
      dest += pitch;
      src  += 512;
    }
  } else if(vi.width == 256 && vi.height == 448) {
    for(y=0;y<448;y++) {
      memcpy(dest, src, 512);
      dest += pitch;
      src  += 256;
    }
  } else if(vi.width == 512 && vi.height == 448) {
    for(y=0;y<448;y++) {
      memcpy(dest, src, 1024);
      dest += pitch;
      src  += 512;
    }
  }


  screen_info.rs.x = 0;
  screen_info.rs.y = (vi.height == 224) ?   1 :   2;
  screen_info.rs.w = vi.width;
  screen_info.rs.h = (vi.height == 224) ? 223 : 446;
}

void render32() {}

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
