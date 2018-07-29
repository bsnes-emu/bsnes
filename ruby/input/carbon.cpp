#include "keyboard/carbon.cpp"

struct InputCarbon : Input {
  InputCarbon() : _keyboard(*this) { initialize(); }
  ~InputCarbon() { terminate(); }

  auto driver() -> string override { return "Carbon"; }
  auto ready() -> bool override { return _ready; }

  auto acquired() -> bool override { return false; }
  auto acquire() -> bool override { return false; }
  auto release() -> bool override { return false; }

  auto poll() -> vector<shared_pointer<HID::Device>> override {
    vector<shared_pointer<HID::Device>> devices;
    _keyboard.poll(devices);
    return devices;
  }

  auto rumble(uint64_t id, bool enable) -> bool override {
    return false;
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!_keyboard.initialize()) return false;
    return _ready = true;
  }

  auto terminate() -> void {
    _ready = false;
    _keyboard.terminate();
  }

  bool _ready = false;

  InputKeyboardCarbon _keyboard;
};
