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

namespace ruby {

struct pInputUdev {
  InputKeyboardXlib xlibKeyboard;
  InputMouseXlib xlibMouse;
  InputJoypadUdev udev;

  struct Settings {
    uintptr_t handle = 0;
  } settings;

  bool cap(const string& name) {
    if(name == Input::Handle) return true;
    if(name == Input::KeyboardSupport) return true;
    if(name == Input::MouseSupport) return true;
    if(name == Input::JoypadSupport) return true;
    if(name == Input::JoypadRumbleSupport) return true;
    return false;
  }

  any get(const string& name) {
    if(name == Input::Handle) return (uintptr_t)settings.handle;
    return false;
  }

  bool set(const string& name, const any& value) {
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
    udev.poll(devices);
    return devices;
  }

  bool rumble(uint64_t id, bool enable) {
    return udev.rumble(id, enable);
  }

  bool init() {
    if(xlibKeyboard.init() == false) return false;
    if(xlibMouse.init(settings.handle) == false) return false;
    if(udev.init() == false) return false;
    return true;
  }

  void term() {
    xlibKeyboard.term();
    xlibMouse.term();
    udev.term();
  }
};

DeclareInput(Udev)

}
