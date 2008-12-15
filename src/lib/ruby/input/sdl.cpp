//================
//SDL input driver
//================
//Keyboard and mouse are controlled directly via Xlib,
//as SDL cannot capture input from windows it does not
//create itself.
//SDL is used only to handle joysticks.

#include <SDL/SDL.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

namespace ruby {

#include "sdl.hpp"
using namespace nall;

class pInputSDL {
public:
  InputSDL &self;
  Display *display;
  Window rootwindow;
  unsigned screenwidth, screenheight;
  unsigned relativex, relativey;
  Cursor InvisibleCursor;
  SDL_Joystick *gamepad[joypad<>::count];
  bool mouseacquired;

  struct {
    //mouse device settings
    int accel_numerator;
    int accel_denominator;
    int threshold;
  } device;

  struct {
    uintptr_t handle;
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
    if(setting == Input::Handle) return settings.handle;
    if(setting == Input::AnalogAxisResistance) return settings.analog_axis_resistance;
    return false;
  }

  bool set(Input::Setting setting, uintptr_t param) {
    if(setting == Input::Handle) {
      settings.handle = param;
      return true;
    }

    if(setting == Input::AnalogAxisResistance) {
      settings.analog_axis_resistance = param;
      return true;
    }

    return false;
  }

  bool acquire() {
    if(acquired()) return true;

    if(XGrabPointer(display, settings.handle, True, 0, GrabModeAsync, GrabModeAsync,
    rootwindow, InvisibleCursor, CurrentTime) == GrabSuccess) {
      //backup existing cursor acceleration settings
      XGetPointerControl(display, &device.accel_numerator, &device.accel_denominator, &device.threshold);

      //disable cursor acceleration
      XChangePointerControl(display, True, False, 1, 1, 0);

      //center cursor (so that first relative poll returns 0, 0 if mouse has not moved)
      XWarpPointer(display, None, rootwindow, 0, 0, 0, 0, screenwidth / 2, screenheight / 2);

      return mouseacquired = true;
    } else {
      return mouseacquired = false;
    }
  }

  bool unacquire() {
    if(acquired()) {
      //restore cursor acceleration and release cursor
      XChangePointerControl(display, True, True, device.accel_numerator, device.accel_denominator, device.threshold);
      XUngrabPointer(display, CurrentTime);
      mouseacquired = false;
    }
    return true;
  }

  bool acquired() {
    return mouseacquired;
  }

