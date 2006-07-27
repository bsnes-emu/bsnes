void InputDI::poll() {
HRESULT hr;
DIJOYSTATE2 js;
  memset(keystate, 0, 256);
  if(di_key) {
    hr = di_key->GetDeviceState(256, keystate);
    if(FAILED(hr)) {
      di_key->Acquire();
      di_key->GetDeviceState(256, keystate);
    }
  }

  memset(joystate, 0, INPUT_JOYMAX * 256);
  for(int i = 0; i < di_joy_count; i++) {
    if(!di_joy[i])continue;

    memset(js.rgbButtons, 0, 128);

    hr = di_joy[i]->Poll();
    if(FAILED(hr)) {
      di_joy[i]->Acquire();
      di_joy[i]->Poll();
    }
    di_joy[i]->GetDeviceState(sizeof(DIJOYSTATE2), &js);

  uint32 index = i * 256; //joypad index
    memcpy(joystate + index, js.rgbButtons, 128);
  //map d-pad axes to joystate[index + {128 - 131}]
  int32 resistance = config::input.axis_resistance;
    if(resistance <  1)resistance =  1;
    if(resistance > 99)resistance = 99;
    resistance = int32(double(resistance) * 32768.0 / 100.0);
  int32 resistance_lo = 0x7fff - resistance;
  int32 resistance_hi = 0x8000 + resistance;
    joystate[index + 128]  = (js.lY <= resistance_lo) ? 0x80 : 0x00;
    joystate[index + 129]  = (js.lY >= resistance_hi) ? 0x80 : 0x00;
    joystate[index + 130]  = (js.lX <= resistance_lo) ? 0x80 : 0x00;
    joystate[index + 131]  = (js.lX >= resistance_hi) ? 0x80 : 0x00;

  //map analog POV (analog directional pad) as well
  uint32 pov = js.rgdwPOV[0];
    joystate[index + 128] |= (pov ==     0 || pov == 31500 || pov ==  4500) ? 0x80 : 0x00;
    joystate[index + 129] |= (pov == 18000 || pov == 13500 || pov == 22500) ? 0x80 : 0x00;
    joystate[index + 130] |= (pov == 27000 || pov == 22500 || pov == 31500) ? 0x80 : 0x00;
    joystate[index + 131] |= (pov ==  9000 || pov ==  4500 || pov == 13500) ? 0x80 : 0x00;
  }
}

bool InputDI::button_down(uint32 r) {
  if((r & JOYMASK) != JOY_NONE) {
  uint8 joynum = (r >> 16) & 0xff;
    joynum %= INPUT_JOYMAX;
    if(joystate[(joynum * 256) + ((r >> 8) & 0xff)] & 0x80) {
      return true;
    }
  }

  if((r & KEYMASK) != KEY_NONE) {
  //keypad value defined
    if(keystate[r & 0xff] & 0x80) {
      return true;
    }
  }

  return false;
}

void InputDI::poll(uint8 type) {
  poll();

HWND fw = GetForegroundWindow();
#define poll_key(__key) \
  __key = (fw == wMain.hwnd) ? button_down(uint32(config::input.__key)) : 0

  switch(type) {
  case SNES::DEV_JOYPAD1:
    poll_key(joypad1.up);
    poll_key(joypad1.down);
    poll_key(joypad1.left);
    poll_key(joypad1.right);
    poll_key(joypad1.a);
    poll_key(joypad1.b);
    poll_key(joypad1.x);
    poll_key(joypad1.y);
    poll_key(joypad1.l);
    poll_key(joypad1.r);
    poll_key(joypad1.select);
    poll_key(joypad1.start);
    break;
  case SNES::DEV_JOYPAD2:
    poll_key(joypad2.up);
    poll_key(joypad2.down);
    poll_key(joypad2.left);
    poll_key(joypad2.right);
    poll_key(joypad2.a);
    poll_key(joypad2.b);
    poll_key(joypad2.x);
    poll_key(joypad2.y);
    poll_key(joypad2.l);
    poll_key(joypad2.r);
    poll_key(joypad2.select);
    poll_key(joypad2.start);
    break;
  }

#undef poll_key
}

bool InputDI::get_status(uint8 device, uint8 button) {
  switch(device) {
  case SNES::DEV_JOYPAD1:
    switch(button) {
    case SNES::JOYPAD_UP:     return joypad1.up;
    case SNES::JOYPAD_DOWN:   return joypad1.down;
    case SNES::JOYPAD_LEFT:   return joypad1.left;
    case SNES::JOYPAD_RIGHT:  return joypad1.right;
    case SNES::JOYPAD_A:      return joypad1.a;
    case SNES::JOYPAD_B:      return joypad1.b;
    case SNES::JOYPAD_X:      return joypad1.x;
    case SNES::JOYPAD_Y:      return joypad1.y;
    case SNES::JOYPAD_L:      return joypad1.l;
    case SNES::JOYPAD_R:      return joypad1.r;
    case SNES::JOYPAD_SELECT: return joypad1.select;
    case SNES::JOYPAD_START:  return joypad1.start;
    }
    break;
  case SNES::DEV_JOYPAD2:
    switch(button) {
    case SNES::JOYPAD_UP:     return joypad2.up;
    case SNES::JOYPAD_DOWN:   return joypad2.down;
    case SNES::JOYPAD_LEFT:   return joypad2.left;
    case SNES::JOYPAD_RIGHT:  return joypad2.right;
    case SNES::JOYPAD_A:      return joypad2.a;
    case SNES::JOYPAD_B:      return joypad2.b;
    case SNES::JOYPAD_X:      return joypad2.x;
    case SNES::JOYPAD_Y:      return joypad2.y;
    case SNES::JOYPAD_L:      return joypad2.l;
    case SNES::JOYPAD_R:      return joypad2.r;
    case SNES::JOYPAD_SELECT: return joypad2.select;
    case SNES::JOYPAD_START:  return joypad2.start;
    }
    break;
  }

  return false;
}

