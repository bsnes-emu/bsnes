/*****
 * SDL input driver
 *
 * Design notes:
 * SDL contains the ability to poll both the keyboard and joypads,
 * however it cannot capture keyboard input from windows that it
 * did not create. Joypad input is captured globally, regardless
 * of what created the active window.
 * Letting SDL create the window to capture keyboard input is not
 * practical. Users of an input library should ideally be able to
 * create a window however they like -- with GTK+, Qt, etc.
 * Therefore, this driver basically uses SDL for joypad support
 * only, and uses the native platform's keyboard input handling
 * routines. Clearly, this is not as portable, but there is little
 * choice in the matter.
 * In the worst case scenario, an unsupported platform, this
 * driver will still work, but will not support keyboard input.
 *****/

#include <SDL/SDL.h>

#if !defined(_WIN32)
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#endif

#include <ruby/ruby.h>

namespace ruby {

#include "sdl.h"
using namespace nall;

class pInputSDL {
public:
  InputSDL &self;
  SDL_Joystick *joy[16];
  SDL_Event event;
  enum {
    joy_button_00, joy_button_01, joy_button_02, joy_button_03,
    joy_button_04, joy_button_05, joy_button_06, joy_button_07,
    joy_button_08, joy_button_09, joy_button_10, joy_button_11,
    joy_button_12, joy_button_13, joy_button_14, joy_button_15,
    joy_up, joy_down, joy_left, joy_right,
    joy_limit,
  };
  bool joystate[16][joy_limit];

  #if !defined(_WIN32)
  char keystate[32];
  Display *display;
  #endif

  struct {
    unsigned analog_axis_resistance;
  } settings;

  bool cap(Input::Setting setting) {
    if(setting == Input::KeyboardSupport) return true;
    if(setting == Input::JoypadSupport) return true;
    if(setting == Input::AnalogAxisResistance) return true;
    return false;
  }

  uintptr_t get(Input::Setting setting) {
    if(setting == Input::AnalogAxisResistance) return settings.analog_axis_resistance;
    return false;
  }

  bool set(Input::Setting setting, uintptr_t param) {
    if(setting == Input::AnalogAxisResistance) {
      settings.analog_axis_resistance = param;
      return true;
    }

    return false;
  }

  bool key_down(uint16_t key) {
    #if !defined(_WIN32)
    #define map(i) (keystate[i >> 3] & (1 << (i & 7)))

    switch(key) {
      case keyboard::escape: return map(0x09);

      case keyboard::f1:  return map(0x43);
      case keyboard::f2:  return map(0x44);
      case keyboard::f3:  return map(0x45);
      case keyboard::f4:  return map(0x46);
      case keyboard::f5:  return map(0x47);
      case keyboard::f6:  return map(0x48);
      case keyboard::f7:  return map(0x49);
      case keyboard::f8:  return map(0x4a);
      case keyboard::f9:  return map(0x4b);
      case keyboard::f10: return map(0x4c);
      case keyboard::f11: return map(0x5f);
      case keyboard::f12: return map(0x60);

      case keyboard::print_screen: return map(0x6f);
      case keyboard::scroll_lock:  return map(0x4e);
      case keyboard::pause:        return map(0x6e);

      case keyboard::tilde: return map(0x31);

      case keyboard::num_1: return map(0x0a);
      case keyboard::num_2: return map(0x0b);
      case keyboard::num_3: return map(0x0c);
      case keyboard::num_4: return map(0x0d);
      case keyboard::num_5: return map(0x0e);
      case keyboard::num_6: return map(0x0f);
      case keyboard::num_7: return map(0x10);
      case keyboard::num_8: return map(0x11);
      case keyboard::num_9: return map(0x12);
      case keyboard::num_0: return map(0x13);

      case keyboard::dash:      return map(0x14);
      case keyboard::equal:     return map(0x15);
      case keyboard::backspace: return map(0x16);

      case keyboard::insert:    return map(0x6a);
      case keyboard::delete_:   return map(0x6b);
      case keyboard::home:      return map(0x61);
      case keyboard::end:       return map(0x67);
      case keyboard::page_up:   return map(0x63);
      case keyboard::page_down: return map(0x69);

      case keyboard::a: return map(0x26);
      case keyboard::b: return map(0x38);
      case keyboard::c: return map(0x36);
      case keyboard::d: return map(0x28);
      case keyboard::e: return map(0x1a);
      case keyboard::f: return map(0x29);
      case keyboard::g: return map(0x2a);
      case keyboard::h: return map(0x2b);
      case keyboard::i: return map(0x1f);
      case keyboard::j: return map(0x2c);
      case keyboard::k: return map(0x2d);
      case keyboard::l: return map(0x2e);
      case keyboard::m: return map(0x3a);
      case keyboard::n: return map(0x39);
      case keyboard::o: return map(0x20);
      case keyboard::p: return map(0x21);
      case keyboard::q: return map(0x18);
      case keyboard::r: return map(0x1b);
      case keyboard::s: return map(0x27);
      case keyboard::t: return map(0x1c);
      case keyboard::u: return map(0x1e);
      case keyboard::v: return map(0x37);
      case keyboard::w: return map(0x19);
      case keyboard::x: return map(0x35);
      case keyboard::y: return map(0x1d);
      case keyboard::z: return map(0x34);

      case keyboard::lbracket:   return map(0x22);
      case keyboard::rbracket:   return map(0x23);
      case keyboard::backslash:  return map(0x33);
      case keyboard::semicolon:  return map(0x2f);
      case keyboard::apostrophe: return map(0x30);
      case keyboard::comma:      return map(0x3b);
      case keyboard::period:     return map(0x3c);
      case keyboard::slash:      return map(0x3d);

      case keyboard::pad_1: return map(0x57);
      case keyboard::pad_2: return map(0x58);
      case keyboard::pad_3: return map(0x59);
      case keyboard::pad_4: return map(0x53);
      case keyboard::pad_5: return map(0x54);
      case keyboard::pad_6: return map(0x55);
      case keyboard::pad_7: return map(0x4f);
      case keyboard::pad_8: return map(0x50);
      case keyboard::pad_9: return map(0x51);

      case keyboard::add:      return map(0x56);
      case keyboard::subtract: return map(0x52);
      case keyboard::multiply: return map(0x3f);
      case keyboard::divide:   return map(0x70);
      case keyboard::enter:    return map(0x6c);

      case keyboard::num_lock:  return map(0x4d);
      case keyboard::caps_lock: return map(0x42);

      case keyboard::up:    return map(0x62);
      case keyboard::down:  return map(0x68);
      case keyboard::left:  return map(0x64);
      case keyboard::right: return map(0x66);

      case keyboard::tab:      return map(0x17);
      case keyboard::return_:  return map(0x24);
      case keyboard::spacebar: return map(0x41);

      case keyboard::lctrl:  return map(0x25);
      case keyboard::rctrl:  return map(0x6d);
      case keyboard::lalt:   return map(0x40);
      case keyboard::ralt:   return map(0x71);
      case keyboard::lshift: return map(0x32);
      case keyboard::rshift: return map(0x3e);
      case keyboard::lsuper: return map(0x73);
      case keyboard::rsuper: return map(0x74);
      case keyboard::menu:   return map(0x75);
    }

    #undef map
    #endif

    for(int i = 0; i < 16; i++) {
      if(key == joypad<>::index(i, joypad<>::up)) return joystate[i][joy_up];
      if(key == joypad<>::index(i, joypad<>::down)) return joystate[i][joy_down];
      if(key == joypad<>::index(i, joypad<>::left)) return joystate[i][joy_left];
      if(key == joypad<>::index(i, joypad<>::right)) return joystate[i][joy_right];
      for(int b = 0; b < 16; b++) {
        if(key == joypad<>::index(i, joypad<>::button_00 + b)) return joystate[i][joy_button_00 + b];
      }
    }

    return false;
  }

