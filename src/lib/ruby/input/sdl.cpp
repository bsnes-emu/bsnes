//================
//SDL input driver
//================
//Keyboard and mouse are controlled directly via Xlib,
//as SDL cannot capture input from windows it does not create itself.
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
  #include "xlibkeys.hpp"
  InputSDL &self;
  Display *display;
  Window rootwindow;
  unsigned screenwidth, screenheight;
  unsigned relativex, relativey;
  bool mouseacquired;
  Cursor InvisibleCursor;
  SDL_Joystick *gamepad[joypad<>::count];

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

    char state[32];
    XQueryKeymap(display, state);

    for(unsigned i = 0; i < keyboard::limit; i++) {
      uint8_t code = keycode[i];
      if(code == 0) continue;  //unmapped
      table[i] = (bool)(state[code >> 3] & (1 << (code & 7)));
    }

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
    init_keycodes();
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
