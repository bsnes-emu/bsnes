#include <assert.h>
#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#define DIRECTINPUT_JOYMAX 16
#include <dinput.h>

#include <ruby/ruby.h>

namespace ruby {

#include "directinput.h"

BOOL CALLBACK DI_EnumJoypadsCallback(const DIDEVICEINSTANCE*, void*);

using namespace nall;

class pInputDI {
public:
  InputDI &self;
  uint8_t keystate[256 + DIRECTINPUT_JOYMAX * 256];
  LPDIRECTINPUT8 di;
  LPDIRECTINPUTDEVICE8 di_key, di_joy[DIRECTINPUT_JOYMAX];
  unsigned di_joy_count;

  struct {
    HWND handle;
    unsigned analog_axis_resistance;
  } settings;

  bool cap(Input::Setting setting) {
    if(setting == Input::Handle) return true;
    if(setting == Input::AnalogAxisResistance) return true;
    return false;
  }

  uintptr_t get(Input::Setting setting) {
    if(setting == Input::Handle) return (uintptr_t)settings.handle;
    if(setting == Input::AnalogAxisResistance) return settings.analog_axis_resistance;
    return false;
  }

  bool set(Input::Setting setting, uintptr_t param) {
    if(setting == Input::Handle) {
      settings.handle = (HWND)param;
      return true;
    }

    if(setting == Input::AnalogAxisResistance) {
      settings.analog_axis_resistance = param;
      return true;
    }

    return false;
  }

  void clear() {
    memset(keystate, 0, sizeof keystate);
  }

  void poll() {
    clear();

    DIJOYSTATE2 js;
    if(di_key) {
      if(FAILED(di_key->GetDeviceState(256, keystate))) {
        di_key->Acquire();
        if(FAILED(di_key->GetDeviceState(256, keystate))) {
          memset(keystate, 0, 256);
        }
      }
    }

    for(int i = 0; i < di_joy_count; i++) {
      if(!di_joy[i]) continue;

      memset(js.rgbButtons, 0, 128);

      if(FAILED(di_joy[i]->Poll())) {
        di_joy[i]->Acquire();
        if(FAILED(di_joy[i]->Poll())) {
          continue;
        }
      }
      di_joy[i]->GetDeviceState(sizeof(DIJOYSTATE2), &js);

      uint16_t index = 256 + (i << 8); //joypad index
      memcpy(keystate + index, js.rgbButtons, 128);

      //map d-pad axes
      int resistance = settings.analog_axis_resistance;
      resistance = max(1, min(99, resistance));
      resistance = int(double(resistance) * 32768.0 / 100.0);
      int resistance_lo = 0x7fff - resistance;
      int resistance_hi = 0x8000 + resistance;
      keystate[index + 0x80] = (js.lY <= resistance_lo) ? 0x80 : 0x00;
      keystate[index + 0x81] = (js.lY >= resistance_hi) ? 0x80 : 0x00;
      keystate[index + 0x82] = (js.lX <= resistance_lo) ? 0x80 : 0x00;
      keystate[index + 0x83] = (js.lX >= resistance_hi) ? 0x80 : 0x00;

      //map analog POV hat (directional pad) as well
      unsigned pov = js.rgdwPOV[0];
      keystate[index + 0x80] |= (pov ==     0 || pov == 31500 || pov ==  4500) ? 0x80 : 0x00;
      keystate[index + 0x81] |= (pov == 18000 || pov == 13500 || pov == 22500) ? 0x80 : 0x00;
      keystate[index + 0x82] |= (pov == 27000 || pov == 22500 || pov == 31500) ? 0x80 : 0x00;
      keystate[index + 0x83] |= (pov ==  9000 || pov ==  4500 || pov == 13500) ? 0x80 : 0x00;
    }
  }

