struct InputMapping {
  auto bind() -> void;
  auto bind(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue) -> bool;
  auto poll() -> int16;
  auto rumble(bool enable) -> void;
  auto unbind() -> void;

  auto isDigital() const -> bool { return !link || link->type == 0; }
  auto isAnalog() const -> bool { return link && link->type == 1; }
  auto isRumble() const -> bool { return link && link->type == 2; }

  auto assignmentName() -> string;
  auto deviceName() -> string;

  string path;  //configuration file key path
  string name;  //input name (human readable)
  string assignment = "None";
  Emulator::Interface::Device::Input* link = nullptr;
  shared_pointer<HID::Device> device;
  uint group = 0;
  uint input = 0;
  enum class Qualifier : uint { None, Lo, Hi, Rumble } qualifier = Qualifier::None;
};

struct InputHotkey : InputMapping {
  function<auto () -> void> press;
  function<auto () -> void> release;

  int16 state = 0;
};

struct InputDevice {
  string name;
  vector<InputMapping*> mappings;  //pointers used so that addresses do not change when arrays are resized
};

struct InputPort {
  string name;
  vector<InputDevice> devices;
};

struct InputEmulator {
  string name;
  vector<InputPort> ports;
};

struct InputManager {
  InputManager();
  auto bind() -> void;
  auto poll() -> void;
  auto onChange(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue) -> void;
  auto quit() -> void;

  auto findMouse() -> shared_pointer<HID::Device>;

  //hotkeys.cpp
  auto appendHotkeys() -> void;
  auto pollHotkeys() -> void;

  vector<shared_pointer<HID::Device>> devices;
  vector<InputEmulator> emulators;
  vector<InputHotkey*> hotkeys;
};

extern InputManager* inputManager;