  bool poll(int16_t *table) {
    memset(table, 0, input_limit * sizeof(int16_t));

    //========
    //Keyboard
    //========

    #define key(n) (bool)(state[n >> 3] & (1 << (n & 7)))
    {
      char state[32];
      XQueryKeymap(display, state);

      table[keyboard::escape] = key(0x09);

      table[keyboard::f1 ] = key(0x43);
      table[keyboard::f2 ] = key(0x44);
      table[keyboard::f3 ] = key(0x45);
      table[keyboard::f4 ] = key(0x46);
      table[keyboard::f5 ] = key(0x47);
      table[keyboard::f6 ] = key(0x48);
      table[keyboard::f7 ] = key(0x49);
      table[keyboard::f8 ] = key(0x4a);
      table[keyboard::f9 ] = key(0x4b);
      table[keyboard::f10] = key(0x4c);
      table[keyboard::f11] = key(0x5f);
      table[keyboard::f12] = key(0x60);

      table[keyboard::print_screen] = key(0x6f);
      table[keyboard::scroll_lock ] = key(0x4e);
      table[keyboard::pause       ] = key(0x6e);

      table[keyboard::tilde] = key(0x31);

      table[keyboard::num_0] = key(0x0a);
      table[keyboard::num_1] = key(0x0b);
      table[keyboard::num_2] = key(0x0c);
      table[keyboard::num_3] = key(0x0d);
      table[keyboard::num_4] = key(0x0e);
      table[keyboard::num_5] = key(0x0f);
      table[keyboard::num_6] = key(0x10);
      table[keyboard::num_7] = key(0x11);
      table[keyboard::num_8] = key(0x12);
      table[keyboard::num_9] = key(0x13);

      table[keyboard::dash     ] = key(0x14);
      table[keyboard::equal    ] = key(0x15);
      table[keyboard::backspace] = key(0x16);

      table[keyboard::insert   ] = key(0x6a);
      table[keyboard::delete_  ] = key(0x6b);
      table[keyboard::home     ] = key(0x61);
      table[keyboard::end      ] = key(0x67);
      table[keyboard::page_up  ] = key(0x63);
      table[keyboard::page_down] = key(0x69);

      table[keyboard::a] = key(0x26);
      table[keyboard::b] = key(0x38);
      table[keyboard::c] = key(0x36);
      table[keyboard::d] = key(0x28);
      table[keyboard::e] = key(0x1a);
      table[keyboard::f] = key(0x29);
      table[keyboard::g] = key(0x2a);
      table[keyboard::h] = key(0x2b);
      table[keyboard::i] = key(0x1f);
      table[keyboard::j] = key(0x2c);
      table[keyboard::k] = key(0x2d);
      table[keyboard::l] = key(0x2e);
      table[keyboard::m] = key(0x3a);
      table[keyboard::n] = key(0x39);
      table[keyboard::o] = key(0x20);
      table[keyboard::p] = key(0x21);
      table[keyboard::q] = key(0x18);
      table[keyboard::r] = key(0x1b);
      table[keyboard::s] = key(0x27);
      table[keyboard::t] = key(0x1c);
      table[keyboard::u] = key(0x1e);
      table[keyboard::v] = key(0x37);
      table[keyboard::w] = key(0x19);
      table[keyboard::x] = key(0x35);
      table[keyboard::y] = key(0x1d);
      table[keyboard::z] = key(0x34);

      table[keyboard::lbracket  ] = key(0x22);
      table[keyboard::rbracket  ] = key(0x23);
      table[keyboard::backslash ] = key(0x33);
      table[keyboard::semicolon ] = key(0x2f);
      table[keyboard::apostrophe] = key(0x30);
      table[keyboard::comma     ] = key(0x3b);
      table[keyboard::period    ] = key(0x3c);
      table[keyboard::slash     ] = key(0x3d);

      table[keyboard::pad_0] = key(0x5a);
      table[keyboard::pad_1] = key(0x57);
      table[keyboard::pad_2] = key(0x58);
      table[keyboard::pad_3] = key(0x59);
      table[keyboard::pad_4] = key(0x53);
      table[keyboard::pad_5] = key(0x54);
      table[keyboard::pad_6] = key(0x55);
      table[keyboard::pad_7] = key(0x4f);
      table[keyboard::pad_8] = key(0x50);
      table[keyboard::pad_9] = key(0x51);

      table[keyboard::add     ] = key(0x56);
      table[keyboard::subtract] = key(0x52);
      table[keyboard::multiply] = key(0x3f);
      table[keyboard::divide  ] = key(0x70);
      table[keyboard::enter   ] = key(0x6c);

      table[keyboard::num_lock ] = key(0x4d);
      table[keyboard::caps_lock] = key(0x42);

      table[keyboard::up   ] = key(0x62);
      table[keyboard::down ] = key(0x68);
      table[keyboard::left ] = key(0x64);
      table[keyboard::right] = key(0x66);

      table[keyboard::tab     ] = key(0x17);
      table[keyboard::return_ ] = key(0x24);
      table[keyboard::spacebar] = key(0x41);

      table[keyboard::lctrl ] = key(0x25);
      table[keyboard::rctrl ] = key(0x6d);
      table[keyboard::lalt  ] = key(0x40);
      table[keyboard::ralt  ] = key(0x71);
      table[keyboard::lshift] = key(0x32);
      table[keyboard::rshift] = key(0x3e);
      table[keyboard::lsuper] = key(0x73);
      table[keyboard::rsuper] = key(0x74);
      table[keyboard::menu  ] = key(0x75);
    }
    #undef key

    //=====
    //Mouse
    //=====

    Window root_return, child_return;
    int root_x_return = 0, root_y_return = 0;
    int win_x_return = 0, win_y_return = 0;
    unsigned int mask_return = 0;
    XQueryPointer(display, settings.handle,
      &root_return, &child_return, &root_x_return, &root_y_return,
      &win_x_return, &win_y_return, &mask_return);

    if(acquired()) {
      XWindowAttributes attributes;
      XGetWindowAttributes(display, settings.handle, &attributes);

      //absolute -> relative conversion
      table[mouse::x] = (int16_t)(root_x_return - screenwidth  / 2);
      table[mouse::y] = (int16_t)(root_y_return - screenheight / 2);

      if(table[mouse::x] != 0 || table[mouse::y] != 0) {
        //if mouse movement occurred, re-center mouse for next poll
        XWarpPointer(display, None, rootwindow, 0, 0, 0, 0, screenwidth / 2, screenheight / 2);
      }
    } else {
      table[mouse::x] = (int16_t)(root_x_return - relativex);
      table[mouse::y] = (int16_t)(root_y_return - relativey);

      relativex = root_x_return;
      relativey = root_y_return;
    }

    //manual device polling is limited to only five buttons ...
    table[mouse::button + 0] = (bool)(mask_return & Button1Mask);
    table[mouse::button + 1] = (bool)(mask_return & Button2Mask);
    table[mouse::button + 2] = (bool)(mask_return & Button3Mask);
    table[mouse::button + 3] = (bool)(mask_return & Button4Mask);
    table[mouse::button + 4] = (bool)(mask_return & Button5Mask);

    //=========
    //Joypad(s)
    //=========

    SDL_JoystickUpdate();
    for(unsigned i = 0; i < joypad<>::count; i++) {
      if(!gamepad[i]) continue;

      unsigned index = joypad<>::index(i, joypad<>::none);
      table[index + joypad<>::up   ] = false;
      table[index + joypad<>::down ] = false;
      table[index + joypad<>::left ] = false;
      table[index + joypad<>::right] = false;

      int resistance = settings.analog_axis_resistance;
      resistance = max(1, min(99, resistance));
      resistance = (int)((double)resistance * 32768.0 / 100.0);

      unsigned axes = min((unsigned)joypad<>::axes, SDL_JoystickNumAxes(gamepad[i]));
      for(unsigned axis = 0; axis < axes; axis++) {
        int16_t value = (int16_t)SDL_JoystickGetAxis(gamepad[i], axis);
        table[index + joypad<>::axis + axis] = value;
        if(axis == 0) {  //X-axis
          table[index + joypad<>::left ] |= value < -resistance;
          table[index + joypad<>::right] |= value > +resistance;
        } else if(axis == 1) {  //Y-axis
          table[index + joypad<>::up   ] |= value < -resistance;
          table[index + joypad<>::down ] |= value > +resistance;
        }
      }

      for(unsigned button = 0; button < joypad<>::buttons; button++) {
        table[index + joypad<>::button + button] = SDL_JoystickGetButton(gamepad[i], button);
      }
    }

    return true;
  }

