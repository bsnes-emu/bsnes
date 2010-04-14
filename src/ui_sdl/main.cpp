#include <snes/libsnes/libsnes.hpp>
#include "SDL.h"

SDL_Surface *screen, *backbuffer;

void video_refresh(const uint16_t *data, unsigned width, unsigned height) {
  if(SDL_MUSTLOCK(screen)) SDL_LockSurface(screen);
  if(SDL_MUSTLOCK(backbuffer)) SDL_LockSurface(backbuffer);

  unsigned outputPitch = backbuffer->pitch;
  uint16_t *outputData = (uint16_t*)backbuffer->pixels;

  for(unsigned y = 0; y < height; y++) {
    uint16_t *src = (uint16_t*)((uint8_t*)data + y * 2048);
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
    if(device == SNES_DEVICE_JOYPAD) {
      if(id == SNES_DEVICE_ID_JOYPAD_B) return state[SDLK_z];
      if(id == SNES_DEVICE_ID_JOYPAD_Y) return state[SDLK_a];
      if(id == SNES_DEVICE_ID_JOYPAD_SELECT) return state[SDLK_RSHIFT];
      if(id == SNES_DEVICE_ID_JOYPAD_START) return state[SDLK_RETURN];
      if(id == SNES_DEVICE_ID_JOYPAD_UP) return state[SDLK_UP];
      if(id == SNES_DEVICE_ID_JOYPAD_DOWN) return state[SDLK_DOWN] & !state[SDLK_UP];
      if(id == SNES_DEVICE_ID_JOYPAD_LEFT) return state[SDLK_LEFT];
      if(id == SNES_DEVICE_ID_JOYPAD_RIGHT) return state[SDLK_RIGHT] & !state[SDLK_LEFT];
      if(id == SNES_DEVICE_ID_JOYPAD_A) return state[SDLK_x];
      if(id == SNES_DEVICE_ID_JOYPAD_X) return state[SDLK_s];
      if(id == SNES_DEVICE_ID_JOYPAD_L) return state[SDLK_d];
      if(id == SNES_DEVICE_ID_JOYPAD_R) return state[SDLK_c];
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  if(argc != 2) return 0;

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

  FILE *fp = fopen(argv[1], "rb");
  fseek(fp, 0, SEEK_END);
  unsigned rom_size = ftell(fp);
  rewind(fp);
  if((rom_size & 0x7fff) == 512) {
    rom_size -= 512;
    fseek(fp, 512, SEEK_SET);
  }
  uint8_t *rom_data = new uint8_t[rom_size];
  rom_size = fread(rom_data, 1, rom_size, fp);
  fclose(fp);

  snes_load_cartridge_normal(0, rom_data, rom_size);
  delete[] rom_data;

  unsigned serial_size = snes_serialize_size();
  uint8_t *serial_data = new uint8_t[serial_size];
  snes_serialize(serial_data, serial_size);

//snes_cheat_set(0, true, "DD32-6DAD");

  while(true) {
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT) break;
    if(event.type == SDL_KEYDOWN) {
      if(event.key.keysym.sym == SDLK_ESCAPE) {
        break;
      } else if(event.key.keysym.sym == SDLK_F2) {
        snes_serialize(serial_data, serial_size);
      } else if(event.key.keysym.sym == SDLK_F4) {
        snes_unserialize(serial_data, serial_size);
      }
    }
    snes_run();
  }

  delete[] serial_data;
  snes_unload();
  snes_term();
  return 0;
}
