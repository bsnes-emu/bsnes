#include "keyboard/carbon.cpp"

struct InputCarbon : Input {
  InputCarbon() : _keyboard(*this) { initialize(); }
  ~InputCarbon() { terminate(); }

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

private:
  auto initialize() -> bool {
    terminate();
    if(!_keyboard.initialize()) return false;
    return _ready = true;
  }

  auto term() -> void {
    _ready = false;
    _keyboard.terminate();
  }

  bool _ready = false;

  InputKeyboardCarbon _keyboard;
};
