#include <snes/libsnes/libsnes.hpp>
#include "SDL.h"

SDL_Surface *screen, *backbuffer;

void video_refresh(uint16_t *data, unsigned pitch, unsigned *line, unsigned width, unsigned height) {
  if(SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);
  if(SDL_MUSTLOCK(backbuffer)) SDL_LockSurface(backbuffer);

  unsigned outputPitch = backbuffer->pitch;
  uint16_t *outputData = (uint16_t*)backbuffer->pixels;

  for(unsigned y = 0; y < height; y++) {
    uint16_t *src = (uint16_t*)((uint8_t*)data + y * pitch);
    uint16_t *dst = (uint16_t*)((uint8_t*)outputData + y * outputPitch);
    memcpy(dst, src, width * sizeof(uint16_t));
  }

  if(SDL_MUSTLOCK(backbuffer)) SDL_UnlockSurface(backbuffer);
  if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);

  SDL_Rect rect;
  rect.x = 0;
  rect.y = 0;
  rect.w = 256;
  rect.h = 224;
  SDL_BlitSurface(backbuffer, &rect, screen, &rect);
  SDL_UpdateRect(screen, 0, 0, 256, 224);
}

void audio_sample(uint16_t l_sample, uint16_t r_sample) {
}

void input_poll() {
}

int16_t input_state(bool port, unsigned device, unsigned index, unsigned id) {
  uint8_t *state = SDL_GetKeyState(0);

  if(port == 0) {
    //B, Y, Select, Start, Up, Down, Left, Right, A, X, L, R
    if(id ==  0) return state[SDLK_z];
    if(id ==  1) return state[SDLK_a];
    if(id ==  2) return state[SDLK_RSHIFT];
    if(id ==  3) return state[SDLK_RETURN];
    if(id ==  4) return state[SDLK_UP];
    if(id ==  5) return state[SDLK_DOWN];
    if(id ==  6) return state[SDLK_LEFT];
    if(id ==  7) return state[SDLK_RIGHT];
    if(id ==  8) return state[SDLK_x];
    if(id ==  9) return state[SDLK_s];
    if(id == 10) return state[SDLK_d];
    if(id == 11) return state[SDLK_c];
  }

  return 0;
}

int main(int argc, char *argv[]) {
  const char *filename = "/media/sdb1/root/snes_roms/zelda_us.sfc";
  const char *xml =
    "<?xml version='1.0' encoding='UTF-8'?>"
    "<cartridge region='NTSC'>"
    "  <title>The Legend of Zelda - A Link to the Past</title>"
    "  <pcb>SHVC-1A3M-30</pcb>"
    "  <rom>"
    "    <map mode='linear' address='00-7f:8000-ffff'/>"
    "    <map mode='linear' address='80-ff:8000-ffff'/>"
    "  </rom>"
    "  <ram size='2000'>"
    "    <map mode='linear' address='70-7f:0000-7fff'/>"
    "    <map mode='linear' address='f0-ff:0000-7fff'/>"
    "  </ram>"
    "</cartridge>";

  atexit(SDL_Quit);
  SDL_Init(SDL_INIT_VIDEO);
  screen = SDL_SetVideoMode(256, 224, 16, SDL_HWSURFACE);
  backbuffer = SDL_CreateRGBSurface(SDL_HWSURFACE, 256, 224, 16, 0x001f, 0x03e0, 0x7c00, 0x0000);
  SDL_WM_SetCaption("bsnes/SDL", 0);

  snes_set_video_refresh(video_refresh);
  snes_set_audio_sample(audio_sample);
  snes_set_input_poll(input_poll);
  snes_set_input_state(input_state);
  snes_init();

  FILE *fp = fopen(filename, "rb");
  fseek(fp, 0, SEEK_END);
  unsigned rom_size = ftell(fp);
  rewind(fp);
  uint8_t *rom_data = new uint8_t[rom_size];
  rom_size = fread(rom_data, 1, rom_size, fp);
  fclose(fp);
  unsigned ram_size = 0x2000;
  uint8_t *ram_data = new uint8_t[ram_size]();

  snes_load_cartridge(xml, rom_data, rom_size, ram_data, ram_size, 0, 0);
  snes_load();

  while(true) {
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT) break;
    snes_run();
  }

  snes_unload();
  snes_term();
  return 0;
}
