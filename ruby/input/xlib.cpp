#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include "keyboard/xlib.cpp"
#include "mouse/xlib.cpp"

namespace ruby {

struct pInputXlib {
  InputKeyboardXlib xlibKeyboard;
  InputMouseXlib xlibMouse;

  struct Settings {
    uintptr_t handle = 0;
  } settings;

  bool cap(const string& name) {
    if(name == Input::KeyboardSupport) return true;
    if(name == Input::MouseSupport) return true;
    return false;
  }

  any get(const string& name) {
    if(name == Input::Handle) return (uintptr_t)settings.handle;
    return false;
  }

  bool set(const string& name, const any &value) {
    if(name == Input::Handle) {
      settings.handle = any_cast<uintptr_t>(value);
      return true;
    }

    return false;
  }

  bool acquire() {
    return xlibMouse.acquire();
  }

  bool unacquire() {
    return xlibMouse.unacquire();
  }

  bool acquired() {
    return xlibMouse.acquired();
  }

  vector<HID::Device*> poll() {
    vector<HID::Device*> devices;
    xlibKeyboard.poll(devices);
    xlibMouse.poll(devices);
    return devices;
  }

  bool rumble(uint64_t id, bool enable) {
    return false;
  }

  bool init() {
    if(xlibKeyboard.init() == false) return false;
    if(xlibMouse.init(settings.handle) == false) return false;
    return true;
  }

  void term() {
    xlibKeyboard.term();
    xlibMouse.term();
  }
};

DeclareInput(Xlib)

}