void InputDI::set_status(uint8 device, uint8 button, bool status) {
  switch(device) {
  case SNES::DEV_JOYPAD1:
    switch(button) {
    case SNES::JOYPAD_UP:     joypad1.up     = status; break;
    case SNES::JOYPAD_DOWN:   joypad1.down   = status; break;
    case SNES::JOYPAD_LEFT:   joypad1.left   = status; break;
    case SNES::JOYPAD_RIGHT:  joypad1.right  = status; break;
    case SNES::JOYPAD_A:      joypad1.a      = status; break;
    case SNES::JOYPAD_B:      joypad1.b      = status; break;
    case SNES::JOYPAD_X:      joypad1.x      = status; break;
    case SNES::JOYPAD_Y:      joypad1.y      = status; break;
    case SNES::JOYPAD_L:      joypad1.l      = status; break;
    case SNES::JOYPAD_R:      joypad1.r      = status; break;
    case SNES::JOYPAD_SELECT: joypad1.select = status; break;
    case SNES::JOYPAD_START:  joypad1.start  = status; break;
    } break;
  case SNES::DEV_JOYPAD2:
    switch(button) {
    case SNES::JOYPAD_UP:     joypad2.up     = status; break;
    case SNES::JOYPAD_DOWN:   joypad2.down   = status; break;
    case SNES::JOYPAD_LEFT:   joypad2.left   = status; break;
    case SNES::JOYPAD_RIGHT:  joypad2.right  = status; break;
    case SNES::JOYPAD_A:      joypad2.a      = status; break;
    case SNES::JOYPAD_B:      joypad2.b      = status; break;
    case SNES::JOYPAD_X:      joypad2.x      = status; break;
    case SNES::JOYPAD_Y:      joypad2.y      = status; break;
    case SNES::JOYPAD_L:      joypad2.l      = status; break;
    case SNES::JOYPAD_R:      joypad2.r      = status; break;
    case SNES::JOYPAD_SELECT: joypad2.select = status; break;
    case SNES::JOYPAD_START:  joypad2.start  = status; break;
    } break;
  }
}

void InputDI::clear_input() {
  joypad1.up     = joypad2.up     =
  joypad1.down   = joypad2.down   =
  joypad1.left   = joypad2.left   =
  joypad1.right  = joypad2.right  =
  joypad1.a      = joypad2.a      =
  joypad1.b      = joypad2.b      =
  joypad1.x      = joypad2.x      =
  joypad1.y      = joypad2.y      =
  joypad1.l      = joypad2.l      =
  joypad1.r      = joypad2.r      =
  joypad1.select = joypad2.select =
  joypad1.start  = joypad2.start  = false;
}

BOOL CALLBACK DI_EnumJoypadsCallback(const DIDEVICEINSTANCE *instance, void *context) {
  return static_cast<InputDI*>(uiInput)->enum_joypads(instance);
}

bool InputDI::enum_joypads(const DIDEVICEINSTANCE *instance) {
HRESULT hr = di->CreateDevice(instance->guidInstance, &di_joy[di_joy_count], 0);
  if(FAILED(hr)) {
    return DIENUM_CONTINUE;
  }

  di_joy[di_joy_count]->SetDataFormat(&c_dfDIJoystick2);
  di_joy[di_joy_count]->SetCooperativeLevel(wMain.hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

  if(++di_joy_count >= INPUT_JOYMAX) {
  //too many joypads?
    return DIENUM_STOP;
  }

  return DIENUM_CONTINUE;
}

void InputDI::init() {
  di_key = 0;
  for(int i = 0; i < INPUT_JOYMAX; i++)di_joy[i] = 0;
  di = 0;
  di_joy_count = 0;

  clear_input();

  DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION,
    IID_IDirectInput8, (void**)&di, 0);
  di->CreateDevice(GUID_SysKeyboard, &di_key, 0);

  di_key->SetDataFormat(&c_dfDIKeyboard);
  di_key->SetCooperativeLevel(wMain.hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
  di_key->Acquire();

  di->EnumDevices(DI8DEVCLASS_GAMECTRL, DI_EnumJoypadsCallback, 0, DIEDFL_ATTACHEDONLY);

  #include "dinput_keymap.cpp"

  Input::init();
}

void InputDI::term() {
  if(di_key) { di_key->Unacquire(); di_key->Release(); di_key = 0; }
  for(int i = 0; i < INPUT_JOYMAX; i++) {
    if(di_joy[i]) { di_joy[i]->Unacquire(); di_joy[i]->Release(); di_joy[i] = 0; }
  }
  if(di) { di->Release(); di = 0; }
  di_joy_count = 0;

  Input::term();
}
