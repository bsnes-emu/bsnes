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
  #include "xlibkeys.hpp"

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

    char state[32];
    XQueryKeymap(display, state);

    for(unsigned i = 0; i < keyboard<>::length; i++) {
      uint8_t code = keycode[i];
      if(code == 0) continue;  //unmapped
      table[i] = (bool)(state[code >> 3] & (1 << (code & 7)));
    }

    return true;
  }

  bool init() {
    init_keycodes();
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
