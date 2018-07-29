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

struct InputUdev : Input {
  InputUdev() : _keyboard(*this), _mouse(*this), _joypad(*this) { initialize(); }
  ~InputUdev() { terminate(); }

  auto driver() -> string override { return "udev"; }
  auto ready() -> bool { return _ready; }

  auto hasContext() -> bool override { return true; }

  auto setContext(uintptr context) -> bool override {
    if(context == Input::context()) return true;
    if(!Input::setContext(context)) return false;
    return initialize();
  }

  auto acquired() -> bool override {
    return _mouse.acquired();
  }

  auto acquire() -> bool override {
    return _mouse.acquire();
  }

  auto release() -> bool override {
    return _mouse.release();
  }

  auto poll() -> vector<shared_pointer<HID::Device>> override {
    vector<shared_pointer<HID::Device>> devices;
    _keyboard.poll(devices);
    _mouse.poll(devices);
    _joypad.poll(devices);
    return devices;
  }

  auto rumble(uint64_t id, bool enable) -> bool override {
    return _joypad.rumble(id, enable);
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!_context) return false;
    if(!_keyboard.initialize()) return false;
    if(!_mouse.initialize(_context)) return false;
    if(!_joypad.initialize()) return false;
    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;
    _keyboard.terminate();
    _mouse.terminate();
    _joypad.terminate();
  }

  bool _ready = false;

  InputKeyboardXlib _keyboard;
  InputMouseXlib _mouse;
  InputJoypadUdev _joypad;
};
