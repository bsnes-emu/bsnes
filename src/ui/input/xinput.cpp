#include "xinput.h"

bool InputX::key_down(uint16 key) {
#define map(i) (keymap[i >> 3] & (1 << (i & 7)))
  switch(key) {
  case keymap::esc: return map(0x09);

  case keymap::f1:  return map(0x43);
  case keymap::f2:  return map(0x44);
  case keymap::f3:  return map(0x45);
  case keymap::f4:  return map(0x46);
  case keymap::f5:  return map(0x47);
  case keymap::f6:  return map(0x48);
  case keymap::f7:  return map(0x49);
  case keymap::f8:  return map(0x4a);
  case keymap::f9:  return map(0x4b);
  case keymap::f10: return map(0x4c);
  case keymap::f11: return map(0x5f);
  case keymap::f12: return map(0x60);

  case keymap::print_screen: return map(0x6f);
  case keymap::scroll_lock:  return map(0x4e);
  case keymap::pause:        return map(0x6e);

  case keymap::grave: return map(0x31);

  case keymap::num_1: return map(0x0a);
  case keymap::num_2: return map(0x0b);
  case keymap::num_3: return map(0x0c);
  case keymap::num_4: return map(0x0d);
  case keymap::num_5: return map(0x0e);
  case keymap::num_6: return map(0x0f);
  case keymap::num_7: return map(0x10);
  case keymap::num_8: return map(0x11);
  case keymap::num_9: return map(0x12);
  case keymap::num_0: return map(0x13);

  case keymap::minus:     return map(0x14);
  case keymap::equal:     return map(0x15);
  case keymap::backspace: return map(0x16);

  case keymap::ins:       return map(0x6a);
  case keymap::del:       return map(0x6b);
  case keymap::home:      return map(0x61);
  case keymap::end:       return map(0x67);
  case keymap::page_up:   return map(0x63);
  case keymap::page_down: return map(0x69);

  case keymap::a: return map(0x26);
  case keymap::b: return map(0x38);
  case keymap::c: return map(0x36);
  case keymap::d: return map(0x28);
  case keymap::e: return map(0x1a);
  case keymap::f: return map(0x29);
  case keymap::g: return map(0x2a);
  case keymap::h: return map(0x2b);
  case keymap::i: return map(0x1f);
  case keymap::j: return map(0x2c);
  case keymap::k: return map(0x2d);
  case keymap::l: return map(0x2e);
  case keymap::m: return map(0x3a);
  case keymap::n: return map(0x39);
  case keymap::o: return map(0x20);
  case keymap::p: return map(0x21);
  case keymap::q: return map(0x18);
  case keymap::r: return map(0x1b);
  case keymap::s: return map(0x27);
  case keymap::t: return map(0x1c);
  case keymap::u: return map(0x1e);
  case keymap::v: return map(0x37);
  case keymap::w: return map(0x19);
  case keymap::x: return map(0x35);
  case keymap::y: return map(0x1d);
  case keymap::z: return map(0x34);

  case keymap::lbracket:   return map(0x22);
  case keymap::rbracket:   return map(0x23);
  case keymap::backslash:  return map(0x33);
  case keymap::semicolon:  return map(0x2f);
  case keymap::apostrophe: return map(0x30);
  case keymap::comma:      return map(0x3b);
  case keymap::period:     return map(0x3c);
  case keymap::slash:      return map(0x3d);

  case keymap::kp_1: return map(0x57);
  case keymap::kp_2: return map(0x58);
  case keymap::kp_3: return map(0x59);
  case keymap::kp_4: return map(0x53);
  case keymap::kp_5: return map(0x54);
  case keymap::kp_6: return map(0x55);
  case keymap::kp_7: return map(0x4f);
  case keymap::kp_8: return map(0x50);
  case keymap::kp_9: return map(0x51);

  case keymap::kp_plus:  return map(0x56);
  case keymap::kp_minus: return map(0x52);
  case keymap::kp_mul:   return map(0x3f);
  case keymap::kp_div:   return map(0x70);
  case keymap::kp_enter: return map(0x6c);

  case keymap::num_lock:  return map(0x4d);
  case keymap::caps_lock: return map(0x42);

  case keymap::up:    return map(0x62);
  case keymap::down:  return map(0x68);
  case keymap::left:  return map(0x64);
  case keymap::right: return map(0x66);

  case keymap::tab:   return map(0x17);
  case keymap::enter: return map(0x24);
  case keymap::space: return map(0x41);

  case keymap::lctrl:  return map(0x25);
  case keymap::rctrl:  return map(0x6d);
  case keymap::lalt:   return map(0x40);
  case keymap::ralt:   return map(0x71);
  case keymap::lshift: return map(0x32);
  case keymap::rshift: return map(0x3e);
  case keymap::lsuper: return map(0x73);
  case keymap::rsuper: return map(0x74);
  case keymap::menu:   return map(0x75);
  }
#undef map

  return false;
}

void InputX::clear_input() {
  memset(keymap, 0, sizeof keymap);
}

void InputX::poll() {
  XQueryKeymap(display, keymap);
}

void InputX::init() {
  Input::init();
  display = XOpenDisplay(0);
}

void InputX::term() {
  Input::term();
}
