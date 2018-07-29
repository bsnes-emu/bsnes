#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include "keyboard/xlib.cpp"
#include "mouse/xlib.cpp"

struct InputXlib : Input {
  InputXlib() : _keyboard(*this), _mouse(*this) { initialize(); }
  ~InputXlib() { terminate(); }

  auto driver() -> string override { return "Xlib"; }
  auto ready() -> bool override { return _ready; }

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
    return devices;
  }

  auto rumble(uint64_t id, bool enable) -> bool override {
    return false;
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!_context) return false;
    if(!_keyboard.initialize()) return false;
    if(!_mouse.initialize(_context)) return false;
    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;
    _keyboard.terminate();
    _mouse.terminate();
  }

  bool _ready = false;

  InputKeyboardXlib _keyboard;
  InputMouseXlib _mouse;
};
