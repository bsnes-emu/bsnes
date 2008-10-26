#include <windows.h>
#include <dinput.h>

namespace ruby {

#include "directinput.hpp"

static BOOL CALLBACK DI_EnumJoypadsCallback(const DIDEVICEINSTANCE*, void*);

using namespace nall;

class pInputDI {
public:
  InputDI &self;
  LPDIRECTINPUT8 context;
  LPDIRECTINPUTDEVICE8 keyboard;
  LPDIRECTINPUTDEVICE8 mouse;
  LPDIRECTINPUTDEVICE8 gamepad[joypad<>::count];
  unsigned gamepad_count;  //number of physical gamepads present
  bool mouseacquired;

  struct {
    HWND handle;
    unsigned analog_axis_resistance;
  } settings;

  bool cap(Input::Setting setting) {
    if(setting == Input::Handle) return true;
    if(setting == Input::KeyboardSupport) return true;
    if(setting == Input::MouseSupport) return true;
    if(setting == Input::JoypadSupport) return true;
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

  bool poll(int16_t *table) {
    memset(table, 0, input_limit * sizeof(int16_t));

    if(keyboard) {
      uint8_t state[256];
      if(FAILED(keyboard->GetDeviceState(sizeof state, state))) {
        keyboard->Acquire();
        if(FAILED(keyboard->GetDeviceState(sizeof state, state))) {
          memset(state, 0, sizeof state);
        }
      }

      table[keyboard::escape] = (bool)(state[0x01] & 0x80);

      table[keyboard::f1 ] = (bool)(state[0x3b] & 0x80);
      table[keyboard::f2 ] = (bool)(state[0x3c] & 0x80);
      table[keyboard::f3 ] = (bool)(state[0x3d] & 0x80);
      table[keyboard::f4 ] = (bool)(state[0x3e] & 0x80);
      table[keyboard::f5 ] = (bool)(state[0x3f] & 0x80);
      table[keyboard::f6 ] = (bool)(state[0x40] & 0x80);
      table[keyboard::f7 ] = (bool)(state[0x41] & 0x80);
      table[keyboard::f8 ] = (bool)(state[0x42] & 0x80);
      table[keyboard::f9 ] = (bool)(state[0x43] & 0x80);
      table[keyboard::f10] = (bool)(state[0x44] & 0x80);
      table[keyboard::f11] = (bool)(state[0x57] & 0x80);
      table[keyboard::f12] = (bool)(state[0x58] & 0x80);

      table[keyboard::print_screen] = (bool)(state[0xb7] & 0x80);
      table[keyboard::scroll_lock ] = (bool)(state[0x46] & 0x80);
      table[keyboard::pause       ] = (bool)(state[0xc5] & 0x80);

      table[keyboard::tilde] = (bool)(state[0x29] & 0x80);

      table[keyboard::num_1] = (bool)(state[0x02] & 0x80);
      table[keyboard::num_2] = (bool)(state[0x03] & 0x80);
      table[keyboard::num_3] = (bool)(state[0x04] & 0x80);
      table[keyboard::num_4] = (bool)(state[0x05] & 0x80);
      table[keyboard::num_5] = (bool)(state[0x06] & 0x80);
      table[keyboard::num_6] = (bool)(state[0x07] & 0x80);
      table[keyboard::num_7] = (bool)(state[0x08] & 0x80);
      table[keyboard::num_8] = (bool)(state[0x09] & 0x80);
      table[keyboard::num_9] = (bool)(state[0x0a] & 0x80);
      table[keyboard::num_0] = (bool)(state[0x0b] & 0x80);

      table[keyboard::dash     ] = (bool)(state[0x0c] & 0x80);
      table[keyboard::equal    ] = (bool)(state[0x0d] & 0x80);
      table[keyboard::backspace] = (bool)(state[0x0e] & 0x80);

      table[keyboard::insert   ] = (bool)(state[0xd2] & 0x80);
      table[keyboard::delete_  ] = (bool)(state[0xd3] & 0x80);
      table[keyboard::home     ] = (bool)(state[0xc7] & 0x80);
      table[keyboard::end      ] = (bool)(state[0xcf] & 0x80);
      table[keyboard::page_up  ] = (bool)(state[0xc9] & 0x80);
      table[keyboard::page_down] = (bool)(state[0xd1] & 0x80);

      table[keyboard::a] = (bool)(state[0x1e] & 0x80);
      table[keyboard::b] = (bool)(state[0x30] & 0x80);
      table[keyboard::c] = (bool)(state[0x2e] & 0x80);
      table[keyboard::d] = (bool)(state[0x20] & 0x80);
      table[keyboard::e] = (bool)(state[0x12] & 0x80);
      table[keyboard::f] = (bool)(state[0x21] & 0x80);
      table[keyboard::g] = (bool)(state[0x22] & 0x80);
      table[keyboard::h] = (bool)(state[0x23] & 0x80);
      table[keyboard::i] = (bool)(state[0x17] & 0x80);
      table[keyboard::j] = (bool)(state[0x24] & 0x80);
      table[keyboard::k] = (bool)(state[0x25] & 0x80);
      table[keyboard::l] = (bool)(state[0x26] & 0x80);
      table[keyboard::m] = (bool)(state[0x32] & 0x80);
      table[keyboard::n] = (bool)(state[0x31] & 0x80);
      table[keyboard::o] = (bool)(state[0x18] & 0x80);
      table[keyboard::p] = (bool)(state[0x19] & 0x80);
      table[keyboard::q] = (bool)(state[0x10] & 0x80);
      table[keyboard::r] = (bool)(state[0x13] & 0x80);
      table[keyboard::s] = (bool)(state[0x1f] & 0x80);
      table[keyboard::t] = (bool)(state[0x14] & 0x80);
      table[keyboard::u] = (bool)(state[0x16] & 0x80);
      table[keyboard::v] = (bool)(state[0x2f] & 0x80);
      table[keyboard::w] = (bool)(state[0x11] & 0x80);
      table[keyboard::x] = (bool)(state[0x2d] & 0x80);
      table[keyboard::y] = (bool)(state[0x15] & 0x80);
      table[keyboard::z] = (bool)(state[0x2c] & 0x80);

      table[keyboard::lbracket  ] = (bool)(state[0x1a] & 0x80);
      table[keyboard::rbracket  ] = (bool)(state[0x1b] & 0x80);
      table[keyboard::backslash ] = (bool)(state[0x2b] & 0x80);
      table[keyboard::semicolon ] = (bool)(state[0x27] & 0x80);
      table[keyboard::apostrophe] = (bool)(state[0x28] & 0x80);
      table[keyboard::comma     ] = (bool)(state[0x33] & 0x80);
      table[keyboard::period    ] = (bool)(state[0x34] & 0x80);
      table[keyboard::slash     ] = (bool)(state[0x35] & 0x80);

      table[keyboard::pad_0] = (bool)(state[0x4f] & 0x80);
      table[keyboard::pad_1] = (bool)(state[0x50] & 0x80);
      table[keyboard::pad_2] = (bool)(state[0x51] & 0x80);
      table[keyboard::pad_3] = (bool)(state[0x4b] & 0x80);
      table[keyboard::pad_4] = (bool)(state[0x4c] & 0x80);
      table[keyboard::pad_5] = (bool)(state[0x4d] & 0x80);
      table[keyboard::pad_6] = (bool)(state[0x47] & 0x80);
      table[keyboard::pad_7] = (bool)(state[0x48] & 0x80);
      table[keyboard::pad_8] = (bool)(state[0x49] & 0x80);
      table[keyboard::pad_9] = (bool)(state[0x52] & 0x80);
      table[keyboard::point] = (bool)(state[0x53] & 0x80);

      table[keyboard::add]      = (bool)(state[0x4e] & 0x80);
      table[keyboard::subtract] = (bool)(state[0x4a] & 0x80);
      table[keyboard::multiply] = (bool)(state[0x37] & 0x80);
      table[keyboard::divide]   = (bool)(state[0xb5] & 0x80);
      table[keyboard::enter]    = (bool)(state[0x9c] & 0x80);

      table[keyboard::num_lock ] = (bool)(state[0x45] & 0x80);
      table[keyboard::caps_lock] = (bool)(state[0x3a] & 0x80);

      table[keyboard::up   ] = (bool)(state[0xc8] & 0x80);
      table[keyboard::down ] = (bool)(state[0xd0] & 0x80);
      table[keyboard::left ] = (bool)(state[0xcb] & 0x80);
      table[keyboard::right] = (bool)(state[0xcd] & 0x80);

      table[keyboard::tab     ] = (bool)(state[0x0f] & 0x80);
      table[keyboard::return_ ] = (bool)(state[0x1c] & 0x80);
      table[keyboard::spacebar] = (bool)(state[0x39] & 0x80);

      table[keyboard::lctrl ] = (bool)(state[0x1d] & 0x80);
      table[keyboard::rctrl ] = (bool)(state[0x9d] & 0x80);
      table[keyboard::lalt  ] = (bool)(state[0x38] & 0x80);
      table[keyboard::ralt  ] = (bool)(state[0xb8] & 0x80);
      table[keyboard::lshift] = (bool)(state[0x2a] & 0x80);
      table[keyboard::rshift] = (bool)(state[0x36] & 0x80);
      table[keyboard::lsuper] = (bool)(state[0xdb] & 0x80);
      table[keyboard::rsuper] = (bool)(state[0xdc] & 0x80);
      table[keyboard::menu  ] = (bool)(state[0xdd] & 0x80);
    }

    if(mouse) {
      DIMOUSESTATE2 state;
      if(FAILED(mouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void*)&state))) {
        mouse->Acquire();
        if(FAILED(mouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void*)&state))) {
          memset(&state, 0, sizeof(DIMOUSESTATE2));
        }
      }

