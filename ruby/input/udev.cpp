#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <libudev.h>
#include <linux/types.h>
#include <linux/input.h>

#include "keyboard/xlib.cpp"
#include "mouse/xlib.cpp"
#include "joypad/udev.cpp"

struct InputUdev : input {
  InputKeyboardXlib xlibKeyboard;
  InputMouseXlib xlibMouse;
  InputJoypadUdev udev;
  Input() : xlibKeyboard(*this), xlibMouse(*this), udev(*this) {}
  ~Input() { term(); }

  struct Settings {
    uintptr_t handle = 0;
  } settings;

  auto cap(const string& name) -> bool {
    if(name == Input::Handle) return true;
    if(name == Input::KeyboardSupport) return true;
    if(name == Input::MouseSupport) return true;
    if(name == Input::JoypadSupport) return true;
    if(name == Input::JoypadRumbleSupport) return true;
    return false;
  }

  auto get(const string& name) -> any {
    if(name == Input::Handle) return settings.handle;
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    if(name == Input::Handle && value.is<uintptr_t>()) {
      settings.handle = value.get<uintptr_t>();
      return true;
    }
    return false;
  }

  auto acquire() -> bool {
    return xlibMouse.acquire();
  }

  auto release() -> bool {
    return xlibMouse.release();
  }

  auto acquired() -> bool {
    return xlibMouse.acquired();
  }

  auto poll() -> vector<shared_pointer<HID::Device>> {
    vector<shared_pointer<HID::Device>> devices;
    xlibKeyboard.poll(devices);
    xlibMouse.poll(devices);
    udev.poll(devices);
    return devices;
  }

  auto rumble(uint64_t id, bool enable) -> bool {
    return udev.rumble(id, enable);
  }

  auto init() -> bool {
    if(xlibKeyboard.init() == false) return false;
    if(xlibMouse.init(settings.handle) == false) return false;
    if(udev.init() == false) return false;
    return true;
  }

  auto term() -> void {
    xlibKeyboard.term();
    xlibMouse.term();
    udev.term();
  }
};
