void render() {
SNES::video_info vi;
  snes.get_video_info(&vi);

SDL_Rect rect;
  rect.x = 0;
  rect.y = (vi.height == 224) ?   1 :   2;
  rect.w = vi.width;
  rect.h = (vi.height == 224) ? 223 : 446;

//documentation says not to use this, but it's rather ridiculous that a graphics
//library wouldn't support simple image scaling... so let's use it anyway and see
//what happens...
  SDL_SoftStretch(backbuffer, &rect, screen, &screen_rect);
//SDL_BlitSurface(backbuffer, &rect, screen, &screen_rect);

  SDL_UpdateRect(screen, screen_rect.x, screen_rect.y, screen_rect.w, screen_rect.h);
}