      table[mouse::x] = state.lX;
      table[mouse::y] = state.lY;
      table[mouse::z] = state.lZ / WHEEL_DELTA;
      for(unsigned n = 0; n < mouse::buttons; n++) {
        table[mouse::button + n] = (bool)state.rgbButtons[n];
      }

      //on Windows, 0 = left, 1 = right, 2 = middle
      //swap middle and right buttons for consistency with Linux
      int16_t temp = table[mouse::button + 1];
      table[mouse::button + 1] = table[mouse::button + 2];
      table[mouse::button + 2] = temp;
    }

    for(unsigned i = 0; i < gamepad_count; i++) {
      if(!gamepad[i]) continue;

      DIJOYSTATE2 state;
      if(FAILED(gamepad[i]->Poll())) {
        gamepad[i]->Acquire();
        if(FAILED(gamepad[i]->Poll())) {
          continue;
        }
      }

      gamepad[i]->GetDeviceState(sizeof(DIJOYSTATE2), &state);

      unsigned index = joypad<>::index(i, joypad<>::none);

      int resistance = settings.analog_axis_resistance;
      resistance = max(1, min(99, resistance));
      resistance = (int)((double)resistance * 32768.0 / 100.0);
      int resistance_lo = 0x7fff - resistance;
      int resistance_hi = 0x8000 + resistance;

      table[index + joypad<>::up   ] = (state.lY <= resistance_lo);
      table[index + joypad<>::down ] = (state.lY >= resistance_hi);
      table[index + joypad<>::left ] = (state.lX <= resistance_lo);
      table[index + joypad<>::right] = (state.lX >= resistance_hi);

      unsigned pov = state.rgdwPOV[0];
      table[index + joypad<>::up   ] |= (pov ==     0 || pov == 31500 || pov ==  4500);
      table[index + joypad<>::down ] |= (pov == 18000 || pov == 13500 || pov == 22500);
      table[index + joypad<>::left ] |= (pov == 27000 || pov == 22500 || pov == 31500);
      table[index + joypad<>::right] |= (pov ==  9000 || pov ==  4500 || pov == 13500);

      table[index + joypad<>::axis + 0] = (int16_t)(state.lX - 32768);
      table[index + joypad<>::axis + 1] = (int16_t)(state.lY - 32768);

      for(unsigned n = 0; n < joypad<>::buttons; n++) {
        table[index + joypad<>::button + n] = (bool)state.rgbButtons[n];
      }
    }

    return true;
  }

  bool enum_joypads(const DIDEVICEINSTANCE *instance) {
    if(FAILED(context->CreateDevice(instance->guidInstance, &gamepad[gamepad_count], 0))) {
      return DIENUM_CONTINUE; //continue and try next gamepad
    }

    gamepad[gamepad_count]->SetDataFormat(&c_dfDIJoystick2);
    gamepad[gamepad_count]->SetCooperativeLevel(settings.handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

    if(++gamepad_count >= joypad<>::count) return DIENUM_STOP;
    return DIENUM_CONTINUE;
  }

  bool init() {
    context = 0;
    keyboard = 0;
    mouse = 0;
    for(unsigned i = 0; i < joypad<>::count; i++) gamepad[i] = 0;
    gamepad_count = 0;
    mouseacquired = false;

    DirectInput8Create(GetModuleHandle(0), 0x0800, IID_IDirectInput8, (void**)&context, 0);

    context->CreateDevice(GUID_SysKeyboard, &keyboard, 0);
    keyboard->SetDataFormat(&c_dfDIKeyboard);
    keyboard->SetCooperativeLevel(settings.handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
    keyboard->Acquire();

    context->CreateDevice(GUID_SysMouse, &mouse, 0);
    mouse->SetDataFormat(&c_dfDIMouse2);
    HRESULT hr = mouse->SetCooperativeLevel(settings.handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
    mouse->Acquire();

    context->EnumDevices(DI8DEVCLASS_GAMECTRL, DI_EnumJoypadsCallback, (void*)this, DIEDFL_ATTACHEDONLY);

    return true;
  }

  void term() {
    if(keyboard) {
      keyboard->Unacquire();
      keyboard->Release();
      keyboard = 0;
    }

    if(mouse) {
      mouse->Unacquire();
      mouse->Release();
      mouse = 0;
    }

    for(unsigned i = 0; i < joypad<>::count; i++) {
      if(gamepad[i]) {
        gamepad[i]->Unacquire();
        gamepad[i]->Release();
        gamepad[i] = 0;
      }
    }
    gamepad_count = 0;

    if(context) {
      context->Release();
      context = 0;
    }
  }

  bool acquire() {
    if(!mouse) return false;
    if(acquired() == false) {
      mouse->Unacquire();
      mouse->SetCooperativeLevel(settings.handle, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
      mouse->Acquire();
      mouseacquired = true;
    }
    return true;
  }

  bool unacquire() {
    if(!mouse) return false;
    if(acquired() == true) {
      mouse->Unacquire();
      mouse->SetCooperativeLevel(settings.handle, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
      mouse->Acquire();
      mouseacquired = false;
    }
    return true;
  }

  bool acquired() {
    return mouseacquired;
  }

  pInputDI(InputDI &self_) : self(self_) {
    context = 0;
    keyboard = 0;
    mouse = 0;
    for(unsigned i = 0; i < joypad<>::count; i++) gamepad[i] = 0;
    gamepad_count = 0;
    mouseacquired = false;

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
bool InputDI::acquire() { return p.acquire(); }
bool InputDI::unacquire() { return p.unacquire(); }
bool InputDI::acquired() { return p.acquired(); }
bool InputDI::poll(int16_t *table) { return p.poll(table); }
bool InputDI::init() { return p.init(); }
void InputDI::term() { p.term(); }
InputDI::InputDI() : p(*new pInputDI(*this)) {}
InputDI::~InputDI() { delete &p; }

} //namespace ruby
