void render() {
SNES::video_info vi;
  snes->get_video_info(&vi);

  screen_info.rs.x = 0;
  screen_info.rs.y = (vi.height == 224) ?   1 :   2;
  screen_info.rs.w = vi.width;
  screen_info.rs.h = (vi.height == 224) ? 223 : 446;

//documentation says not to use this, but it's rather ridiculous that a graphics
//library wouldn't support simple image scaling... so let's use it anyway and see
//what happens...
  SDL_SoftStretch(backbuffer, &screen_info.rs, screen, &screen_info.rd);
//SDL_BlitSurface(backbuffer, &screen_info.rs, screen, &screen_info.rd);

  SDL_UpdateRect(screen, screen_info.rd.x, screen_info.rd.y, screen_info.rd.w, screen_info.rd.h);
}
