#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

namespace ruby {

#include "x.hpp"

class pInputX {
public:
  InputX &self;
  Display *display;

  bool cap(Input::Setting setting) {
    if(setting == Input::KeyboardSupport) return true;
    return false;
  }

  uintptr_t get(Input::Setting setting) {
    return false;
  }

  bool set(Input::Setting setting, uintptr_t param) {
    return false;
  }

  bool poll(int16_t *table) {
    memset(table, 0, input_limit * sizeof(int16_t));

    #define key(n) (bool)(state[n >> 3] & (1 << (n & 7)))
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
    #undef key

    return true;
  }

  bool init() {
    display = XOpenDisplay(0);
    return true;
  }

  void term() {
  }

  pInputX(InputX &self_) : self(self_) {}
};

bool InputX::cap(Setting setting) { return p.cap(setting); }
uintptr_t InputX::get(Setting setting) { return p.get(setting); }
bool InputX::set(Setting setting, uintptr_t param) { return p.set(setting, param); }
bool InputX::poll(int16_t *table) { return p.poll(table); }
bool InputX::init() { return p.init(); }
void InputX::term() { p.term(); }
InputX::InputX() : p(*new pInputX(*this)) {}
InputX::~InputX() { delete &p; }

} //namespace ruby
