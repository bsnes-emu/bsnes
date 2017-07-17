#include <SDL/SDL.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "keyboard/xlib.cpp"
#include "mouse/xlib.cpp"
#include "joypad/sdl.cpp"

struct InputSDL : Input {
  InputSDL() : _keyboard(*this), _mouse(*this), _joypad(*this) { initialize(); }
  ~InputSDL() { terminate(); }

  auto ready() -> bool { return _ready; }

  auto context() -> uintptr { return _context; }

  auto setContext(uintptr context) -> bool {
    if(_context == context) return true;
    _context = context;
    return initialize();
  }

  auto acquired() -> bool {
    return _mouse.acquired();
  }

  auto acquire() -> bool {
    return _mouse.acquire();
  }

  auto release() -> bool {
    return _mouse.release();
  }

  auto poll() -> vector<shared_pointer<HID::Device>> {
    vector<shared_pointer<HID::Device>> devices;
    _keyboard.poll(devices);
    _mouse.poll(devices);
    _joypad.poll(devices);
    return devices;
  }

  auto rumble(uint64_t id, bool enable) -> bool {
    return false;
  }

private:
  auto initialize() -> bool {
    terminate();
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
  uintptr _context = 0;

  InputKeyboardXlib _keyboard;
  InputMouseXlib _mouse;
  InputJoypadSDL _joypad;
};