  bool init() {
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_IGNORE);

    display = XOpenDisplay(0);
    rootwindow = DefaultRootWindow(display);
    XWindowAttributes attributes;
    XGetWindowAttributes(display, rootwindow, &attributes);
    screenwidth  = attributes.width;
    screenheight = attributes.height;

    //Xlib: "because XShowCursor(false) would be too easy."
    //create a fully transparent cursor named InvisibleCursor,
    //for use while acquire() / XGrabPointer() is active.
    Pixmap pixmap;
    XColor black, unused;
    static char invisible_data[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    Colormap colormap = DefaultColormap(display, DefaultScreen(display));
    XAllocNamedColor(display, colormap, "black", &black, &unused);
    pixmap = XCreateBitmapFromData(display, settings.handle, invisible_data, 8, 8);
    InvisibleCursor = XCreatePixmapCursor(display, pixmap, pixmap, &black, &black, 0, 0);
    XFreePixmap(display, pixmap);
    XFreeColors(display, colormap, &black.pixel, 1, 0);

    mouseacquired = false;
    relativex = 0;
    relativey = 0;

    for(unsigned i = 0; i < joypad<>::count && i < SDL_NumJoysticks(); i++) {
      gamepad[i] = SDL_JoystickOpen(i);
    }

    return true;
  }

  void term() {
    unacquire();
    XFreeCursor(display, InvisibleCursor);

    for(unsigned i = 0; i < joypad<>::count; i++) {
      if(gamepad[i]) SDL_JoystickClose(gamepad[i]);
      gamepad[i] = 0;
    }

    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
  }

  pInputSDL(InputSDL &self_) : self(self_) {
    for(unsigned i = 0; i < joypad<>::count; i++) gamepad[i] = 0;
    settings.analog_axis_resistance = 75;
  }
};


bool InputSDL::cap(Setting setting) { return p.cap(setting); }
uintptr_t InputSDL::get(Setting setting) { return p.get(setting); }
bool InputSDL::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
bool InputSDL::acquire() { return p.acquire(); }
bool InputSDL::unacquire() { return p.unacquire(); }
bool InputSDL::acquired() { return p.acquired(); }
bool InputSDL::poll(int16_t *table) { return p.poll(table); }
bool InputSDL::init() { return p.init(); }
void InputSDL::term() { p.term(); }
InputSDL::InputSDL() : p(*new pInputSDL(*this)) {}
InputSDL::~InputSDL() { delete &p; }

} //namespace ruby
