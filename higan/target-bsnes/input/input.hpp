struct InputMapping {
  auto bind() -> void;
  auto bind(string mapping) -> void;
  auto bind(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue) -> bool;
  auto unbind() -> void;
  auto poll() -> int16;
  auto rumble(bool enable) -> void;
  auto displayName() -> string;

  auto isDigital() const -> bool { return type == 0; }
  auto isAnalog() const -> bool { return type == 1; }
  auto isRumble() const -> bool { return type == 2; }

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
  InputHotkey(string name) { this->name = name; }
  auto& onPress(function<void()> press) { return this->press = press, *this; }
  auto& onRelease(function<void()> release) { return this->release = release, *this; }
//auto logic() const -> Logic override { return Logic::AND; }

  function<auto() -> void> press;
  function<auto() -> void> release;
  int16 state = 0;
};

struct InputDevice {
  uint id;
  string name;
  vector<InputMapping> mappings;
};

struct InputPort {
  uint id;
  string name;
  vector<InputDevice> devices;
};

struct InputManager {
  InputManager();
  auto bind() -> void;
  auto poll() -> void;
  auto onChange(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void;
  auto mapping(uint port, uint device, uint input) -> maybe<InputMapping&>;
  auto findMouse() -> shared_pointer<HID::Device>;

  //hotkeys.cpp
  auto bindHotkeys() -> void;
  auto pollHotkeys() -> void;

public:
  vector<shared_pointer<HID::Device>> devices;
  vector<InputPort> ports;
  vector<InputHotkey> hotkeys;

  uint64 lastPoll;   //time in milliseconds since last call to poll()
  uint64 frequency;  //minimum time in milliseconds before poll() can be called again
};

extern unique_pointer<InputManager> inputManager;
