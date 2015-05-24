#include <SDL/SDL.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "keyboard/xlib.cpp"
#include "mouse/xlib.cpp"
#include "joypad/sdl.cpp"

namespace ruby {

struct pInputSDL {
  InputKeyboardXlib xlibKeyboard;
  InputMouseXlib xlibMouse;
  InputJoypadSDL sdl;

  struct Settings {
    uintptr_t handle = 0;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Input::Handle) return true;
    if(name == Input::KeyboardSupport) return true;
    if(name == Input::MouseSupport) return true;
    if(name == Input::JoypadSupport) return true;
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
    sdl.poll(devices);
    return devices;
  }

  auto rumble(uint64_t id, bool enable) -> bool {
    return false;
  }

  auto init() -> bool {
    if(!xlibKeyboard.init()) return false;
    if(!xlibMouse.init(settings.handle)) return false;
    if(!sdl.init()) return false;
    return true;
  }

  auto term() -> void {
    xlibKeyboard.term();
    xlibMouse.term();
    sdl.term();
  }
};

DeclareInput(SDL)

}
