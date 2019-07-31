#if defined(PLATFORM_WINDOWS) || defined(PLATFORM_MACOS)
//TODO: hiro support for TableView::activate(TableViewCell) is required for > 1 binding
enum : uint { BindingLimit = 1 };
#else
enum : uint { BindingLimit = 4 };
#endif

struct InputMapping {
  auto bind() -> void;
  auto bind(string mapping, uint binding) -> void;
  auto bind(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue, uint binding) -> bool;
  auto unbind(uint binding) -> void;
  auto poll() -> int16;
  auto rumble(bool enable) -> void;

  using Type = Emulator::Interface::Input::Type;
  auto isDigital() const -> bool {
    return type == Type::Hat || type == Type::Button || type == Type::Trigger || type == Type::Control;
  }
  auto isAnalog() const -> bool {
    return type == Type::Axis;
  }
  auto isRumble() const -> bool {
    return type == Type::Rumble;
  }

  uint portID = 0;
  uint deviceID = 0;
  uint inputID = 0;
  maybe<uint> turboID;

  string path;  //configuration file key path
  string name;  //input name (human readable)
  uint type = 0;
  string assignments[BindingLimit];

  enum class Logic : uint { AND, OR };
  enum class Qualifier : uint { None, Lo, Hi, Rumble };
  virtual auto logic() const -> Logic { return Logic::OR; }

  struct Binding {
    auto icon() -> image;
    auto name() -> string;

    shared_pointer<HID::Device> device;
    uint group = 0;
    uint input = 0;
    Qualifier qualifier = Qualifier::None;
  };
  Binding bindings[BindingLimit];

  uint3 turboCounter = 0;
};

struct InputHotkey : InputMapping {
  InputHotkey(string name) { this->name = name; }
  auto& onPress(function<void ()> press) { return this->press = press, *this; }
  auto& onRelease(function<void ()> release) { return this->release = release, *this; }
  auto logic() const -> Logic override;

  function<void ()> press;
  function<void ()> release;
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
  InputMapping::Logic hotkeyLogic = InputMapping::Logic::OR;

  auto initialize() -> void;
  auto bind() -> void;
  auto poll() -> void;
  auto frame() -> void;
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

  uint64 lastPoll = 0;   //time in milliseconds since last call to poll()
  uint64 frequency = 0;  //minimum time in milliseconds before poll() can be called again

  uint turboCounter = 0;
  uint turboFrequency = 0;
};

extern InputManager inputManager;