  bool enum_joypads(const DIDEVICEINSTANCE *instance) {
    if(FAILED(di->CreateDevice(instance->guidInstance, &di_joy[di_joy_count], 0))) {
      return DIENUM_CONTINUE; //continue and try next joypad
    }

    di_joy[di_joy_count]->SetDataFormat(&c_dfDIJoystick2);
    di_joy[di_joy_count]->SetCooperativeLevel(settings.handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

    if(++di_joy_count >= DIRECTINPUT_JOYMAX) return DIENUM_STOP;
    return DIENUM_CONTINUE;
  }

  bool init() {
    di_key = 0;
    for(int i = 0; i < DIRECTINPUT_JOYMAX; i++) di_joy[i] = 0;
    di = 0;
    di_joy_count = 0;

    DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&di, 0);
    di->CreateDevice(GUID_SysKeyboard, &di_key, 0);

    di_key->SetDataFormat(&c_dfDIKeyboard);
    di_key->SetCooperativeLevel(settings.handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
    di_key->Acquire();

    di->EnumDevices(DI8DEVCLASS_GAMECTRL, DI_EnumJoypadsCallback, (void*)this, DIEDFL_ATTACHEDONLY);

    return true;
  }

  void term() {
    if(di_key) {
      di_key->Unacquire();
      di_key->Release();
      di_key = 0;
    }

    for(int i = 0; i < DIRECTINPUT_JOYMAX; i++) {
      if(di_joy[i]) {
        di_joy[i]->Unacquire();
        di_joy[i]->Release();
        di_joy[i] = 0;
      }
    }

    if(di) {
      di->Release();
      di = 0;
    }

    di_joy_count = 0;
  }

  bool key_down(uint16_t key) {
    assert(key < sizeof keystate);
    return key ? keystate[translate(key)] & 0x80 : false;
  }

  //translate keymap code to DirectInput code, to lookup key status in DI status table
  uint16_t translate(uint16_t key) {
    switch(key) {
      case keyboard::escape: return 0x01;

      case keyboard::f1:  return 0x3b;
      case keyboard::f2:  return 0x3c;
      case keyboard::f3:  return 0x3d;
      case keyboard::f4:  return 0x3e;
      case keyboard::f5:  return 0x3f;
      case keyboard::f6:  return 0x40;
      case keyboard::f7:  return 0x41;
      case keyboard::f8:  return 0x42;
      case keyboard::f9:  return 0x43;
      case keyboard::f10: return 0x44;
      case keyboard::f11: return 0x57;
      case keyboard::f12: return 0x58;

      case keyboard::print_screen: return 0xb7;
      case keyboard::scroll_lock:  return 0x46;
      case keyboard::pause:        return 0xc5;

      case keyboard::tilde: return 0x29;

      case keyboard::num_1: return 0x02;
      case keyboard::num_2: return 0x03;
      case keyboard::num_3: return 0x04;
      case keyboard::num_4: return 0x05;
      case keyboard::num_5: return 0x06;
      case keyboard::num_6: return 0x07;
      case keyboard::num_7: return 0x08;
      case keyboard::num_8: return 0x09;
      case keyboard::num_9: return 0x0a;
      case keyboard::num_0: return 0x0b;

      case keyboard::dash:      return 0x0c;
      case keyboard::equal:     return 0x0d;
      case keyboard::backspace: return 0x0e;

      case keyboard::insert:    return 0xd2;
      case keyboard::delete_:   return 0xd3;
      case keyboard::home:      return 0xc7;
      case keyboard::end:       return 0xcf;
      case keyboard::page_up:   return 0xc9;
      case keyboard::page_down: return 0xd1;

      case keyboard::a: return 0x1e;
      case keyboard::b: return 0x30;
      case keyboard::c: return 0x2e;
      case keyboard::d: return 0x20;
      case keyboard::e: return 0x12;
      case keyboard::f: return 0x21;
      case keyboard::g: return 0x22;
      case keyboard::h: return 0x23;
      case keyboard::i: return 0x17;
      case keyboard::j: return 0x24;
      case keyboard::k: return 0x25;
      case keyboard::l: return 0x26;
      case keyboard::m: return 0x32;
      case keyboard::n: return 0x31;
      case keyboard::o: return 0x18;
      case keyboard::p: return 0x19;
      case keyboard::q: return 0x10;
      case keyboard::r: return 0x13;
      case keyboard::s: return 0x1f;
      case keyboard::t: return 0x14;
      case keyboard::u: return 0x16;
      case keyboard::v: return 0x2f;
      case keyboard::w: return 0x11;
      case keyboard::x: return 0x2d;
      case keyboard::y: return 0x15;
      case keyboard::z: return 0x2c;

      case keyboard::lbracket:   return 0x1a;
      case keyboard::rbracket:   return 0x1b;
      case keyboard::backslash:  return 0x2b;
      case keyboard::semicolon:  return 0x27;
      case keyboard::apostrophe: return 0x28;
      case keyboard::comma:      return 0x33;
      case keyboard::period:     return 0x34;
      case keyboard::slash:      return 0x35;

      case keyboard::pad_1: return 0x4f;
      case keyboard::pad_2: return 0x50;
      case keyboard::pad_3: return 0x51;
      case keyboard::pad_4: return 0x4b;
      case keyboard::pad_5: return 0x4c;
      case keyboard::pad_6: return 0x4d;
      case keyboard::pad_7: return 0x47;
      case keyboard::pad_8: return 0x48;
      case keyboard::pad_9: return 0x49;
      case keyboard::pad_0: return 0x52;
      case keyboard::point: return 0x53;

      case keyboard::add:      return 0x4e;
      case keyboard::subtract: return 0x4a;
      case keyboard::multiply: return 0x37;
      case keyboard::divide:   return 0xb5;
      case keyboard::enter:    return 0x9c;

      case keyboard::num_lock : return 0x45;
      case keyboard::caps_lock: return 0x3a;

      case keyboard::up:    return 0xc8;
      case keyboard::down:  return 0xd0;
      case keyboard::left:  return 0xcb;
      case keyboard::right: return 0xcd;

      case keyboard::tab:      return 0x0f;
      case keyboard::return_:  return 0x1c;
      case keyboard::spacebar: return 0x39;

      case keyboard::lctrl : return 0x1d;
      case keyboard::rctrl : return 0x9d;
      case keyboard::lalt  : return 0x38;
      case keyboard::ralt  : return 0xb8;
      case keyboard::lshift: return 0x2a;
      case keyboard::rshift: return 0x36;
      case keyboard::lsuper: return 0xdb;
      case keyboard::rsuper: return 0xdc;
      case keyboard::menu:   return 0xdd;
    }

    for(uint16_t j = 0; j < 16; j++) {
      uint16_t index = 256 + (j << 8);
      if(key == joypad<>::index(j, joypad<>::up)) return index + 0x80;
      if(key == joypad<>::index(j, joypad<>::down)) return index + 0x81;
      if(key == joypad<>::index(j, joypad<>::left)) return index + 0x82;
      if(key == joypad<>::index(j, joypad<>::right)) return index + 0x83;
      for(uint16_t b = 0; b < 16; b++) {
        if(key == joypad<>::index(j, joypad<>::button_00 + b)) return index + b;
      }
    }

    return 0x00;
  }

  pInputDI(InputDI &self_) : self(self_) {
    di = 0;
    di_key = 0;
    for(int i = 0; i < DIRECTINPUT_JOYMAX; i++) di_joy[i] = 0;

    settings.handle = 0;
    settings.analog_axis_resistance = 75;
  }

  ~pInputDI() { term(); }
};

BOOL CALLBACK DI_EnumJoypadsCallback(const DIDEVICEINSTANCE *instance, void *p) {
  return ((pInputDI*)p)->enum_joypads(instance);
}

bool InputDI::cap(Setting setting) { return p.cap(setting); }
uintptr_t InputDI::get(Setting setting) { return p.get(setting); }
bool InputDI::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
bool InputDI::key_down(uint16_t key) { return p.key_down(key); }
void InputDI::clear() { p.clear(); }
void InputDI::poll() { p.poll(); }
bool InputDI::init() { return p.init(); }
void InputDI::term() { p.term(); }
InputDI::InputDI() : p(*new pInputDI(*this)) {}
InputDI::~InputDI() { delete &p; }

} //namespace ruby
