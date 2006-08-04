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
 *** audio functions ***
 ***********************/

void bSNES::poll_input(uint8 type) {
  uiInput->poll(type);

  if((bool)config::input.joypad1.allow_invalid_input == false) {
    if(uiInput->get_status(DEV_JOYPAD1, JOYPAD_UP) &&
       uiInput->get_status(DEV_JOYPAD1, JOYPAD_DOWN)) {
      uiInput->set_status(DEV_JOYPAD1, JOYPAD_DOWN, false);
    }

    if(uiInput->get_status(DEV_JOYPAD1, JOYPAD_LEFT) &&
       uiInput->get_status(DEV_JOYPAD1, JOYPAD_RIGHT)) {
      uiInput->set_status(DEV_JOYPAD1, JOYPAD_RIGHT, false);
    }
  }

  if((bool)config::input.joypad2.allow_invalid_input == false) {
    if(uiInput->get_status(DEV_JOYPAD2, JOYPAD_UP) &&
       uiInput->get_status(DEV_JOYPAD2, JOYPAD_DOWN)) {
      uiInput->set_status(DEV_JOYPAD2, JOYPAD_DOWN, false);
    }

    if(uiInput->get_status(DEV_JOYPAD2, JOYPAD_LEFT) &&
       uiInput->get_status(DEV_JOYPAD2, JOYPAD_RIGHT)) {
      uiInput->set_status(DEV_JOYPAD2, JOYPAD_RIGHT, false);
    }
  }
}

bool bSNES::get_input_status(uint8 device, uint8 button) {
  return uiInput->get_status(device, button);
}

/***************************
 *** debugging functions ***
 ***************************/
void bSNES::notify(uint32 message, uint32 param1, uint32 param2) {
  debugger.notify(message, param1, param2);
}
