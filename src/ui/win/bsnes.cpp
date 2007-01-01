uint bSNES::get_state() {
  return state;
}

void bSNES::set_state(uint new_state) {
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

void bSNES::run() {
  if(!r_mem->cart_loaded()) {
    Sleep(1);
    return;
  }

  switch(state) {
  case RUN:
    snes.runtoframe();
    video_run();
    break;
  case STOP:
    Sleep(1);
    break;
  }
}

void bSNES::video_run() {
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
  r_ppu->enable_renderer(wMain.frameskip_pos == 0);
}