  void clear() {
    #if !defined(_WIN32)
    memset(keystate, 0, sizeof keystate);
    #endif

    for(int i = 0; i < 16; i++) {
      for(int b = 0; b < joy_limit; b++) {
        joystate[i][b] = false;
      }
    }
  }

  void poll() {
    SDL_JoystickUpdate();

    #if !defined(_WIN32)
    XQueryKeymap(display, keystate);
    #endif

    for(int i = 0; i < 16; i++) {
      if(!joy[i]) continue;

      joystate[i][joy_up]    = false;
      joystate[i][joy_down]  = false;
      joystate[i][joy_left]  = false;
      joystate[i][joy_right] = false;

      int resistance = settings.analog_axis_resistance;
      resistance = max(1, min(99, resistance));
      resistance = int(double(resistance) * 32768.0 / 100.0);

      //only poll X,Y axes for D-pad, left analog and right analog.
      //note 1: right analog is swapped on some controllers, this cannot be helped.
      //note 2: some controllers report more axes than physically exist.
      //these tend to return fixed values (eg 32767), which makes state changes
      //impossible to detect. hence why polling *all* axes is unsafe.
      int axes = SDL_JoystickNumAxes(joy[i]);
      for(int a = 0; a < min(axes, 6); a++) {
        int value = SDL_JoystickGetAxis(joy[i], a);
        if((a & 1) == 0) { //X axis
          joystate[i][joy_left]  |= value < -resistance;
          joystate[i][joy_right] |= value > +resistance;
        } else { //Y axis
          joystate[i][joy_up]    |= value < -resistance;
          joystate[i][joy_down]  |= value > +resistance;
        }
      }

      for(int b = 0; b < 16; b++) {
        joystate[i][b] = SDL_JoystickGetButton(joy[i], b);
      }
    }
  }

  bool init() {
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_IGNORE);

    for(int i = 0; i < SDL_NumJoysticks(); i++) {
      joy[i] = SDL_JoystickOpen(i);
    }

    #if !defined(_WIN32)
    display = XOpenDisplay(0);
    #endif

    return true;
  }

  void term() {
    for(int i = 0; i < 16; i++) {
      if(joy[i]) SDL_JoystickClose(joy[i]);
    }
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
  }

  pInputSDL(InputSDL &self_) : self(self_) {
    for(int i = 0; i < 16; i++) joy[i] = 0;
    clear();

    settings.analog_axis_resistance = 75;
  }
};


bool InputSDL::cap(Setting setting) { return p.cap(setting); }
uintptr_t InputSDL::get(Setting setting) { return p.get(setting); }
bool InputSDL::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
bool InputSDL::key_down(uint16_t key) { return p.key_down(key); }
void InputSDL::clear() { p.clear(); }
void InputSDL::poll() { p.poll(); }
bool InputSDL::init() { return p.init(); }
void InputSDL::term() { p.term(); }
InputSDL::InputSDL() : p(*new pInputSDL(*this)) {}
InputSDL::~InputSDL() { delete &p; }

} //namespace ruby
