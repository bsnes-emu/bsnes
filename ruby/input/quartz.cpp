#include "keyboard/quartz.cpp"

struct InputQuartz : InputDriver {
  InputQuartz(Input& super) : InputDriver(super), keyboard(super) {}
  ~InputQuartz() { terminate(); }

  auto create() -> bool override {
    return initialize();
  }

  auto driver() -> string override { return "Quartz"; }
  auto ready() -> bool override { return isReady; }

  auto acquired() -> bool override { return false; }
  auto acquire() -> bool override { return false; }
  auto release() -> bool override { return false; }

  auto poll() -> vector<shared_pointer<HID::Device>> override {
    vector<shared_pointer<HID::Device>> devices;
    keyboard.poll(devices);
    return devices;
  }

  auto rumble(uint64_t id, bool enable) -> bool override {
    return false;
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!keyboard.initialize()) return false;
    return _ready = true;
  }

  auto terminate() -> void {
    isReady = false;
    keyboard.terminate();
  }

  InputQuartz& self = *this;
  bool isReady = false;
  InputKeyboardQuartz keyboard;
};
