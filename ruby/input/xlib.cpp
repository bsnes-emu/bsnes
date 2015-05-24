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

  auto cap(const string& name) -> bool {
    if(name == Input::KeyboardSupport) return true;
    if(name == Input::MouseSupport) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Input::Handle) return (uintptr_t)settings.handle;
    return false;
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Input::Handle) {
      settings.handle = any_cast<uintptr_t>(value);
      return true;
    }

    return false;
  }

  auto acquire() -> bool {
    return xlibMouse.acquire();
  }

  auto unacquire() -> bool {
    return xlibMouse.unacquire();
  }

  auto acquired() -> bool {
    return xlibMouse.acquired();
  }

  auto poll() -> vector<shared_pointer<HID::Device>> {
    vector<shared_pointer<HID::Device>> devices;
    xlibKeyboard.poll(devices);
    xlibMouse.poll(devices);
    return devices;
  }

  auto rumble(uint64_t id, bool enable) -> bool {
    return false;
  }

  auto init() -> bool {
    if(!xlibKeyboard.init()) return false;
    if(!xlibMouse.init(settings.handle)) return false;
    return true;
  }

  auto term() -> void {
    xlibKeyboard.term();
    xlibMouse.term();
  }
};

DeclareInput(Xlib)

}
