void InputDI::poll() {
  if(wMain.hwnd != GetForegroundWindow()) {
    clear_input();
    return;
  }
  Input::poll();
}

void InputDI::poll_hw() {
HRESULT hr;
DIJOYSTATE2 js;
  memset(keystate, 0, sizeof(keystate));

  if(di_key) {
    hr = di_key->GetDeviceState(256, keystate);
    if(FAILED(hr)) {
      di_key->Acquire();
      hr = di_key->GetDeviceState(256, keystate);
    }
  }

  for(int i = 0; i < di_joy_count; i++) {
    if(!di_joy[i])continue;

    memset(js.rgbButtons, 0, 128);

    hr = di_joy[i]->Poll();
    if(FAILED(hr)) {
      di_joy[i]->Acquire();
      di_joy[i]->Poll();
    }
    di_joy[i]->GetDeviceState(sizeof(DIJOYSTATE2), &js);

  uint index = 0x100 + i * 128; //joypad index
    memcpy(keystate + index, js.rgbButtons, 124);

  //map d-pad axes to keystate[index + {124 - 127}]
  int resistance = config::input.axis_resistance;
    if(resistance <  1)resistance =  1;
    if(resistance > 99)resistance = 99;
    resistance = int32(double(resistance) * 32768.0 / 100.0);
  int resistance_lo = 0x7fff - resistance;
  int resistance_hi = 0x8000 + resistance;
    keystate[index + 124]  = (js.lY <= resistance_lo) ? 0x80 : 0x00;
    keystate[index + 125]  = (js.lY >= resistance_hi) ? 0x80 : 0x00;
    keystate[index + 126]  = (js.lX <= resistance_lo) ? 0x80 : 0x00;
    keystate[index + 127]  = (js.lX >= resistance_hi) ? 0x80 : 0x00;

  //map analog POV (analog directional pad) as well
  uint pov = js.rgdwPOV[0];
    keystate[index + 124] |= (pov ==     0 || pov == 31500 || pov ==  4500) ? 0x80 : 0x00;
    keystate[index + 125] |= (pov == 18000 || pov == 13500 || pov == 22500) ? 0x80 : 0x00;
    keystate[index + 126] |= (pov == 27000 || pov == 22500 || pov == 31500) ? 0x80 : 0x00;
    keystate[index + 127] |= (pov ==  9000 || pov ==  4500 || pov == 13500) ? 0x80 : 0x00;
  }
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
