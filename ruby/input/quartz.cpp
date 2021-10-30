#include "keyboard/quartz.cpp"
#include "mouse/quartz.cpp"
#include "joypad/iokit.cpp"

struct InputQuartz : InputDriver {
  InputQuartz& self = *this;
  InputQuartz(Input& super) : InputDriver(super), keyboard(super), mouse(super), joypad(super) {}
  ~InputQuartz() { terminate(); }

  auto create() -> bool override {
    return initialize();
  }

  auto driver() -> string override { return "Quartz"; }
  auto ready() -> bool override { return isReady; }

  auto acquired() -> bool override { return mouse.acquired(); }
  auto acquire() -> bool override { return mouse.acquire(); }
  auto release() -> bool override { return mouse.release(); }

  auto poll() -> vector<shared_pointer<HID::Device>> override {
    vector<shared_pointer<HID::Device>> devices;
    keyboard.poll(devices);
    mouse.poll(devices);
    joypad.poll(devices);
    return devices;
  }

  auto rumble(uint64_t id, bool enable) -> bool override {
    return false;
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!keyboard.initialize()) return false;
    if(!mouse.initialize()) return false;
    if(!joypad.initialize()) return false;
    return isReady = true;
  }

  auto terminate() -> void {
    isReady = false;
    keyboard.terminate();
    mouse.terminate();
    joypad.terminate();
  }

  bool isReady = false;
  InputKeyboardQuartz keyboard;
  InputMouseQuartz mouse;
  InputJoypadIOKit joypad;
};
