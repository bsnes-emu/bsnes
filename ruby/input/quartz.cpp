#include "keyboard/quartz.cpp"

struct InputQuartz : Input {
  InputQuartz() : _keyboard(*this) { initialize(); }
  ~InputQuartz() { terminate(); }

  auto ready() -> bool { return _ready; }

  auto acquired() -> bool { return false; }
  auto acquire() -> bool { return false; }
  auto release() -> bool { return false; }

  auto poll() -> vector<shared_pointer<HID::Device>> {
    vector<shared_pointer<HID::Device>> devices;
    _keyboard.poll(devices);
    return devices;
  }

  auto rumble(uint64 id, bool enable) -> bool {
    return false;
  }

  auto initialize() -> bool {
    terminate();
    if(!_keyboard.init()) return false;
    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;
    _keyboard.term();
  }

  bool _ready = false;

  InputKeyboardQuartz _keyboard;
};
