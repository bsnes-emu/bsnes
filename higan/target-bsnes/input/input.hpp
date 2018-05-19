struct InputMapping {
  auto poll() -> int16;

  string path;  //configuration file key path
  string name;  //input name (human readable)
  uint type = 0;
  string assignment = "None";
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
  auto poll() -> void;
  auto onChange(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void;
  auto mapping(uint port, uint device, uint input) -> maybe<InputMapping&>;

private:
  vector<shared_pointer<HID::Device>> devices;
  vector<InputPort> ports;

  uint64 lastPoll;   //time in milliseconds since last call to poll()
  uint64 frequency;  //minimum time in milliseconds before poll() can be called again
};

extern unique_pointer<InputManager> inputManager;
