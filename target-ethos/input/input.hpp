extern HID::Null hidNull;

struct AbstractInput {
  string name;
  string mapping;
  bool logic = 0;  //0 = OR, 1 = AND
  bool state = 0;

  struct Input {
    HID::Device* device = nullptr;
    uint64_t id = 0;
    unsigned group = 0;
    unsigned input = 0;
    enum class Qualifier : unsigned { None, Lo, Hi } qualifier;
  };
  vector<Input> inputList;

  void bind();
  bool append(string mapping);
  virtual bool bind(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue) { return false; }
  virtual int16_t poll() { return 0; }
  virtual void rumble(bool enable) {}
};

struct DigitalInput : AbstractInput {
  using AbstractInput::bind;
  bool bind(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue);
  int16_t poll();
};

struct RelativeInput : AbstractInput {
  using AbstractInput::bind;
  bool bind(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue);
  int16_t poll();
};

struct RumbleInput : AbstractInput {
  using AbstractInput::bind;
  bool bind(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue);
  void rumble(bool enable);
};

struct HotkeyInput : DigitalInput {
  function<void ()> press;
  function<void ()> release;
  HotkeyInput();
};

struct InputManager {
  vector<HID::Device*> devices;
  vector<AbstractInput*> inputMap;
  vector<HotkeyInput*> hotkeyMap;

  void onChange(HID::Device& device, unsigned group, unsigned input, int16_t oldValue, int16_t newValue);
  HID::Device* findMouse();
  void bind();
  void poll();
  void saveConfiguration();
  void bootstrap();
  InputManager();

  //hotkeys.cpp
  void appendHotkeys();
  void pollHotkeys();

private:
  Configuration::Document config;
};

extern InputManager* inputManager;
