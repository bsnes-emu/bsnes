struct InputMapping {
  auto bind() -> void;
  auto bind(string mapping) -> void;
  auto bind(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue) -> bool;
  auto poll() -> int16;
  auto rumble(bool enable) -> void;
  auto unbind() -> void;

  auto isDigital() const -> bool { return type == 0; }
  auto isAnalog() const -> bool { return type == 1; }
  auto isRumble() const -> bool { return type == 2; }

  auto displayName() -> string;

  string path;  //configuration file key path
  string name;  //input name (human readable)
  uint type = 0;
  string assignment = "None";

  enum class Logic : uint { AND, OR };
  enum class Qualifier : uint { None, Lo, Hi, Rumble };
  virtual auto logic() const -> Logic { return Logic::OR; }
  struct Mapping {
    shared_pointer<HID::Device> device;
    uint group = 0;
    uint input = 0;
    Qualifier qualifier = Qualifier::None;
  };
  vector<Mapping> mappings;
};

struct InputHotkey : InputMapping {
  auto logic() const -> Logic override { return Logic::AND; }

  function<auto () -> void> press;
  function<auto () -> void> release;

  int16 state = 0;
};

struct InputDevice {
  string name;
  vector<InputMapping> mappings;
};

struct InputPort {
  string name;
  vector<InputDevice> devices;
};

struct InputEmulator {
  Emulator::Interface* interface = nullptr;
  string name;
  vector<InputPort> ports;
};

struct InputManager {
  InputManager();
  auto bind(Emulator::Interface*) -> void;
  auto bind() -> void;
  auto poll() -> void;
  auto onChange(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void;
  auto quit() -> void;

  auto findMouse() -> shared_pointer<HID::Device>;

  //hotkeys.cpp
  auto appendHotkeys() -> void;
  auto pollHotkeys() -> void;

  vector<shared_pointer<HID::Device>> devices;
  vector<InputEmulator> emulators;
  vector<InputHotkey*> hotkeys;

  InputEmulator* emulator = nullptr;  //points to InputEmulator that represents the currently active emulator
  uint64 lastPoll;   //time in milliseconds since last call to poll()
  uint64 frequency;  //minimum time in milliseconds before poll() can be called again
};

extern unique_pointer<InputManager> inputManager;
