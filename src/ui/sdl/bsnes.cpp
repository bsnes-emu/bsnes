void   bSNES::set_status(uint32 new_status) { run_status = new_status; }
uint32 bSNES::get_status()                  { return run_status; }

void bSNES::run() {
  switch(run_status) {
  case RUN:
    SNES::runtoframe();
    return;
  case STOP:
    break;
  }
}

void bSNES::video_run() {
  if(r_ppu->status.frames_updated) {
  char s[512], t[512];
    r_ppu->status.frames_updated = false;
    sprintf(s, "%s : %d fps", BSNES_TITLE, r_ppu->status.frames_executed);
    SDL_WM_SetCaption(s, 0);
  }

  render();
}

void bSNES::sound_run(uint32 data) {
  uiAudio->run(data);
}

uint16 *bSNES::video_lock(uint &pitch) {
  if(SDL_MUSTLOCK(screen)) {
    SDL_LockSurface(screen);
  }
  if(SDL_MUSTLOCK(backbuffer)) {
    SDL_LockSurface(backbuffer);
  }

  pitch = backbuffer->pitch;
  return (uint16*)backbuffer->pixels;
}

void bSNES::video_unlock() {
  if(SDL_MUSTLOCK(backbuffer)) {
    SDL_UnlockSurface(backbuffer);
  }
  if(SDL_MUSTLOCK(screen)) {
    SDL_UnlockSurface(screen);
  }
}

/***********************
 *** Input functions ***
 ***********************/

void bSNES::poll_input(uint8 type) {
  uiInput->poll();
}

bool bSNES::get_input_status(uint8 device, uint8 button) {
  return uiInput->get_status(device, button);
}

void bSNES::notify(uint32 message, uint32 param1, uint32 param2) {}

bSNES::bSNES() {
  run_status = RUN;
}
