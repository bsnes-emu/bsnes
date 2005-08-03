void   bSNES::set_status(uint32 new_status) { run_status = new_status; }
uint32 bSNES::get_status()                  { return run_status; }

void bSNES::snes_run() {
  if(!rom_image->loaded())return;

  switch(run_status) {
  case RUN:
    while(update_frame == false) {
      run();
    }
    update_frame = false;
    render();
    return;
  case STOP:
    break;
  }
}

void bSNES::render_frame() {}

/***********************
 *** Input functions ***
 ***********************/

//It would appear that keystate does not need to be free'd
//after calling SDL_GetKeyState... doing so causes libSDL
//to throw error messages about a bad free call to stdout...
void bSNES::poll_input() {
uint8 *keystate = SDL_GetKeyState(0);
  joypad1.up     = keystate[cfg.input.joypad1.up];
  joypad1.down   = keystate[cfg.input.joypad1.down];
  joypad1.left   = keystate[cfg.input.joypad1.left];
  joypad1.right  = keystate[cfg.input.joypad1.right];
  joypad1.select = keystate[cfg.input.joypad1.select];
  joypad1.start  = keystate[cfg.input.joypad1.start];
  joypad1.y      = keystate[cfg.input.joypad1.y];
  joypad1.b      = keystate[cfg.input.joypad1.b];
  joypad1.x      = keystate[cfg.input.joypad1.x];
  joypad1.a      = keystate[cfg.input.joypad1.a];
  joypad1.l      = keystate[cfg.input.joypad1.l];
  joypad1.r      = keystate[cfg.input.joypad1.r];

//It's impossible to hold both up+down, or left+right down
//at the same time on a directional pad; and besides, allowing
//this to happen causes glitches in many SNES games.
  if(joypad1.up)  joypad1.down  = 0;
  if(joypad1.left)joypad1.right = 0;
}

bool bSNES::get_input_status(uint8 device, uint8 button) {
  switch(device) {
  case DEV_JOYPAD1:
    switch(button) {
    case JOYPAD_UP:    return joypad1.up;
    case JOYPAD_DOWN:  return joypad1.down;
    case JOYPAD_LEFT:  return joypad1.left;
    case JOYPAD_RIGHT: return joypad1.right;
    case JOYPAD_A:     return joypad1.a;
    case JOYPAD_B:     return joypad1.b;
    case JOYPAD_X:     return joypad1.x;
    case JOYPAD_Y:     return joypad1.y;
    case JOYPAD_L:     return joypad1.l;
    case JOYPAD_R:     return joypad1.r;
    case JOYPAD_SELECT:return joypad1.select;
    case JOYPAD_START: return joypad1.start;
    }
    break;
  }
  return 0;
}

bJoypad::bJoypad() {
  up = down = left = right = false;
  a = b = x = y = false;
  l = r = select = start = false;
}

void bSNES::notify(uint32 message, uint32 param1, uint32 param2) {
  switch(message) {
  case RENDER_FRAME:
    update_frame = true;
    break;
  }
}

bSNES::bSNES() {
  run_status   = STOP;
  update_frame = false;
}
