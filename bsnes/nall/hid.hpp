#ifndef NALL_HID_HPP
#define NALL_HID_HPP

#include <nall/xorg/xorg.hpp>
#include <nall/input.hpp>

namespace nall {
namespace HID {

struct Keyboard {
  XlibDisplay *display;

  inline void poll() {
    XQueryKeymap(display, state);
  }

  inline bool operator[](unsigned id) {
    return state[scancode[id] >> 3] & (1 << (scancode[id] & 7));
  }

  inline Keyboard() {
    display = XOpenDisplay(0);
    memset(&scancode, 0, sizeof scancode);

    #define map(key, sym) scancode[key] = XKeysymToKeycode(display, sym)

    using nall::Keyboard;
    map(Keyboard::Insert, XK_Insert);
    map(Keyboard::Delete, XK_Delete);
    map(Keyboard::Home, XK_Home);
    map(Keyboard::End, XK_End);
    map(Keyboard::PageUp, XK_Prior);
    map(Keyboard::PageDown, XK_Next);

    map(Keyboard::Up, XK_Up);
    map(Keyboard::Down, XK_Down);
    map(Keyboard::Left, XK_Left);
    map(Keyboard::Right, XK_Right);

    #undef map
  }

  inline ~Keyboard() {
    XCloseDisplay(display);
  }

private:
  char state[32];
  uint8_t scancode[256];
};

}
}

#endif
