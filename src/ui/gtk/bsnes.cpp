void   bSNES::set_status(uint32 new_status) { run_status = new_status; }
uint32 bSNES::get_status()                  { return run_status; }

void bSNES::run() {
  if(cartridge.loaded() == false)return;

  switch(run_status) {
  case RUN:
    SNES::runtoframe();
    return;
  case STOP:
    return;
  }
}

void bSNES::video_run() {
  if(r_ppu->status.frames_updated == true) {
  char t[256];
    r_ppu->status.frames_updated = false;
    sprintf(t, "%s : %d fps", BSNES_TITLE, r_ppu->status.frames_executed);
    gtk_window_set_title(GTK_WINDOW(main_window.window), t);
  }

  uiVideo->redraw();
}

void bSNES::sound_run(uint32 data) {
  uiAudio->run(data);
}

uint16 *bSNES::video_lock(uint &pitch) {
  return uiVideo->lock(pitch);
}

void bSNES::video_unlock() {
  uiVideo->unlock();
}

void bSNES::poll_input(uint8 type) {
  uiInput->poll(type);
}

bool bSNES::get_input_status(uint8 device, uint8 button) {
  return uiInput->get_status(device, button);
}

void bSNES::notify(uint32 message, uint32 param1, uint32 param2) {}
bSNES::bSNES() { run_status = STOP; }
