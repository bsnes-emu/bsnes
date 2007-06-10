class InputManager { public:
  struct Joypad {
    uint16 up, down, left, right, a, b, x, y, l, r, select, start;
  } joypad1, joypad2;
  struct Joystat {
    bool   up, down, left, right, a, b, x, y, l, r, select, start;
  } joystat1, joystat2;

  uint16 scan();
  void bind();
  void poll();
  bool get_status(uint device, uint button);
} input_manager;

//search all key bindings, return keymap::none if no keys are active
uint16 InputManager::scan() {
  uiInput->poll();
  for(uint i = 0; i < keymap::limit; i++) {
    if(uiInput->key_down(i)) { return i; }
  }
  for(uint j = 0; j < 16; j++) {
    if(uiInput->key_down(keymap::joypad_flag | (j << 16) | keymap::joypad_up)) {
      return (keymap::joypad_flag | (j << 16) | keymap::joypad_up);
    }
    if(uiInput->key_down(keymap::joypad_flag | (j << 16) | keymap::joypad_down)) {
      return (keymap::joypad_flag | (j << 16) | keymap::joypad_down);
    }
    if(uiInput->key_down(keymap::joypad_flag | (j << 16) | keymap::joypad_left)) {
      return (keymap::joypad_flag | (j << 16) | keymap::joypad_left);
    }
    if(uiInput->key_down(keymap::joypad_flag | (j << 16) | keymap::joypad_right)) {
      return (keymap::joypad_flag | (j << 16) | keymap::joypad_right);
    }
    for(uint i = 0; i < 128; i++) {
      if(uiInput->key_down(keymap::joypad_flag | (j << 16) | i)) {
        return (keymap::joypad_flag | (j << 16) | i);
      }
    }
  }
  return keymap::none;
}

void InputManager::bind() {
  joypad1.up     = keymap::find(config::input.joypad1.up);
  joypad1.down   = keymap::find(config::input.joypad1.down);
  joypad1.left   = keymap::find(config::input.joypad1.left);
  joypad1.right  = keymap::find(config::input.joypad1.right);
  joypad1.a      = keymap::find(config::input.joypad1.a);
  joypad1.b      = keymap::find(config::input.joypad1.b);
  joypad1.x      = keymap::find(config::input.joypad1.x);
  joypad1.y      = keymap::find(config::input.joypad1.y);
  joypad1.l      = keymap::find(config::input.joypad1.l);
  joypad1.r      = keymap::find(config::input.joypad1.r);
  joypad1.select = keymap::find(config::input.joypad1.select);
  joypad1.start  = keymap::find(config::input.joypad1.start);

  joystat1.up = joystat1.down = joystat1.left   = joystat1.right =
  joystat1.a  = joystat1.b    = joystat1.x      = joystat1.y     =
  joystat1.l  = joystat1.r    = joystat1.select = joystat1.start = false;

  joypad2.up     = keymap::find(config::input.joypad2.up);
  joypad2.down   = keymap::find(config::input.joypad2.down);
  joypad2.left   = keymap::find(config::input.joypad2.left);
  joypad2.right  = keymap::find(config::input.joypad2.right);
  joypad2.a      = keymap::find(config::input.joypad2.a);
  joypad2.b      = keymap::find(config::input.joypad2.b);
  joypad2.x      = keymap::find(config::input.joypad2.x);
  joypad2.y      = keymap::find(config::input.joypad2.y);
  joypad2.l      = keymap::find(config::input.joypad2.l);
  joypad2.r      = keymap::find(config::input.joypad2.r);
  joypad2.select = keymap::find(config::input.joypad2.select);
  joypad2.start  = keymap::find(config::input.joypad2.start);

  joystat2.up = joystat2.down = joystat2.left   = joystat2.right =
  joystat2.a  = joystat2.b    = joystat2.x      = joystat2.y     =
  joystat2.l  = joystat2.r    = joystat2.select = joystat2.start = false;
}

void InputManager::poll() {
  joystat1.up     = uiInput->key_down(joypad1.up);
  joystat1.down   = uiInput->key_down(joypad1.down);
  joystat1.left   = uiInput->key_down(joypad1.left);
  joystat1.right  = uiInput->key_down(joypad1.right);
  joystat1.a      = uiInput->key_down(joypad1.a);
  joystat1.b      = uiInput->key_down(joypad1.b);
  joystat1.x      = uiInput->key_down(joypad1.x);
  joystat1.y      = uiInput->key_down(joypad1.y);
  joystat1.l      = uiInput->key_down(joypad1.l);
  joystat1.r      = uiInput->key_down(joypad1.r);
  joystat1.select = uiInput->key_down(joypad1.select);
  joystat1.start  = uiInput->key_down(joypad1.start);

  joystat2.up     = uiInput->key_down(joypad2.up);
  joystat2.down   = uiInput->key_down(joypad2.down);
  joystat2.left   = uiInput->key_down(joypad2.left);
  joystat2.right  = uiInput->key_down(joypad2.right);
  joystat2.a      = uiInput->key_down(joypad2.a);
  joystat2.b      = uiInput->key_down(joypad2.b);
  joystat2.x      = uiInput->key_down(joypad2.x);
  joystat2.y      = uiInput->key_down(joypad2.y);
  joystat2.l      = uiInput->key_down(joypad2.l);
  joystat2.r      = uiInput->key_down(joypad2.r);
  joystat2.select = uiInput->key_down(joypad2.select);
  joystat2.start  = uiInput->key_down(joypad2.start);
}

bool InputManager::get_status(uint device, uint button) {
  switch(device) {
  case SNES::DEVICEID_JOYPAD1:
    switch(button) {
    case SNES::JOYPAD_UP:     return joystat1.up;
    case SNES::JOYPAD_DOWN:   return joystat1.down;
    case SNES::JOYPAD_LEFT:   return joystat1.left;
    case SNES::JOYPAD_RIGHT:  return joystat1.right;
    case SNES::JOYPAD_A:      return joystat1.a;
    case SNES::JOYPAD_B:      return joystat1.b;
    case SNES::JOYPAD_X:      return joystat1.x;
    case SNES::JOYPAD_Y:      return joystat1.y;
    case SNES::JOYPAD_L:      return joystat1.l;
    case SNES::JOYPAD_R:      return joystat1.r;
    case SNES::JOYPAD_SELECT: return joystat1.select;
    case SNES::JOYPAD_START:  return joystat1.start;
    }
    break;
  case SNES::DEVICEID_JOYPAD2:
    switch(button) {
    case SNES::JOYPAD_UP:     return joystat2.up;
    case SNES::JOYPAD_DOWN:   return joystat2.down;
    case SNES::JOYPAD_LEFT:   return joystat2.left;
    case SNES::JOYPAD_RIGHT:  return joystat2.right;
    case SNES::JOYPAD_A:      return joystat2.a;
    case SNES::JOYPAD_B:      return joystat2.b;
    case SNES::JOYPAD_X:      return joystat2.x;
    case SNES::JOYPAD_Y:      return joystat2.y;
    case SNES::JOYPAD_L:      return joystat2.l;
    case SNES::JOYPAD_R:      return joystat2.r;
    case SNES::JOYPAD_SELECT: return joystat2.select;
    case SNES::JOYPAD_START:  return joystat2.start;
    }
    break;
  }

  return false;
}
