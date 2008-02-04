#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
#include <X11/extensions/XShm.h>

#include <ruby/ruby.h>

namespace ruby {

#include "x.h"

class pInputX {
public:
  InputX &self;
  Display *display;
  char keymap[32];

  bool key_down(uint16_t key) {
    using namespace nall;
    #define map(i) (keymap[i >> 3] & (1 << (i & 7)))
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
    return false;
  }

  void clear() {
    memset(keymap, 0, sizeof keymap);
  }

  void poll() {
    XQueryKeymap(display, keymap);
  }

  bool init() {
    display = XOpenDisplay(0);
    return true;
  }

  void term() {
  }

  pInputX(InputX &self_) : self(self_) {}
};

bool InputX::key_down(uint16_t key) { return p.key_down(key); }
void InputX::clear() { p.clear(); }
void InputX::poll() { p.poll(); }
bool InputX::init() { return p.init(); }
void InputX::term() { p.term(); }
InputX::InputX() : p(*new pInputX(*this)) {}
InputX::~InputX() { delete &p; }

} //namespace ruby
