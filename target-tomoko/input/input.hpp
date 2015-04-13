struct InputMapping {
  auto bind() -> void;
  auto bind(HID::Device& device, unsigned group, unsigned input, int16 oldValue, int16 newValue) -> bool;
  auto poll() -> int16;
  auto unbind() -> void;

  string name;
  string assignment = "None";
  Emulator::Interface::Device::Input* link = nullptr;
  HID::Device* device = nullptr;
  unsigned group = 0;
  unsigned input = 0;
};

struct InputHotkey : InputMapping {
  function<void ()> action;

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
  auto onChange(HID::Device& device, unsigned group, unsigned input, int16 oldValue, int16 newValue) -> void;
  auto quit() -> void;

  //hotkeys.cpp
  auto appendHotkeys() -> void;
  auto pollHotkeys() -> void;

  vector<HID::Device*> devices;
  vector<InputEmulator> emulators;
  vector<InputHotkey*> hotkeys;
  Configuration::Document config;
};

extern InputManager* inputManager;
