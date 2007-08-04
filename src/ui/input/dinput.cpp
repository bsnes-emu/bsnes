#include "dinput.h"

void InputDI::clear_input() {
  memset(keystate, 0, sizeof keystate);
}

void InputDI::poll() {
  clear_input();

HRESULT hr;
DIJOYSTATE2 js;
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

  uint index = keymap::joypad_flag | (i << 8); //joypad index
    memcpy(keystate + index, js.rgbButtons, 128);

  //map d-pad axes
  int resistance = config::input.axis_resistance;
    if(resistance <  1)resistance =  1;
    if(resistance > 99)resistance = 99;
    resistance = int32(double(resistance) * 32768.0 / 100.0);
  int resistance_lo = 0x7fff - resistance;
  int resistance_hi = 0x8000 + resistance;
    keystate[index + keymap::joypad_up]    = (js.lY <= resistance_lo) ? 0x80 : 0x00;
    keystate[index + keymap::joypad_down]  = (js.lY >= resistance_hi) ? 0x80 : 0x00;
    keystate[index + keymap::joypad_left]  = (js.lX <= resistance_lo) ? 0x80 : 0x00;
    keystate[index + keymap::joypad_right] = (js.lX >= resistance_hi) ? 0x80 : 0x00;

  //map analog POV (analog directional pad) as well
  uint pov = js.rgdwPOV[0];
    keystate[index + keymap::joypad_up]    |= (pov ==     0 || pov == 31500 || pov ==  4500) ? 0x80 : 0x00;
    keystate[index + keymap::joypad_down]  |= (pov == 18000 || pov == 13500 || pov == 22500) ? 0x80 : 0x00;
    keystate[index + keymap::joypad_left]  |= (pov == 27000 || pov == 22500 || pov == 31500) ? 0x80 : 0x00;
    keystate[index + keymap::joypad_right] |= (pov ==  9000 || pov ==  4500 || pov == 13500) ? 0x80 : 0x00;
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
  di_joy[di_joy_count]->SetCooperativeLevel(owner, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

  if(++di_joy_count >= DIRECTINPUT_JOYMAX) {
  //too many joypads?
    return DIENUM_STOP;
  }

  return DIENUM_CONTINUE;
}

void InputDI::init() {
  di_key = 0;
  for(int i = 0; i < DIRECTINPUT_JOYMAX; i++)di_joy[i] = 0;
  di = 0;
  di_joy_count = 0;

  DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION,
    IID_IDirectInput8, (void**)&di, 0);
  di->CreateDevice(GUID_SysKeyboard, &di_key, 0);

  di_key->SetDataFormat(&c_dfDIKeyboard);
  di_key->SetCooperativeLevel(owner, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
  di_key->Acquire();

  di->EnumDevices(DI8DEVCLASS_GAMECTRL, DI_EnumJoypadsCallback, 0, DIEDFL_ATTACHEDONLY);

  Input::init();
}

void InputDI::term() {
  if(di_key) { di_key->Unacquire(); di_key->Release(); di_key = 0; }
  for(int i = 0; i < DIRECTINPUT_JOYMAX; i++) {
    if(di_joy[i]) { di_joy[i]->Unacquire(); di_joy[i]->Release(); di_joy[i] = 0; }
  }
  if(di) { di->Release(); di = 0; }
  di_joy_count = 0;

  Input::term();
}

bool InputDI::key_down(uint16 key) {
  return keystate[translate(key)] & 0x80;
}

//translate keymap code to DirectInput code, to lookup key status in DI status table
uint16 InputDI::translate(uint16 key) {
//DI joypad codes share 1:1 mapping with keymap codes
  if(key & keymap::joypad_flag) { return key; }

  switch(key) {
  case keymap::esc: return 0x01;

  case keymap::f1:  return 0x3b;
  case keymap::f2:  return 0x3c;
  case keymap::f3:  return 0x3d;
  case keymap::f4:  return 0x3e;
  case keymap::f5:  return 0x3f;
  case keymap::f6:  return 0x40;
  case keymap::f7:  return 0x41;
  case keymap::f8:  return 0x42;
  case keymap::f9:  return 0x43;
  case keymap::f10: return 0x44;
  case keymap::f11: return 0x57;
  case keymap::f12: return 0x58;

  case keymap::print_screen: return 0xb7;
  case keymap::scroll_lock:  return 0x46;
  case keymap::pause:        return 0xc5;

  case keymap::grave: return 0x29;

  case keymap::num_1: return 0x02;
  case keymap::num_2: return 0x03;
  case keymap::num_3: return 0x04;
  case keymap::num_4: return 0x05;
  case keymap::num_5: return 0x06;
  case keymap::num_6: return 0x07;
  case keymap::num_7: return 0x08;
  case keymap::num_8: return 0x09;
  case keymap::num_9: return 0x0a;
  case keymap::num_0: return 0x0b;

  case keymap::minus:     return 0x0c;
  case keymap::equal:     return 0x0d;
  case keymap::backspace: return 0x0e;

  case keymap::ins:       return 0xd2;
  case keymap::del:       return 0xd3;
  case keymap::home:      return 0xc7;
  case keymap::end:       return 0xcf;
  case keymap::page_up:   return 0xc9;
  case keymap::page_down: return 0xd1;

  case keymap::a: return 0x1e;
  case keymap::b: return 0x30;
  case keymap::c: return 0x2e;
  case keymap::d: return 0x20;
  case keymap::e: return 0x12;
  case keymap::f: return 0x21;
  case keymap::g: return 0x22;
  case keymap::h: return 0x23;
  case keymap::i: return 0x17;
  case keymap::j: return 0x24;
  case keymap::k: return 0x25;
  case keymap::l: return 0x26;
  case keymap::m: return 0x32;
  case keymap::n: return 0x31;
  case keymap::o: return 0x18;
  case keymap::p: return 0x19;
  case keymap::q: return 0x10;
  case keymap::r: return 0x13;
  case keymap::s: return 0x1f;
  case keymap::t: return 0x14;
  case keymap::u: return 0x16;
  case keymap::v: return 0x2f;
  case keymap::w: return 0x11;
  case keymap::x: return 0x2d;
  case keymap::y: return 0x15;
  case keymap::z: return 0x2c;

  case keymap::lbracket:   return 0x1a;
  case keymap::rbracket:   return 0x1b;
  case keymap::backslash:  return 0x2b;
  case keymap::semicolon:  return 0x27;
  case keymap::apostrophe: return 0x28;
  case keymap::comma:      return 0x33;
  case keymap::period:     return 0x34;
  case keymap::slash:      return 0x35;

  case keymap::kp_1: return 0x4f;
  case keymap::kp_2: return 0x50;
  case keymap::kp_3: return 0x51;
  case keymap::kp_4: return 0x4b;
  case keymap::kp_5: return 0x4c;
  case keymap::kp_6: return 0x4d;
  case keymap::kp_7: return 0x47;
  case keymap::kp_8: return 0x48;
  case keymap::kp_9: return 0x49;
  case keymap::kp_0: return 0x52;
  case keymap::kp_decimal: return 0x53;

  case keymap::kp_plus:  return 0x4e;
  case keymap::kp_minus: return 0x4a;
  case keymap::kp_mul:   return 0x37;
  case keymap::kp_div:   return 0xb5;
  case keymap::kp_enter: return 0x9c;

  case keymap::num_lock : return 0x45;
  case keymap::caps_lock: return 0x3a;

  case keymap::up:    return 0xc8;
  case keymap::down:  return 0xd0;
  case keymap::left:  return 0xcb;
  case keymap::right: return 0xcd;

  case keymap::tab:   return 0x0f;
  case keymap::enter: return 0x1c;
  case keymap::space: return 0x39;

  case keymap::lctrl : return 0x1d;
  case keymap::rctrl : return 0x9d;
  case keymap::lalt  : return 0x38;
  case keymap::ralt  : return 0xb8;
  case keymap::lshift: return 0x2a;
  case keymap::rshift: return 0x36;
  case keymap::lsuper: return 0xdb;
  case keymap::rsuper: return 0xdc;
  case keymap::menu:   return 0xdd;
  }

  return 0x00;
}
