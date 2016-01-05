#include "keyboard/quartz.cpp"

struct InputQuartz : Input {
  InputKeyboardQuartz quartzKeyboard;
  InputQuartz() : quartzKeyboard(*this) {}
  ~InputQuartz() { term(); }

  auto cap(const string& name) -> bool {
    if(name == Input::KeyboardSupport) return true;
    return false;
  }

  auto get(const string& name) -> any {
    return {};
  }

  auto set(const string& name, const any& value) -> bool {
    return false;
  }

  auto acquire() -> bool { return false; }
  auto release() -> bool { return false; }
  auto acquired() -> bool { return false; }

  auto poll() -> vector<shared_pointer<HID::Device>> {
    vector<shared_pointer<HID::Device>> devices;
    quartzKeyboard.poll(devices);
    return devices;
  }

  auto rumble(uint64 id, bool enable) -> bool {
    return false;
  }

  auto init() -> bool {
    if(!quartzKeyboard.init()) return false;
    return true;
  }

  auto term() -> void {
    quartzKeyboard.term();
  }
};
