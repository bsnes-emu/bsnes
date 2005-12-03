uint32 bSNES::get_state() { return state; }
void bSNES::set_state(uint32 new_state) { state = new_state; }

void bSNES::power() {
  SNES::power();
}

void bSNES::reset() {
  SNES::reset();
}

void bSNES::scanline() {
  SNES::scanline();
  uiVideo->scanline();
}

void bSNES::run() {
  if(!r_mem->cart_loaded() || state == PAUSE) {
    Sleep(1);
    return;
  }

  SNES::runtoframe();
}

void bSNES::video_run() {
  if(r_ppu->status.frames_updated) {
  char s[512], t[512];
    r_ppu->status.frames_updated = false;
    if((bool)config::gui.show_fps == true) {
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

uint16 *bSNES::video_lock(uint32 &pitch) {
  return uiVideo->lock(pitch);
}

void bSNES::video_unlock() {
  uiVideo->unlock();
}

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
