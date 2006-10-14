uint32 bSNES::get_state() {
  return state;
}

void bSNES::set_state(uint32 new_state) {
  state = new_state;

  switch(state) {
  case RUN:
    uiVideo->pause_disable();
    SetWindowText(wMain.hwnd, BSNES_TITLE);
    break;
  case STOP:
    uiVideo->pause_enable();
    uiAudio->clear_audio();
    SetWindowText(wMain.hwnd, BSNES_TITLE " (Paused)");
    break;
  }

  wDebug.SetState(state);
}

void bSNES::power() {
  SNES::power();
}

void bSNES::reset() {
  SNES::reset();
}

void bSNES::scanline() {
  SNES::scanline();
}

void bSNES::run() {
  if(!r_mem->cart_loaded()) {
    Sleep(1);
    return;
  }

  switch(state) {
  case RUN:
    SNES::runtoframe();
    break;
  case STOP:
    Sleep(1);
    break;
  }
}

void bSNES::video_run() {
//temporary code to try and fix input buffer stalls
static int sync_counter = 0;
  if(++sync_counter >= 30) {
    Sleep(1);
    sync_counter = 0;
  }
//remove above code after testing

  if(r_ppu->status.frames_updated) {
  char s[512], t[512];
    r_ppu->status.frames_updated = false;
    if((bool)config::misc.show_fps == true) {
      sprintf(s, "%s : %d fps", BSNES_TITLE, r_ppu->status.frames_executed);
      if(wMain.frameskip != 0) {
        sprintf(t, " (%d frames)", r_ppu->status.frames_rendered);
        strcat(s, t);
      }
      SetWindowText(wMain.hwnd, s);
    }
  }

  wMain.frameskip_pos++;
  wMain.frameskip_pos %= (wMain.frameskip + 1);
  if(r_ppu->renderer_enabled())uiVideo->update();
  r_ppu->enable_renderer(wMain.frameskip_pos == 0);
}

void bSNES::sound_run(uint32 data) {
  uiAudio->run(data);
}

/***********************
 *** video functions ***
 ***********************/

uint16 *bSNES::video_lock(uint &pitch) {
  return uiVideo->lock(pitch);
}

void bSNES::video_unlock() {
  uiVideo->unlock();
}

/***********************
 *** input functions ***
 ***********************/

void bSNES::poll_input() {
  uiInput->poll();
  SNES::poll_input();
}

bool bSNES::get_input_status(uint8 deviceid, uint8 button) {
  return uiInput->get_status(deviceid, button);
}
